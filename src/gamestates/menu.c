/*! \file menu.c
 *  \brief Main Menu view.
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
#include <stdio.h>
#include <math.h>
#include "../config.h"
#include "menu.h"

void DrawMenuState(struct Game *game) {
	ALLEGRO_FONT *font;
	char* text = malloc(255*sizeof(char));
	char* text2 = malloc(255*sizeof(char));
	struct ALLEGRO_COLOR color;
	switch (game->menu.menustate) {
		case MENUSTATE_GAME:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.4, ALLEGRO_ALIGN_CENTRE, "Easy");
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, "Medium");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, "Hard");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.7, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_OPTIONS:
			if (game->menu.options.fullscreen) {
				sprintf(text, "Fullscreen: yes");
			}
			else {
				sprintf(text, "Fullscreen: no");
			}
			if (game->menu.options.audio) {
				sprintf(text2, "Music: yes");
			}
			else {
				sprintf(text2, "Music: no");
			}
			if (game->menu.options.fullscreen!=game->fullscreen) {
				color = al_map_rgba(255,255,255,255);
			} else {
				color = al_map_rgba(0,0,0,128);
			}
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.4, ALLEGRO_ALIGN_CENTRE, text2);
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, text);
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, color, game->viewportWidth*0.5, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, "Restart game");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.7, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_PAUSE:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.4, ALLEGRO_ALIGN_CENTRE, "Start game");
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, "Options");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, "About");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.7, ALLEGRO_ALIGN_CENTRE, "Exit");
			break;
		default:
			game->menu.selected=0;
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.4, ALLEGRO_ALIGN_CENTRE, "Not implemented yet");
			break;
	}
	free(text);
	free(text2);
}

void Menu_Draw(struct Game *game) {
	al_draw_bitmap(game->loading.image4,0,0,0);
	if (!game->menu.loaded) {
		game->gamestate=GAMESTATE_LOADING;
		game->loadstate=GAMESTATE_MENU;
		return;
	}


	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.1, ALLEGRO_ALIGN_CENTRE, "Cadence Throw");
	al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.275, ALLEGRO_ALIGN_CENTRE, "Throwing wifes is Magic!");

	DrawMenuState(game);
}

void Menu_Logic(struct Game *game) {
}

void Menu_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
	PROGRESS_INIT(5);

	game->menu.options.fullscreen = game->fullscreen;
	game->menu.options.audio = game->music;
	game->menu.loaded = true;

	//game->menu.sample = al_load_sample( GetDataFilePath("menu/menu.flac") );
	game->loading.image1 = LoadScaledBitmap("loading1.png", game->viewportWidth, game->viewportHeight);
	PROGRESS;
	//game->menu.rain_sample = al_load_sample( GetDataFilePath("menu/rain.flac") );
	game->loading.image1 = LoadScaledBitmap("loading1.png", game->viewportWidth, game->viewportHeight);
	game->loading.image1 = LoadScaledBitmap("loading1.png", game->viewportWidth, game->viewportHeight);
	game->loading.image1 = LoadScaledBitmap("loading1.png", game->viewportWidth, game->viewportHeight);
	PROGRESS;
	game->menu.click_sample = al_load_sample( GetDataFilePath("menu/click.flac") );
	game->loading.image1 = LoadScaledBitmap("loading1.png", game->viewportWidth, game->viewportHeight);
	PROGRESS;

	/*game->menu.music = al_create_sample_instance(game->menu.sample);
	al_attach_sample_instance_to_mixer(game->menu.music, game->audio.music);
	al_set_sample_instance_playmode(game->menu.music, ALLEGRO_PLAYMODE_LOOP);

	game->menu.rain_sound = al_create_sample_instance(game->menu.rain_sample);
	al_attach_sample_instance_to_mixer(game->menu.rain_sound, game->audio.fx);
	al_set_sample_instance_playmode(game->menu.rain_sound, ALLEGRO_PLAYMODE_LOOP);*/

	game->menu.click = al_create_sample_instance(game->menu.click_sample);
	al_attach_sample_instance_to_mixer(game->menu.click, game->audio.fx);
	al_set_sample_instance_playmode(game->menu.click, ALLEGRO_PLAYMODE_ONCE);

	game->menu.font_title = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewportHeight*0.16,0 );
	game->menu.font_subtitle = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewportHeight*0.08,0 );
	game->menu.font = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewportHeight*0.05,0 );
	game->menu.font_selected = al_load_ttf_font(GetDataFilePath("fonts/ShadowsIntoLight.ttf"),game->viewportHeight*0.065,0 );
	PROGRESS;

/*	if (!game->menu.sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}

	if (!game->menu.rain_sample){
		fprintf(stderr, "Audio clip sample#2 not loaded!\n" );
		exit(-1);
	}*/

	if (!game->menu.click_sample){
		fprintf(stderr, "Audio clip sample#3 not loaded!\n" );
		exit(-1);
	}
	PROGRESS;
}

void Menu_Stop(struct Game* game) {
	//FadeGameState(game, false);
}

