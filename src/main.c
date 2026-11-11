/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define MAX_ENTITIES 2048
#define TARGET_FPS 60
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

enum kinds {
    Hero,
    Enemy,
    Bullet,
    KindsCount
};

typedef struct Entity {
    float dx;
    float dy;
    float speed;
    int x;
    int y;
    int width;
    int height;
    enum kinds kind;
} Entity;

Entity entities[MAX_ENTITIES] = {0};
int entity_count = 0;
Texture textures[KindsCount];
float enemy_spawn_rate;
float enemy_spawn_countdown;
int score;
Entity* player;

int add_entity(enum kinds kind) {
    if (entity_count < MAX_ENTITIES) {
        Entity* e = &entities[entity_count++];
        e->dx = 0.0f;
        e->dy = 0.0f;
        e->speed = 0.0f;
        e->x = 0;
        e->y = 0;
        e->width = 0;
        e->height = 0;
        e->kind = kind;
        return entity_count - 1;
    }

    exit(1);
    return 0;
}

void delete_entity(int index) {
    if (index == 0) {
        printf("tried to remove the player_idx index dummy\n");
        return;
    }

    entities[index] = entities[--entity_count];
}

void draw_entity(Entity* e) {
    DrawTexture(textures[e->kind], e->x, e->y, WHITE);
}

int are_entities_colliding(Entity* e1, Entity* e2) {
    if (e1->x + e1->width < e2->x || e1->y + e1->height < e2->y ||
            e1->x > e2->x + e2->width || e1->y > e2->y + e2->height) {
        return 0;
    }

    return 1;
}

Vector2 direction_vector(int x1, int y1, int x2, int y2) {
    return (Vector2) { x2-x1, y2-y1 };
}

void update_player();
void update_enemy(int i);
void update_bullet(int i);



void init_game() {
    entity_count = 0;
    player = &entities[add_entity(Hero)];

    // player is the first entity
    player->x = 200;
    player->y = 400;
    player->speed = 5.0f;
    player->width = 64;
    player->height = 64;

    score = 0;

    enemy_spawn_rate = 1.8f;
    enemy_spawn_countdown = 0.0f;
}

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
    // TODO: check for errors?
	textures[Hero] = LoadTexture("hero.png");
	textures[Enemy] = LoadTexture("enemy.png");
    textures[Bullet] = LoadTexture("bullet.png");

    init_game();

    // setup stuff for enemy spawner

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
        // enemy spawner
        if (enemy_spawn_countdown <= 0.0f) {
                printf("spawnrate: %f\n", enemy_spawn_rate);
            enemy_spawn_countdown = enemy_spawn_rate;
            if (enemy_spawn_rate > 0.4f) {
                enemy_spawn_rate -= 0.05;
            }
            Entity* new_enemy = &entities[add_entity(Enemy)];
            new_enemy->width = 64;
            new_enemy->height = 64;
            new_enemy->speed = 3.0f;
            new_enemy->x = GetRandomValue(10, WINDOW_WIDTH - new_enemy->width - 10);
            new_enemy->y = GetRandomValue(10, WINDOW_HEIGHT - new_enemy->height -10);
        } else {
            enemy_spawn_countdown -= GetFrameTime();
        }

        for (int i = 0; i < entity_count; i++) {
            Entity* e = &entities[i];
            float adjusted_speed = e->speed * GetFrameTime() * TARGET_FPS;

            switch (e->kind) {
            case Hero:
                update_player();
                break;
            case Enemy:
                Vector2 delta = direction_vector(e->x, e->y, player->x, player->y);
                double dx = delta.x;
                double dy = delta.y;
                double mag = sqrt(dx*dx + dy*dy);
                dx /= mag;
                dy /= mag;
                e->x += dx * adjusted_speed;
                e->y += dy * adjusted_speed;
                break;
            case Bullet:
                e->x += e->dx * adjusted_speed;
                e->y += e->dy * adjusted_speed;
                if (e->x < 0 || e->x > WINDOW_WIDTH || e->y < 0 || e->y > WINDOW_HEIGHT) {
                    delete_entity(i);
                    i--;
                    continue;
                }

                // check if bullet hits an enemy
                for (int j = 0; j < entity_count; j++) {
                    if (entities[j].kind == Enemy && are_entities_colliding(e, &entities[j])) {
                        score += 100;

                        delete_entity(i);
                        i--;
                        delete_entity(j);
                        break;
                    }
                }
                break;
            }
        }
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(DARKBLUE);

		// draw our texture to the screen
        for (int i = 0; i < entity_count; i++) {
            draw_entity(&entities[i]);
        }

        // debug info
        char entity_count_msg[32] = { 0 };
        sprintf(entity_count_msg, "entity count: %d", entity_count);
        DrawText(entity_count_msg, 10, 10, 16, BLACK);

        // score info
        char score_msg[32] = { };
        sprintf(score_msg, "Score: %d:", score);
        DrawText(score_msg, 200, 10, 48, BLACK);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
    for (int i = 0; i < KindsCount; i++) {
        UnloadTexture(textures[i]);
    }

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

void update_player() {
        int is_moving_up_or_down = 0;
        int is_moving_left_or_right = 0;

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
        
        player->x += player->dx;
        player->y += player->dy;

        for (int i = 0; i < entity_count; i++) {
            if (entities[i].kind == Enemy && are_entities_colliding(player, &entities[i])) {
                init_game();
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int new_bullet_idx = add_entity(Bullet);
            Entity* new_bullet = &entities[new_bullet_idx];
            new_bullet->speed = 9.0f;

            new_bullet->width = 16;
            new_bullet->height = 16;

            Vector2 delta = direction_vector(player->x, player->y, GetMouseX(), GetMouseY());

            float dx = delta.x;
            float dy = delta.y;

            float mag = sqrt(dx*dx + dy*dy);
            dx /= mag;
            dy /= mag;

            new_bullet->x = player->x + player->width / 2 - new_bullet->width/2 + dx * 0.8f * player->width;
            new_bullet->y = player->y + player->height / 2 - new_bullet->height/2 + dy * 0.8f * player->height;

            new_bullet->dx = dx;
            new_bullet->dy = dy;
        }

}

