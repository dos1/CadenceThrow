/*! \file intro.c
 *  \brief Intro view with plot description.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <allegro5/allegro_video.h>
#include "intro.h"
#include "pause.h"

ALLEGRO_VIDEO *videos[4] = {};
int durations[4] = {8, 1, 10, 1};
ALLEGRO_BITMAP *bitmap = NULL;

double oldpos;
int counter = -1;
int power = -1;

bool won = false, lost = false;

bool CheckPos(int id) {
	float pos = al_get_video_position(videos[id], 0);
	if (pos < durations[id]) return false;
	if (al_is_video_paused(videos[id])) return false;
	if (pos==0) return false;

	if (pos==oldpos) {
		counter++;
		if (counter>5) return true;
		return false;
	}
	oldpos = pos;
	counter = 0;
	return false;
}

bool initial = true;
int cur = 0;


void Intro_Logic(struct Game *game) {}

void Intro_Pause(struct Game *game) {
	al_pause_video(videos[cur], true);
}

void Intro_Resume(struct Game *game) {
	al_seek_video(videos[cur], 3.45);
	al_pause_video(videos[cur], false);
}


void NextVideo(struct Game *game, int inc) {
	//al_close_video(videos[cur]);
	cur+=inc;
	al_start_video(videos[cur], game->audio.mixer);
	al_pause_video(videos[cur], false);
}

void Intro_Draw(struct Game *game) {

	if (won || lost) {
		if (bitmap) al_draw_scaled_bitmap(bitmap, 0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), 0, 0, al_get_display_width(game->display), al_get_display_height(game->display), 0);
		char* text;
		if (won) text = "You won!";
		if (lost) text = "You lost!";
		al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.4, ALLEGRO_ALIGN_CENTRE, text);
		return;
	}

	//al_clear_to_color(al_map_rgb(255,0,0));
	//al_start_video(video, game->audio.mixer);
	if (videos[cur]) bitmap = al_get_video_frame(videos[cur]);
	if (CheckPos(cur)) {
		if (cur==2) won=true;
		else if (cur>2) lost=true;
		else {
			int inc = 1;
			if (cur==1) {
				PrintConsole(game, "power status: %d", power);
				if (power < 50) inc = 2;
			}
			NextVideo(game,inc);
		}
	}
	if (bitmap) al_draw_scaled_bitmap(bitmap, 0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), 0, 0, al_get_display_width(game->display), al_get_display_height(game->display), 0);
	if (cur == 0 && al_get_video_position(videos[cur], 0) > 3.45 && initial) {
		initial=false;
		game->gamestate=GAMESTATE_PAUSE;
		game->loadstate=GAMESTATE_INTRO;
		PauseGameState(game);
		Pause_Load(game);
		return;
	}
	//PrintConsole(game, "%f %f %f", al_get_video_position(videos[0], 0), al_get_video_position(videos[1], 0), al_get_video_position(videos[2], 0));
	//PrintConsole(game, "%d", GetCurrentPower());
}

void Intro_Load(struct Game *game) {
	al_start_video(videos[cur], game->audio.mixer);
	//al_play_sample_instance(game->intro.music);
	//FadeGameState(game, true);
	//al_set_audio_stream_playing(game->intro.audiostream, true);
}

int GetCurrentPower() {
	if (power>=0) return power;
	float pos = al_get_video_position(videos[cur], 0);
	if (cur == 0 && pos >= 8 && pos <= 8.5) {
		return (int)((pos-8)*200);
	} else {
		return -1;
	}
}

int Intro_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		return 1;
	}
	if (ev->keyboard.keycode == ALLEGRO_KEY_ENTER && (won || lost)) {
		won = false;
		lost = false;
		cur = 0;
		initial = true;
		power = -1;
		al_close_video(videos[0]);
		al_close_video(videos[1]);
		al_close_video(videos[2]);
		al_close_video(videos[3]);
		videos[0] = al_open_video("data/1.avi");
		videos[1] = al_open_video("data/2.avi");
		videos[2] = al_open_video("data/3.avi");
		videos[3] = al_open_video("data/fail.avi");
		al_start_video(videos[cur], game->audio.mixer);
		//al_seek_video(videos[cur], 3.45);
	}
	if (ev->keyboard.keycode == ALLEGRO_KEY_SPACE && cur == 0 && !initial) {
		int pwr;
		if ((pwr = GetCurrentPower()) != -1) {
			PrintConsole(game, "POWER: %d", pwr);
			power = pwr;
		}
	}
	return 0;
}

void Intro_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	//Menu_Preload(game, progress);
	PROGRESS_INIT(5);
	Pause_Preload(game);
	PROGRESS;
	PrintConsole(game,"Opening video");
	videos[0] = al_open_video("data/1.avi");
	PROGRESS;
	videos[1] = al_open_video("data/2.avi");
	PROGRESS;
	videos[2] = al_open_video("data/3.avi");
	PROGRESS;
	videos[3] = al_open_video("data/fail.avi");
	PrintConsole(game,"Opening video DONE");
	PROGRESS;
}

void Intro_Unload(struct Game *game) {
	//FadeGameState(game, false);
	//al_pause_video(video, true);
	al_close_video(videos[0]);
	al_close_video(videos[1]);
	al_close_video(videos[2]);
	//video=NULL;
}
