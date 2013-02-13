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
#include <math.h>
#include "level.h"
#include "pause.h"

ALLEGRO_VIDEO *videos[5] = {};
int durations[5] = {3, 5, 0, 10, 1};
ALLEGRO_BITMAP *bitmap = NULL;

double oldpos;
int counter = -1;
int power = -1;


bool paused = false;

bool won = false, lost = false;
bool crashed = false;

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
			al_set_sample_instance_playing(game->level.music2,true);
			al_set_sample_instance_position(game->level.music2, 50000);
			break;
		case 2:
			al_set_sample_instance_playing(game->level.music3,true);
			break;
		case 3:
			al_set_sample_instance_playing(game->level.music4,true);
			break;
		case 4:
			al_set_sample_instance_playing(game->level.music5,true);
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

void Level_Logic(struct Game *game) {
	if (CheckPos(cur)) {
		if (cur==0) {
			game->gamestate=GAMESTATE_PAUSE;
			game->loadstate=GAMESTATE_LEVEL;
			PauseGameState(game);
			Pause_Load(game);
			return;
		}
		else if (cur==3) {
			won=true;
			al_set_sample_instance_position(game->level.musicwin, 0);
			al_play_sample_instance(game->level.musicwin);
			cur=255;
		}
		else if (cur==4) {
			lost=true;
			al_set_sample_instance_position(game->level.musiclost, 0);
			al_play_sample_instance(game->level.musiclost);
			game->level.cadencepos = 0;
			cur=255;
		}
		else {
			int inc = 1;
			double p = (0.21+((0.79-0.21)*(power/100.0)));

			if (cur==2) {
				PrintConsole(game, "power status: %d, %f, %f", power, p, 0.7-(game->level.range/100.0));
				if (p>0.7) {
					inc = 2;
				}
			}
			if (cur==1) {
				if (p<0.7-(game->level.range/100.0)) {
					lost=true;
					game->level.cadencepos = 0;
					al_set_sample_instance_position(game->level.musiclost, 0);
					al_set_sample_instance_position(game->level.musiccrash, 0);
				}
			}
			NextVideo(game,inc);
		}
	}
	//PrintConsole(game, "%d", GetCurrentPower());
}

void Level_Pause(struct Game *game) {
	//al_pause_video(videos[cur], true);
	paused = true;
}

void Level_Resume(struct Game *game) {
	//al_seek_video(videos[cur], 3.45);
	//al_pause_video(videos[cur], false);
	won = false;
	lost = false;
	cur = 0;
	power = -1;
	crashed = false;
	al_stop_sample_instance(game->level.musicwin);
	al_stop_sample_instance(game->level.musiclost);
	al_set_sample_instance_playing(game->level.music, false);
	NextVideo(game, 1);
	paused = false;
}

void Level_Draw(struct Game *game) {

	if (won || lost) {

		bool display = !paused;

		if (bitmap) al_draw_scaled_bitmap(bitmap, 0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), 0, 0, game->viewportWidth, game->viewportHeight, 0);
		char* text;
		if (won) text = "You won!";
		if (lost) { text = "You lost!";

			if ((0.21+((0.79-0.21)*(power/100.0))) > 0.7) {
				al_draw_bitmap(game->level.space, 0, 0, 0);
				al_draw_rotated_bitmap(game->level.cadence, 0, 0, game->viewportWidth*(game->level.cadencepos/1000.0), game->viewportHeight*(1-(game->level.cadencepos/1100.0)), game->level.cadencepos/256.0, 0);
			} else {
				al_draw_bitmap_region(game->level.fail,game->viewportWidth*(int)fmod(game->level.cadencepos/5,7),game->viewportHeight*(((int)(game->level.cadencepos/5/7))%2),game->viewportWidth, game->viewportHeight,0,0,0);
				if (game->level.cadencepos/5 >= 13) {
					game->level.cadencepos = 13*5;
					if (!crashed) {
						al_play_sample_instance(game->level.musiclost);
						al_play_sample_instance(game->level.musiccrash);
					}
					crashed = true;
				} else {
					display = false;
				}
			}

			game->level.cadencepos++;
		}
		if (display) {
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

			al_draw_filled_rectangle(game->viewportWidth*(0.7-(game->level.range/100.0)), game->viewportHeight*0.81, game->viewportWidth*0.7, game->viewportHeight*0.89, al_map_rgb(150,255,50));

			al_draw_filled_rectangle(game->viewportWidth*0.21, game->viewportHeight*0.81, game->viewportWidth*(0.21+((0.79-0.21)*(p/100.0))), game->viewportHeight*0.89, al_map_rgb(255,255,150));

			char *pstr = malloc(5*sizeof(char));
			sprintf(pstr, "%d%%", p);
			al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.8, ALLEGRO_ALIGN_CENTRE, pstr);
			free(pstr);

		}
	}

}

