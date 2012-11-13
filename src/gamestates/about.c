/*! \file about.c
 *  \brief About screen.
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
#include "level.h"
#include "about.h"

void About_Logic(struct Game *game) {
	if (al_get_sample_instance_position(game->about.music)<700000) { return; }
	if (game->about.fadeloop>=0) {
		if (game->about.fadeloop==0) PrintConsole(game, "Fade in");
		game->about.fadeloop+=5;
		if (game->about.fadeloop>=256) {
			al_destroy_bitmap(game->about.fade_bitmap);
			game->about.fadeloop=-1;
		}
		return;
	}
	game->about.x+=0.00025;
}

void About_Draw(struct Game *game) {
	Level_Draw(game);

	al_draw_filled_rectangle(0, 0, game->viewportWidth, game->viewportHeight, al_map_rgba_f(0,0,0,0.75));

	al_draw_text_with_shadow(game->menu.font_title, al_map_rgb(255,255,255), game->viewportWidth*0.05, game->viewportHeight*0.05, ALLEGRO_ALIGN_LEFT, "Cadence Throw");

	al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewportWidth*0.05, game->viewportHeight*0.25, ALLEGRO_ALIGN_LEFT, "Silly MLP minigame based on");

		al_draw_text_with_shadow(game->menu.font_subtitle, al_map_rgb(255,255,255), game->viewportWidth*0.05, game->viewportHeight*0.35, ALLEGRO_ALIGN_LEFT, "Super Derpy engine.");

		al_draw_text_with_shadow(game->menu.font, al_map_rgb(255,255,255), game->viewportWidth*0.95, game->viewportHeight*0.375, ALLEGRO_ALIGN_RIGHT, "by Sebastian Krzyszkowiak");

	al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), game->viewportWidth*0.95, game->viewportHeight*0.5, ALLEGRO_ALIGN_RIGHT, "...speaking of which - have you seen");
	al_draw_text_with_shadow(game->menu.font_selected, al_map_rgb(255,255,255), game->viewportWidth*0.95, game->viewportHeight*0.6, ALLEGRO_ALIGN_RIGHT, "Super Derpy: Muffin Attack project?");
	al_draw_text_with_shadow(game->menu.font, al_map_rgb(255,255,255), game->viewportWidth*0.95, game->viewportHeight*0.75, ALLEGRO_ALIGN_RIGHT, "http://www.superderpy.com/");

	al_draw_text_with_shadow(game->menu.font, al_map_rgb(255,255,255), game->viewportWidth*0.5, game->viewportHeight*0.9, ALLEGRO_ALIGN_CENTER, "Press Escape to return");

}

void About_Load(struct Game *game) {}

int About_Keydown(struct Game *game, ALLEGRO_EVENT *ev) {
	if (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		game->gamestate = GAMESTATE_PAUSE;
		game->loadstate = GAMESTATE_LEVEL;
	}
	return 0;
}

void About_Preload(struct Game *game, void (*progress)(struct Game*, float)) {
}

void About_Unload(struct Game *game) {

}
