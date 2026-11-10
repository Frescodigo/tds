/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
#include <stdlib.h>

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define MAX_ENTITIES 2048

typedef struct Entity {
    float x;
    float y;
    Texture tex;
} Entity;

Entity entities[MAX_ENTITIES] = {0};
int entity_count = 0;

Entity* add_entity(Texture tex) {
    if (entity_count < MAX_ENTITIES) {
        Entity* e = &entities[entity_count++];
        e->tex = tex;
        return e;
    }

    return NULL;
}

void draw_entity(Entity* e) {
    DrawTexture(e->tex, e->x, e->y, WHITE);
}


Entity* player;

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 600, "Top Down Shooter");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture heroTex = LoadTexture("hero.png");
	Texture enemyTex = LoadTexture("enemy.png");

    player = add_entity(heroTex);

    player->x = 200;
    player->y = 400;
    player->tex = heroTex;
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{

        // update
        // TODO:


		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(DARKBLUE);

		// draw our texture to the screen
		// DrawTexture(wabbit, 400, 200, WHITE);
		// DrawTexture(heroTex, 200, 200, WHITE);
        draw_entity(player);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(heroTex);
	UnloadTexture(enemyTex);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