void Level_Load(struct Game *game) {
	won = false;
	lost = false;
	cur = 0;
	power = -1;
	crashed = false;
	al_start_video(videos[cur], game->audio.voice);
	al_set_sample_instance_position(game->level.music, 0);
	al_play_sample_instance(game->level.music);
	//FadeGameState(game, true);
	//al_set_audio_stream_playing(game->level.audiostream, true);
}

int Level_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		return 1;
	}
	if (ev->keyboard.keycode == ALLEGRO_KEY_ENTER && (won || lost)) {
		al_close_video(videos[0]);
		al_close_video(videos[1]);
		al_close_video(videos[2]);
		al_close_video(videos[3]);
		al_close_video(videos[4]);
        videos[0] = al_open_video(GetDataFilePath("intro.avi"));
        videos[1] = al_open_video(GetDataFilePath("prepare.avi"));
        videos[2] = al_open_video(GetDataFilePath("throw.avi"));
        videos[3] = al_open_video(GetDataFilePath("success.avi"));
        videos[4] = al_open_video(GetDataFilePath("fail.avi"));
		game->gamestate=GAMESTATE_PAUSE;
		game->loadstate=GAMESTATE_LEVEL;
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

void Level_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	//Menu_Preload(game, progress);
	PROGRESS_INIT(19);
	Pause_Preload(game);
	PROGRESS;
	PrintConsole(game,"Opening video");
    videos[0] = al_open_video(GetDataFilePath("intro.avi"));
	PROGRESS;
    videos[1] = al_open_video(GetDataFilePath("prepare.avi"));
	PROGRESS;
    videos[2] = al_open_video(GetDataFilePath("throw.avi"));
	PROGRESS;
    videos[3] = al_open_video(GetDataFilePath("success.avi"));
	PROGRESS;
    videos[4] = al_open_video(GetDataFilePath("fail.avi"));
	PrintConsole(game,"Opening video DONE");
	PROGRESS;

	game->level.sample = al_load_sample( GetDataFilePath("intro.flac") );
	PROGRESS;

	game->level.music = al_create_sample_instance(game->level.sample);
	al_attach_sample_instance_to_mixer(game->level.music, game->audio.music);
	//al_set_sample_instance_gain(game->level.music, 0.75);
	al_set_sample_instance_playmode(game->level.music, ALLEGRO_PLAYMODE_LOOP);

	game->level.sample2 = al_load_sample( GetDataFilePath("prepare.flac") );
	PROGRESS;

	game->level.music2 = al_create_sample_instance(game->level.sample2);
	al_attach_sample_instance_to_mixer(game->level.music2, game->audio.music);
	//al_set_sample_instance_gain(game->level.music, 0.75);
	al_set_sample_instance_playmode(game->level.music2, ALLEGRO_PLAYMODE_ONCE);

	game->level.sample3 = al_load_sample( GetDataFilePath("throw.flac") );
	PROGRESS;

	game->level.music3 = al_create_sample_instance(game->level.sample3);
	al_attach_sample_instance_to_mixer(game->level.music3, game->audio.music);
	//al_set_sample_instance_gain(game->level.music, 0.75);
	al_set_sample_instance_playmode(game->level.music3, ALLEGRO_PLAYMODE_ONCE);

	game->level.sample4 = al_load_sample( GetDataFilePath("success.flac") );
	PROGRESS;

	game->level.music4 = al_create_sample_instance(game->level.sample4);
	al_attach_sample_instance_to_mixer(game->level.music4, game->audio.music);
	//al_set_sample_instance_gain(game->level.music, 0.75);
	al_set_sample_instance_playmode(game->level.music4, ALLEGRO_PLAYMODE_ONCE);

	game->level.sample5 = al_load_sample( GetDataFilePath("fail.flac") );
	PROGRESS;

	game->level.music5 = al_create_sample_instance(game->level.sample5);
	al_attach_sample_instance_to_mixer(game->level.music5, game->audio.music);
	//al_set_sample_instance_gain(game->level.music, 0.75);
	al_set_sample_instance_playmode(game->level.music5, ALLEGRO_PLAYMODE_ONCE);

	game->level.samplewin = al_load_sample( GetDataFilePath("win.flac") );
	PROGRESS;

	game->level.musicwin = al_create_sample_instance(game->level.samplewin);
	al_attach_sample_instance_to_mixer(game->level.musicwin, game->audio.music);
	//al_set_sample_instance_gain(game->level.music, 0.75);
	al_set_sample_instance_playmode(game->level.musicwin, ALLEGRO_PLAYMODE_ONCE);

	game->level.samplelost = al_load_sample( GetDataFilePath("lost.flac") );
	PROGRESS;

	game->level.musiclost = al_create_sample_instance(game->level.samplelost);
	al_attach_sample_instance_to_mixer(game->level.musiclost, game->audio.music);
	al_set_sample_instance_gain(game->level.musiclost, 0.75);
	al_set_sample_instance_playmode(game->level.musiclost, ALLEGRO_PLAYMODE_ONCE);


	game->level.samplecrash = al_load_sample( GetDataFilePath("crash.flac") );
	PROGRESS;

	game->level.musiccrash = al_create_sample_instance(game->level.samplecrash);
	al_attach_sample_instance_to_mixer(game->level.musiccrash, game->audio.music);
	//al_set_sample_instance_gain(game->level.music, 0.75);
	al_set_sample_instance_playmode(game->level.musiccrash, ALLEGRO_PLAYMODE_ONCE);


	PROGRESS;
	game->level.space =LoadScaledBitmap("space.jpg", game->viewportWidth, game->viewportHeight);
	PROGRESS;
	game->level.cadence = LoadScaledBitmap("cadence.png", game->viewportWidth*0.1, game->viewportWidth*0.1 );
	PROGRESS;

	game->level.fail = LoadScaledBitmap("fail.png", game->viewportWidth*7, game->viewportHeight*2 );
	PROGRESS;

	if ((!game->level.sample) || (!game->level.sample2) || (!game->level.sample3) || (!game->level.sample4) || (!game->level.sample5)) {
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}
	PROGRESS;


}

