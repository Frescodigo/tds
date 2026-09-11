#include <raylib.h>

#define TARGET_FPS 60

typedef struct {
	Texture2D texture;
	Vector2 position;
	Vector2 delta;
	float speed;
} Thing;

void DrawThing(Thing *thing) {
	DrawTextureV(thing->texture, thing->position, WHITE);
}

Vector2 AddVectors(Vector2 a, Vector2 b) {
	Vector2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}

Vector2 ScaleVector(Vector2 a, float s) {
	Vector2 result;
	result.x = a.x * s;
	result.y = a.y * s;
	return result;
}

void gameLoop() {
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Top-down Shooter");
	SetTargetFPS(TARGET_FPS);

	Texture2D heroTexture = LoadTexture("resources/hero.png");
	Texture2D enemyTexture = LoadTexture("resources/enemy.png");

	Thing player = {0};
	player.position = (Vector2) { 40.0f, 80.0f };
	player.delta = (Vector2) { 0.0f, 0.0f };
	player.texture = heroTexture;
	player.speed = 5.0f;

	float deltaTime = 0.0f;

	while (!WindowShouldClose()) {

		deltaTime = GetFrameTime();

		player.delta = ScaleVector(player.delta, 0.8);

		if (IsKeyDown(KEY_D)) player.delta.x += player.speed * deltaTime;
		if (IsKeyDown(KEY_A)) player.delta.x -= player.speed * deltaTime;
		if (IsKeyDown(KEY_W)) player.delta.y -= player.speed * deltaTime;
		if (IsKeyDown(KEY_S)) player.delta.y += player.speed * deltaTime;
		
		player.position = AddVectors(player.position, player.delta);

		BeginDrawing();

		DrawFPS(20, 20);

		DrawThing(&player);

		ClearBackground(DARKGRAY);

		EndDrawing();
	}

	CloseWindow();
}

