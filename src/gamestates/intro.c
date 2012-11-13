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
#include <stdio.h>
#include "intro.h"
#include "pause.h"

ALLEGRO_VIDEO *videos[5] = {};
int durations[5] = {3, 5, 1, 10, 1};
ALLEGRO_BITMAP *bitmap = NULL;

double oldpos;
int counter = -1;
int power = -1;

bool paused = false;

bool won = false, lost = false;

bool CheckPos(int id) {
	if (id>10) return false;
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

int cur = 0;

void NextVideo(struct Game *game, int inc) {
	//al_close_video(videos[cur]);
	cur+=inc;
	al_start_video(videos[cur], game->audio.voice);
	al_pause_video(videos[cur], false);

	switch (cur) {
		case 1:
			al_set_sample_instance_playing(game->intro.music2,true);
			al_set_sample_instance_position(game->intro.music2, 50000);
			break;
		case 2:
			al_set_sample_instance_playing(game->intro.music3,true);
			break;
		case 3:
			al_set_sample_instance_playing(game->intro.music4,true);
			break;
		case 4:
			al_set_sample_instance_playing(game->intro.music5,true);
			break;
	}
}

int GetCurrentPower() {
	if (power>=0) return power;
	float pos = al_get_video_position(videos[cur], 0);
	if (cur == 1 && pos >= 4.5 && pos <= 5) {
		return (int)((pos-4.5)*200);
	} else {
		if (pos > 5) return 100;
		return -1;
	}
}

void Intro_Logic(struct Game *game) {
	if (CheckPos(cur)) {
		if (cur==0) {
			game->gamestate=GAMESTATE_PAUSE;
			game->loadstate=GAMESTATE_INTRO;
			PauseGameState(game);
			Pause_Load(game);
			return;
		}
		else if (cur==3) {
			won=true;
			al_set_sample_instance_position(game->intro.musicwin, 0);
			al_play_sample_instance(game->intro.musicwin);
			cur=255;
		}
		else if (cur==4) {
			lost=true;
			al_set_sample_instance_position(game->intro.musiclost, 0);
			al_play_sample_instance(game->intro.musiclost);
			game->intro.cadencepos = 0;
			cur=255;
		}
		else {
			int inc = 1;
			if (cur==2) {
				double p = (0.21+((0.79-0.21)*(power/100.0)));
				PrintConsole(game, "power status: %d, %f, %f", power, p, 0.7-(game->intro.range/100.0));
				if (!((p>=0.7-(game->intro.range/100.0)) && p<=0.7)) inc = 2;
			}
			NextVideo(game,inc);
		}
	}
	//PrintConsole(game, "%d", GetCurrentPower());
}

void Intro_Pause(struct Game *game) {
	//al_pause_video(videos[cur], true);
	paused = true;
}

void Intro_Resume(struct Game *game) {
	//al_seek_video(videos[cur], 3.45);
	//al_pause_video(videos[cur], false);
	won = false;
	lost = false;
	cur = 0;
	power = -1;
	al_stop_sample_instance(game->intro.musicwin);
	al_stop_sample_instance(game->intro.musiclost);
	al_set_sample_instance_playing(game->intro.music, false);
	NextVideo(game, 1);
	paused = false;
}

void Intro_Draw(struct Game *game) {

	if (won || lost) {
		if (bitmap) al_draw_scaled_bitmap(bitmap, 0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), 0, 0, game->viewportWidth, game->viewportHeight, 0);
		char* text;
		if (won) text = "You won!";
		if (lost) { text = "You lost!";
			al_draw_bitmap(game->intro.space, 0, 0, 0);
			al_draw_rotated_bitmap(game->intro.cadence, 0, 0, game->viewportWidth*(game->intro.cadencepos/1000.0), game->viewportHeight*(1-(game->intro.cadencepos/1100.0)), game->intro.cadencepos/256.0, 0);
			game->intro.cadencepos++;
		}
		if (!paused) {
			al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.4, ALLEGRO_ALIGN_CENTRE, text);
			al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, "Press Enter to play again!");
		}
		return;
	}

	//al_clear_to_color(al_map_rgb(255,0,0));
	//al_start_video(video, game->audio.voice);
	if (videos[cur]) bitmap = al_get_video_frame(videos[cur]);

	if (bitmap) al_draw_scaled_bitmap(bitmap, 0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), 0, 0, game->viewportWidth, game->viewportHeight, 0);

	//PrintConsole(game, "%f %f %f", al_get_video_position(videos[0], 0), al_get_video_position(videos[1], 0), al_get_video_position(videos[2], 0));
	//PrintConsole(game, "%d", GetCurrentPower());

	if (cur == 1) {
		int p = GetCurrentPower();
		if (power != -1) p = power;
		if (p==-1) {
			float pos = al_get_video_position(videos[cur], 0);
			if (pos >= 3 && pos < 5) {
				al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.8, ALLEGRO_ALIGN_CENTRE, "Prepare...");
			}
		} else {

			int color = (power > -1) ? 220 : 50;

			al_draw_filled_rectangle(game->viewportWidth*0.2, game->viewportHeight*0.8, game->viewportWidth*0.8, game->viewportHeight*0.9, al_map_rgb(color/5,color/4,color));

			al_draw_filled_rectangle(game->viewportWidth*0.21, game->viewportHeight*0.81, game->viewportWidth*0.79, game->viewportHeight*0.89, al_map_rgb(255,50,50));

			al_draw_filled_rectangle(game->viewportWidth*(0.7-(game->intro.range/100.0)), game->viewportHeight*0.81, game->viewportWidth*0.7, game->viewportHeight*0.89, al_map_rgb(150,255,50));

			al_draw_filled_rectangle(game->viewportWidth*0.21, game->viewportHeight*0.81, game->viewportWidth*(0.21+((0.79-0.21)*(p/100.0))), game->viewportHeight*0.89, al_map_rgb(255,255,150));

			char *pstr = malloc(5*sizeof(char));
			sprintf(pstr, "%d%%", p);
			al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.8, ALLEGRO_ALIGN_CENTRE, pstr);
			free(pstr);

		}
	}

}