void Level_Unload(struct Game *game) {
	//FadeGameState(game, false);
	//al_pause_video(video, true);
	al_close_video(videos[0]);
	al_close_video(videos[1]);
	al_close_video(videos[2]);
	al_close_video(videos[3]);
	al_close_video(videos[4]);
	al_destroy_bitmap(game->level.fail);
	al_destroy_bitmap(game->level.cadence);
	al_destroy_bitmap(game->level.space);
	al_destroy_sample_instance(game->level.music);
	al_destroy_sample_instance(game->level.music2);
	al_destroy_sample_instance(game->level.music3);
	al_destroy_sample_instance(game->level.music4);
	al_destroy_sample_instance(game->level.music5);
	al_destroy_sample_instance(game->level.musiccrash);
	al_destroy_sample_instance(game->level.musiclost);
	al_destroy_sample_instance(game->level.musicwin);
	al_destroy_sample(game->level.sample);
	al_destroy_sample(game->level.sample2);
	al_destroy_sample(game->level.sample3);
	al_destroy_sample(game->level.sample4);
	al_destroy_sample(game->level.sample5);
	al_destroy_sample(game->level.samplecrash);
	al_destroy_sample(game->level.samplelost);
	al_destroy_sample(game->level.samplewin);
	//video=NULL;
}
