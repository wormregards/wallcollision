#include "../header/controls.h"
#include <math.h>

float speed = 400;

void handleMovement(Rectangle *player, float deltaTime) {
	if (getControls().left) {
		player->x -= speed * deltaTime;
	}

	if (getControls().right) {
		player->x += speed * deltaTime;
	}

	if (getControls().up) {
		player->y -= speed * deltaTime;
	}

	if (getControls().down) {
		player->y += speed * deltaTime;
	}
}

void ResolveCollision(Rectangle* player, Rectangle* wall) {
	// Calculate the intersection rectangle
	float overlapX = fminf(player->x + player->width, wall->x + wall->width) - fmaxf(player->x, wall->x);
	float overlapY = fminf(player->y + player->height, wall->y + wall->height) - fmaxf(player->y, wall->y);

	// Determine which direction has the smallest overlap
	if (overlapX < overlapY) {
		// Resolve collision along the X axis
		if (player->x < wall->x) {
			// Player is to the left of the wall
			player->x = wall->x - player->width;
		}
		else {
			// Player is to the right of the wall
			player->x = wall->x + wall->width;
		}
	}
	else {
		// Resolve collision along the Y axis
		if (player->y < wall->y) {
			// Player is above the wall
			player->y = wall->y - player->height;
		}
		else {
			// Player is below the wall
			player->y = wall->y + wall->height;
		}
	}
}

int main() {
	int screenWidth = 800;
	int screenHeight = 700;
	int targetFPS = 60;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Wall Collision");
	SetTargetFPS(targetFPS);
	
	Rectangle player = { 0, 0, 30, 30 };
	Rectangle box = { 100, 100, 100, 100 };
	Rectangle box2 = { 400, 100, 50, 350 };

	Camera2D camera = { 0 };
	camera.zoom = 1.f;
	camera.target = (Vector2){player.x, player.y};
	camera.offset = (Vector2){ GetScreenWidth() / 2, GetScreenHeight() / 2 };

	float deltaTime = GetFrameTime();
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground((Color) { 0x17, 0x17, 0x17, 0xFF });
		deltaTime = GetFrameTime();
		if (GetScreenWidth() != screenWidth || GetScreenHeight() != screenHeight) {
			screenWidth = GetScreenWidth();
			screenHeight = GetScreenHeight();

			camera.offset = (Vector2){ GetScreenWidth() / 2, GetScreenHeight() / 2 };
		}
		
		handleMovement(&player, deltaTime);
		

		if (CheckCollisionRecs(player, box)) {
			ResolveCollision(&player, &box);
		}

		if (CheckCollisionRecs(player, box2)) {
			ResolveCollision(&player, &box2);
		}
		camera.target = (Vector2){ player.x, player.y };
		BeginMode2D(camera);
		DrawRectangleRec(box, BLUE);
		DrawRectangleRec(box2, RED);
		DrawRectangleRec(player, WHITE);
		EndMode2D();
		EndDrawing();
	}


	CloseWindow();
	return 0;
}

