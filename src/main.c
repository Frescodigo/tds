/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define MAX_ENTITIES 2048
#define TARGET_FPS 60
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef struct Entity {
    float dx;
    float dy;
    float speed;
    int x;
    int y;
    Texture tex;
} Entity;

Entity entities[MAX_ENTITIES] = {0};
int entity_count = 0;

int add_entity(Texture tex) {
    if (entity_count < MAX_ENTITIES) {
        Entity* e = &entities[entity_count++];
        e->tex = tex;
        return entity_count - 1;
    }

    exit(1);
    return 0;
}

void delete_entity(int index) {
    if (index = 0) {
        printf("tried to remove the player_idx index dummy\n");
        return;
    }

    printf("trying to delete %d\n", index);
    entities[index] = entities[entity_count - 1];
    entity_count--;
}

void draw_entity(Entity* e) {
    DrawTexture(e->tex, e->x, e->y, WHITE);
}

void update_player();

int player_idx;

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    SetTargetFPS(TARGET_FPS);

	// Create the window and OpenGL context
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Top Down Shooter");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture hero_tex = LoadTexture("hero.png");
	Texture enemy_tex = LoadTexture("enemy.png");
    Texture bullet_tex = LoadTexture("bullet.png");

    player_idx = add_entity(hero_tex);

    entities[player_idx].x = 200;
    entities[player_idx].y = 400;
    entities[player_idx].speed = 5.0f;
    entities[player_idx].tex = hero_tex;
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{

        // update
        update_player();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int new_bullet_idx = add_entity(bullet_tex);
            Entity* new_bullet = &entities[new_bullet_idx];
            new_bullet->tex = bullet_tex;
            new_bullet->x = entities[player_idx].x;
            new_bullet->y = entities[player_idx].y;
            new_bullet->speed = 9.0f;
        }

        int i = 1;
        while (i < entity_count) {
            entities[i].x += entities[i].speed;
            if (entities[i].x > WINDOW_WIDTH) {
                printf("what the fuck is `i` here: %d\n");
                delete_entity(i);
                continue;
            }
            i++;
        }

        printf("entity_count: %d\n", entity_count);

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(DARKBLUE);

		// draw our texture to the screen
        for (int i = 0; i < entity_count; i++) {
            draw_entity(&entities[i]);
        }
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(hero_tex);
	UnloadTexture(enemy_tex);
	UnloadTexture(bullet_tex);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

void update_player() {
        int is_moving_up_or_down = 0;
        int is_moving_left_or_right = 0;

        Entity* player = &entities[player_idx];

        player->dx = 0.0f;
        player->dy = 0.0f;
        
        float adjusted_speed = player->speed * GetFrameTime() * TARGET_FPS;

        if (IsKeyDown(KEY_W)) {
            player->dy -= adjusted_speed;
            is_moving_up_or_down = 1;
        }
        if (IsKeyDown(KEY_S)) {
            player->dy += adjusted_speed;
            is_moving_up_or_down = 1;
        }
        if (IsKeyDown(KEY_A)) {
            player->dx -= adjusted_speed;
            is_moving_left_or_right = 1;
        }
        if (IsKeyDown(KEY_D)) {
            player->dx += adjusted_speed;
            is_moving_left_or_right = 1;
        }

        if (is_moving_left_or_right && is_moving_up_or_down) {
            player->dx *= 0.7f;
            player->dy *= 0.7f;
        }

        player->x += entities[player_idx].dx;
        player->y += entities[player_idx].dy;

}