void ChangeMenuState(struct Game *game, enum menustate_enum state) {
	game->menu.menustate=state;
	game->menu.selected=0;
	PrintConsole(game, "menu state changed %d", state);
}

void Menu_Unload(struct Game *game) {
	if (!game->menu.loaded) return;
	if (game->gamestate==GAMESTATE_MENU) Menu_Stop(game);
	al_destroy_font(game->menu.font_title);
	al_destroy_font(game->menu.font_subtitle);
	al_destroy_font(game->menu.font);
	al_destroy_font(game->menu.font_selected);
//	al_destroy_sample_instance(game->menu.music);
//	al_destroy_sample_instance(game->menu.rain_sound);
	al_destroy_sample_instance(game->menu.click);
	//al_destroy_sample(game->menu.sample);
	//al_destroy_sample(game->menu.rain_sample);
	al_destroy_sample(game->menu.click_sample);
	game->menu.loaded = false;
}

void Menu_Load(struct Game *game) {
	//if (!game->menu.loaded) return;
	ChangeMenuState(game,MENUSTATE_PAUSE);
}

int Menu_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {

	if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		game->menu.selected--;
		if ((game->menu.menustate==MENUSTATE_OPTIONS) && (game->menu.selected==2) && game->menu.options.fullscreen==game->fullscreen) game->menu.selected--;
		al_play_sample_instance(game->menu.click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		game->menu.selected++;
		if ((game->menu.menustate==MENUSTATE_OPTIONS) && (game->menu.selected==2) && game->menu.options.fullscreen==game->fullscreen) game->menu.selected++;
		al_play_sample_instance(game->menu.click);
	}

	if (ev->keyboard.keycode==ALLEGRO_KEY_ENTER) {
		al_play_sample_instance(game->menu.click);
		switch (game->menu.menustate) {
			case MENUSTATE_GAME:
				switch (game->menu.selected) {
					case 0:
						PrintConsole(game,"Game resumed.");
						//al_destroy_bitmap(game->pause.bitmap);
						//game->pause.bitmap = NULL;
						ResumeGameState(game);
						game->gamestate = game->loadstate;
						game->level.range = 20;
						break;
					case 1:
						PrintConsole(game,"Game resumed.");
						//al_destroy_bitmap(game->pause.bitmap);
						//game->pause.bitmap = NULL;
						ResumeGameState(game);
						game->gamestate = game->loadstate;
						game->level.range = 12;
						break;
					case 2:
						PrintConsole(game,"Game resumed.");
						//al_destroy_bitmap(game->pause.bitmap);
						//game->pause.bitmap = NULL;
						ResumeGameState(game);
						game->gamestate = game->loadstate;
						game->level.range = 6;
						break;
					case 3:
						ChangeMenuState(game,MENUSTATE_PAUSE);
						break;
				};
				break;
			case MENUSTATE_OPTIONS:
				switch (game->menu.selected) {
					case 0:
						game->menu.options.audio = !game->menu.options.audio;
						if (game->menu.options.audio) {
							SetConfigOption("SuperDerpy", "music", "10");
							al_set_mixer_gain(game->audio.mixer, 1);
						}
						else {
							SetConfigOption("SuperDerpy", "music", "0");
							al_set_mixer_gain(game->audio.mixer, 0);
						}
						break;
					case 1:
						game->menu.options.fullscreen = !game->menu.options.fullscreen;
						if (game->menu.options.fullscreen)
							SetConfigOption("SuperDerpy", "fullscreen", "1");
						else
							SetConfigOption("SuperDerpy", "fullscreen", "0");
						break;
					case 2:
						game->restart=1;
						return 1;
						break;
					case 3:
						ChangeMenuState(game,MENUSTATE_PAUSE);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_PAUSE:
				switch (game->menu.selected){
					case 0:
						ChangeMenuState(game,MENUSTATE_GAME);
						game->menu.selected = 1;
						break;
					case 1:
						ChangeMenuState(game,MENUSTATE_OPTIONS);
						break;
					case 2:
						//game->gamestate=game->loadstate;
						//UnloadGameState(game);
						game->gamestate = GAMESTATE_ABOUT;
						//game->loadstate = GAMESTATE_ABOUT;
						break;
					case 3:
						return 1;
					default:
						break;
				}
				break;
			default:
				return 1;
				break;
		}
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		switch (game->menu.menustate) {
			case MENUSTATE_OPTIONS:
				ChangeMenuState(game,MENUSTATE_PAUSE);
				break;
		case MENUSTATE_GAME:
				ChangeMenuState(game,MENUSTATE_PAUSE);
				break;
			case MENUSTATE_PAUSE:
				//PrintConsole(game,"Game resumed.");
				//al_destroy_bitmap(game->pause.bitmap);
				//game->pause.bitmap = NULL;
				return 1;
				ResumeGameState(game);
				game->gamestate = game->loadstate;
				break;
			default:
				return 1;
				break;
		}
	}

	if (game->menu.selected==-1) game->menu.selected=3;
	if (game->menu.selected==4) game->menu.selected=0;
	return 0;
}