void Intro_Load(struct Game *game) {
	won = false;
	lost = false;
	cur = 0;
	power = -1;
	al_start_video(videos[cur], game->audio.voice);
	al_set_sample_instance_position(game->intro.music, 0);
	al_play_sample_instance(game->intro.music);
	//FadeGameState(game, true);
	//al_set_audio_stream_playing(game->intro.audiostream, true);
}

int Intro_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		return 1;
	}
	if (ev->keyboard.keycode == ALLEGRO_KEY_ENTER && (won || lost)) {
		al_close_video(videos[0]);
		al_close_video(videos[1]);
		al_close_video(videos[2]);
		al_close_video(videos[3]);
		al_close_video(videos[4]);
		videos[0] = al_open_video("data/intro.avi");
		videos[1] = al_open_video("data/prepare.avi");
		videos[2] = al_open_video("data/throw.avi");
		videos[3] = al_open_video("data/success.avi");
		videos[4] = al_open_video("data/fail.avi");
		game->gamestate=GAMESTATE_PAUSE;
		game->loadstate=GAMESTATE_INTRO;
		PauseGameState(game);
		Pause_Load(game);
		//al_seek_video(videos[cur], 3.45);
	}
	if ((ev->keyboard.keycode == ALLEGRO_KEY_SPACE || ev->keyboard.keycode == ALLEGRO_KEY_ENTER) && cur == 1) {
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
	PROGRESS_INIT(17);
	Pause_Preload(game);
	PROGRESS;
	PrintConsole(game,"Opening video");
	videos[0] = al_open_video("data/intro.avi");
	PROGRESS;
	videos[1] = al_open_video("data/prepare.avi");
	PROGRESS;
	videos[2] = al_open_video("data/throw.avi");
	PROGRESS;
	videos[3] = al_open_video("data/success.avi");
	PROGRESS;
	videos[4] = al_open_video("data/fail.avi");
	PrintConsole(game,"Opening video DONE");
	PROGRESS;

	game->intro.sample = al_load_sample( GetDataFilePath("intro.flac") );
	PROGRESS;

	game->intro.music = al_create_sample_instance(game->intro.sample);
	al_attach_sample_instance_to_mixer(game->intro.music, game->audio.music);
	//al_set_sample_instance_gain(game->intro.music, 0.75);
	al_set_sample_instance_playmode(game->intro.music, ALLEGRO_PLAYMODE_LOOP);

	game->intro.sample2 = al_load_sample( GetDataFilePath("prepare.flac") );
	PROGRESS;

	game->intro.music2 = al_create_sample_instance(game->intro.sample2);
	al_attach_sample_instance_to_mixer(game->intro.music2, game->audio.music);
	//al_set_sample_instance_gain(game->intro.music, 0.75);
	al_set_sample_instance_playmode(game->intro.music2, ALLEGRO_PLAYMODE_ONCE);

	game->intro.sample3 = al_load_sample( GetDataFilePath("throw.flac") );
	PROGRESS;

	game->intro.music3 = al_create_sample_instance(game->intro.sample3);
	al_attach_sample_instance_to_mixer(game->intro.music3, game->audio.music);
	//al_set_sample_instance_gain(game->intro.music, 0.75);
	al_set_sample_instance_playmode(game->intro.music3, ALLEGRO_PLAYMODE_ONCE);

	game->intro.sample4 = al_load_sample( GetDataFilePath("success.flac") );
	PROGRESS;

	game->intro.music4 = al_create_sample_instance(game->intro.sample4);
	al_attach_sample_instance_to_mixer(game->intro.music4, game->audio.music);
	//al_set_sample_instance_gain(game->intro.music, 0.75);
	al_set_sample_instance_playmode(game->intro.music4, ALLEGRO_PLAYMODE_ONCE);

	game->intro.sample5 = al_load_sample( GetDataFilePath("fail.flac") );
	PROGRESS;

	game->intro.music5 = al_create_sample_instance(game->intro.sample5);
	al_attach_sample_instance_to_mixer(game->intro.music5, game->audio.music);
	//al_set_sample_instance_gain(game->intro.music, 0.75);
	al_set_sample_instance_playmode(game->intro.music5, ALLEGRO_PLAYMODE_ONCE);

	game->intro.samplewin = al_load_sample( GetDataFilePath("win.flac") );
	PROGRESS;

	game->intro.musicwin = al_create_sample_instance(game->intro.samplewin);
	al_attach_sample_instance_to_mixer(game->intro.musicwin, game->audio.music);
	//al_set_sample_instance_gain(game->intro.music, 0.75);
	al_set_sample_instance_playmode(game->intro.musicwin, ALLEGRO_PLAYMODE_ONCE);

	game->intro.samplelost = al_load_sample( GetDataFilePath("lost.flac") );
	PROGRESS;

	game->intro.musiclost = al_create_sample_instance(game->intro.samplelost);
	al_attach_sample_instance_to_mixer(game->intro.musiclost, game->audio.music);
	al_set_sample_instance_gain(game->intro.musiclost, 0.75);
	al_set_sample_instance_playmode(game->intro.musiclost, ALLEGRO_PLAYMODE_ONCE);

	PROGRESS;
	game->intro.space =LoadScaledBitmap("space.jpg", game->viewportWidth, game->viewportHeight);
	PROGRESS;
	game->intro.cadence = LoadScaledBitmap("cadence.png", game->viewportWidth*0.1, game->viewportWidth*0.1 );
	PROGRESS;

	if ((!game->intro.sample) || (!game->intro.sample2) || (!game->intro.sample3) || (!game->intro.sample4) || (!game->intro.sample5)) {
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	PROGRESS;


}

void Intro_Unload(struct Game *game) {
	//FadeGameState(game, false);
	//al_pause_video(video, true);
	al_close_video(videos[0]);
	al_close_video(videos[1]);
	al_close_video(videos[2]);
	al_close_video(videos[3]);
	al_close_video(videos[4]);
	//video=NULL;
}
