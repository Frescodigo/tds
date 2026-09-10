#include <raylib.h>

#define TARGET_FPS 60

int main(void) {

  int screenWidth = 800;
  int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Top-down Shooter");
  SetTargetFPS(TARGET_FPS);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(BLUE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
