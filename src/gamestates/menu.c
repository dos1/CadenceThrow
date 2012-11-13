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
	struct ALLEGRO_COLOR color;
	switch (game->menu.menustate) {
		case MENUSTATE_MAIN:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, "Start game");
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, "Options");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.7, ALLEGRO_ALIGN_CENTRE, "About");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.8, ALLEGRO_ALIGN_CENTRE, "Exit");
			break;
		case MENUSTATE_OPTIONS:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, "Control settings");
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, "Video settings");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.7, ALLEGRO_ALIGN_CENTRE, "Audio settings");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_AUDIO:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			if (game->music) sprintf(text, "Music volume: %d0%%", game->music);
			else sprintf(text, "Music disabled");
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, text);
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			if (game->fx) sprintf(text, "Effects volume: %d0%%", game->fx);
			else sprintf(text, "Effects disabled");
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, text);
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			if (game->voice) sprintf(text, "Voice volume: %d0%%", game->voice);
			else sprintf(text, "Voice disabled");
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.7, ALLEGRO_ALIGN_CENTRE, text);
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_VIDEO:
			if (game->menu.options.fullscreen) {
				sprintf(text, "Fullscreen: yes");
				color = al_map_rgba(0,0,0,128);
			}
			else {
				sprintf(text, "Fullscreen: no");
				color = al_map_rgba(255,255,255,255);
			}
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, text);
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, color, game->viewportWidth*0.5, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, "Resolution: 800x500");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.7, ALLEGRO_ALIGN_CENTRE, "FPS: 60");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_PAUSE:
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, "Resume game");
			font = game->menu.font; if (game->menu.selected==1) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.6, ALLEGRO_ALIGN_CENTRE, "Return to map");
			font = game->menu.font; if (game->menu.selected==2) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.7, ALLEGRO_ALIGN_CENTRE, "Options");
			font = game->menu.font; if (game->menu.selected==3) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.8, ALLEGRO_ALIGN_CENTRE, "Exit");
			break;
		default:
			game->menu.selected=0;
			font = game->menu.font; if (game->menu.selected==0) font = game->menu.font_selected;
			al_draw_text_with_shadow(font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.5, ALLEGRO_ALIGN_CENTRE, "Not implemented yet");
			break;
	}
	free(text);
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
	PROGRESS_INIT(6);

	game->menu.options.fullscreen = game->fullscreen;
	game->menu.options.fps = game->fps;
	game->menu.options.width = game->width;
	game->menu.options.height = game->height;
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
	ChangeMenuState(game,MENUSTATE_MAIN);
}

int Menu_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {

	if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		game->menu.selected--;
		if ((game->menu.menustate==MENUSTATE_VIDEO) && (game->menu.selected==1) && (game->menu.options.fullscreen)) game->menu.selected--;
		al_play_sample_instance(game->menu.click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		game->menu.selected++;
		if ((game->menu.menustate==MENUSTATE_VIDEO) && (game->menu.selected==1) && (game->menu.options.fullscreen)) game->menu.selected++;
		al_play_sample_instance(game->menu.click);
	}

	if (ev->keyboard.keycode==ALLEGRO_KEY_ENTER) {
		char *text;
		al_play_sample_instance(game->menu.click);
		switch (game->menu.menustate) {
			case MENUSTATE_MAIN:
				switch (game->menu.selected) {
					case 0:
						UnloadGameState(game);
						game->gamestate = GAMESTATE_LOADING;
						game->loadstate = GAMESTATE_INTRO;
						break;
					case 1:
						ChangeMenuState(game,MENUSTATE_OPTIONS);
						break;
					case 2:
						UnloadGameState(game);
						game->gamestate = GAMESTATE_LOADING;
						game->loadstate = GAMESTATE_ABOUT;
						break;
					case 3:
						return 1;
						break;
				}
				break;
			case MENUSTATE_AUDIO:
				text = malloc(255*sizeof(char));
				switch (game->menu.selected) {
					case 0:
						game->music--;
						if (game->music<0) game->music=10;
						sprintf(text, "%d", game->music);
						SetConfigOption("SuperDerpy", "music", text);
						al_set_mixer_gain(game->audio.music, game->music/10.0);
						break;
					case 1:
						game->fx--;
						if (game->fx<0) game->fx=10;
						sprintf(text, "%d", game->fx);
						SetConfigOption("SuperDerpy", "fx", text);
						al_set_mixer_gain(game->audio.fx, game->fx/10.0);
						break;
					case 2:
						game->voice--;
						if (game->voice<0) game->voice=10;
						sprintf(text, "%d", game->voice);
						SetConfigOption("SuperDerpy", "voice", text);
						al_set_mixer_gain(game->audio.voice, game->voice/10.0);
						break;
					case 3:
						ChangeMenuState(game,MENUSTATE_OPTIONS);
						break;
				}
				free(text);
				break;
			case MENUSTATE_OPTIONS:
				switch (game->menu.selected) {
					case 0:
						ChangeMenuState(game,MENUSTATE_CONTROLS);
						break;
					case 1:
						ChangeMenuState(game,MENUSTATE_VIDEO);
						break;
					case 2:
						ChangeMenuState(game,MENUSTATE_AUDIO);
						break;
					case 3:
						ChangeMenuState(game,MENUSTATE_MAIN);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_PAUSE:
				switch (game->menu.selected){
					case 0:
						PrintConsole(game,"Game resumed.");
						al_destroy_bitmap(game->pause.bitmap);
						game->pause.bitmap = NULL;
						ResumeGameState(game);
						game->gamestate = game->loadstate;
						break;
					case 1:
						game->gamestate=game->loadstate;
						UnloadGameState(game);
						game->gamestate = GAMESTATE_LOADING;
						game->loadstate = GAMESTATE_MAP;
						break;
					case 2:
						ChangeMenuState(game,MENUSTATE_OPTIONS);
						break;
					case 3:
						return 1;
					default:
						break;
				}
				break;
			case MENUSTATE_CONTROLS:
				switch (game->menu.selected) {
					case 3:
						ChangeMenuState(game,MENUSTATE_OPTIONS);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_VIDEO:
				switch (game->menu.selected) {
					case 0:
						game->menu.options.fullscreen = !game->menu.options.fullscreen;
						if (game->menu.options.fullscreen)
							SetConfigOption("SuperDerpy", "fullscreen", "1");
						else
							SetConfigOption("SuperDerpy", "fullscreen", "0");
						break;
					case 3:
						if ((game->menu.options.fullscreen==game->fullscreen) && (game->menu.options.fps==game->fps) && (game->menu.options.width==game->width) && (game->menu.options.height==game->height)) {
							ChangeMenuState(game,MENUSTATE_OPTIONS);
						} else {
							PrintConsole(game, "video settings changed, restarting...");
							game->restart = true;
							return 1;
						}
						break;
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
				ChangeMenuState(game,MENUSTATE_MAIN);
				break;
			case MENUSTATE_VIDEO:
				ChangeMenuState(game,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_AUDIO:
				ChangeMenuState(game,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_CONTROLS:
				ChangeMenuState(game,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_PAUSE:
				PrintConsole(game,"Game resumed.");
				al_destroy_bitmap(game->pause.bitmap);
				game->pause.bitmap = NULL;
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
