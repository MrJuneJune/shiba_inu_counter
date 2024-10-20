#include "gameplay/entity.h"
#include "graphics/draw.h"
#include <array>
#include <random>

Entity *createPlayers(int playerCount, float xmax, float ymax, float vmin,
                      float vmax) {
  Entity *players = new Entity[playerCount];

  // Create a random number generator
  std::random_device rd;
  std::mt19937 gen(rd());

  // Ugly random number but whatver but whatever
  std::uniform_real_distribution<> randomX(0, xmax - 83);
  std::uniform_real_distribution<> randomY(0, ymax - 83);
  std::uniform_real_distribution<> randomV(vmin, vmax);
  // looked nicest with 2
  std::uniform_real_distribution<> randomA(vmin, vmax);

  for (int i = 0; i < playerCount; i++) {
    players[i].position->x = randomX(gen);
    players[i].position->y = randomY(gen);
    players[i].position->z = 0;

    players[i].velocity->x = randomV(gen);
    players[i].velocity->y = randomV(gen);
    players[i].velocity->z = 0;

    players[i].acceleration->x = randomA(gen);
    players[i].acceleration->y = randomA(gen);
    players[i].acceleration->z = 0;
  }

  return players;
}

Texture2D createTextureFromImage(const char *filePath) {
  Image image = LoadImage(filePath);

  int originalWidth = image.width;
  int originalHeight = image.height;

  // 250px sqr into 83 ish
  ImageResize(&image, originalWidth / 3, originalHeight / 3);

  Texture2D texture = LoadTextureFromImage(image);

  UnloadImage(image);
  return texture;
}

// clock wise, starting top left
std::array<int, 4>
countNumberOfDogs(const std::array<std::array<int, 4>, 4> &quadrant,
                  Entity *players, const int &playerCount) {
  std::array<int, 4> ans = {0, 0, 0, 0};
  for (int i = 0; i < playerCount; i++) {
    auto &player = players[i];
    for (int j = 0; j < 4; j++) {
      auto &q = quadrant[j];
      if (player.position->x > q[0] && player.position->x < q[2] &&
          player.position->y > q[1] && player.position->y < q[3]) {
        ans[j]++;
        break;
      }
    }
  }
  return ans;
}

int hoveredQuadrant(const std::array<std::array<int, 4>, 4> &quadrant,
                    Vector2 mousePoint) {
  int ans = -1;
  for (int i = 0; i < 4; i++) {
    int x1 = quadrant[i][0]; // Top-left x
    int y1 = quadrant[i][1]; // Top-left y
    int x2 = quadrant[i][2]; // Bottom-right x
    int y2 = quadrant[i][3]; // Bottom-right y

    if (mousePoint.x >= x1 && mousePoint.x <= x2 && mousePoint.y >= y1 &&
        mousePoint.y <= y2) {
      ans = i;
      break;
    }
    if (ans > -1) {
      break;
    }
  }
  return ans;
}

int main() {
  const int playerCount = 50;
  const int maxWidth = 800;
  const int maxHeight = 600;
  const int fontSize = 20;

  std::array<std::array<int, 4>, 4> quadrant = {{
      {0, 0, maxWidth / 2, maxHeight / 2},
      {maxWidth / 2, 0, maxWidth, maxHeight / 2},
      {0, maxHeight / 2, maxWidth / 2, maxHeight},
      {maxWidth / 2, maxHeight / 2, maxWidth, maxHeight},
  }};

  const std::array<Color, 4> colors = {{
      LIGHTGRAY,
      SKYBLUE,
      LIME,
      GREEN,
  }};

  InitWindow(maxWidth, maxHeight, "Shiba inus");
  SetTargetFPS(60);

  const char *filePath = "./assets/dog2.png";
  const char *filePath2 = "./assets/dog3.png";

  Texture2D texture = createTextureFromImage(filePath);
  Texture2D texture2 = createTextureFromImage(filePath2);

  Entity *players = createPlayers(playerCount, maxWidth, maxHeight, -0.5, 0.2);

  // State related?
  int state = 0;
  bool hasPicked = false;
  int picked = -1;
  int countDown = 600;
  std::array<int, 4> numberOfDogs = {0, 0, 0, 0};
  int maxVal = -1;

  while (!WindowShouldClose()) {
    Vector2 mousePoint = GetMousePosition();
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw quadrant first
    for (int i = 0; i < 4; i++) {
      auto &q = quadrant[i];
      DrawRectangle(q[0], q[1], q[2] - q[0], q[3] - q[1], colors[i]);
    }

    // Make sure the user has picked quadrant
    if (!hasPicked && state != 0) {
      auto &q = quadrant[hoveredQuadrant(quadrant, mousePoint)];
      DrawRectangle(q[0], q[1], q[2] - q[0], q[3] - q[1], DARKPURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        picked = hoveredQuadrant(quadrant, mousePoint);
        hasPicked = true;
      }
      DrawText(TextFormat("Pick a quadrant where you think there will be "
                          "most dogs on it"),
               10, 30, fontSize, DARKGRAY);
    } else {
      auto &q = quadrant[picked];
      DrawRectangle(q[0], q[1], q[2] - q[0], q[3] - q[1], DARKPURPLE);
      // Update Players
      if (countDown > 0) {
        for (int i = 0; i < playerCount; i++) {
          players[i].update();
        }
        state++;
        state = state == 40 ? 0 : state;
        numberOfDogs = countNumberOfDogs(quadrant, players, playerCount);
      } else {
        if (maxVal < 0) {
          int temp = 0;
          for (int i = 0; i < numberOfDogs.size(); i++) {
            if (numberOfDogs[i] > maxVal) {
              maxVal = numberOfDogs[i];
              temp = i;
            }
          }
          maxVal = temp;
        }
      }

      // Animations
      // TODO move this to entity state aspect?
      for (int i = 0; i < playerCount; i++) {
        if (state <= 20) {
          drawEntity(players[i], maxWidth, maxHeight, texture);
        } else {
          drawEntity(players[i], maxWidth, maxHeight, texture2);
        }
      }

      if (maxVal < 0) {
        // Show dogs on different quadrant
        for (int i = 0; i < 4; i++) {
          DrawText(TextFormat("Dogs in Quadrant %i: %i", i, numberOfDogs[i]),
                   10, 30 + (fontSize * i), fontSize, BLACK);
        }
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, fontSize, BLACK);
        DrawText(TextFormat("Count Down %i", (countDown + 60) / 60), 300, 40,
                 fontSize + (5 * (600 / (countDown + 20))), BLACK);
        countDown--;

      } else {
        DrawText((maxVal == picked) ? "You won :)" : "You lost :(",
                 maxWidth / 3, 20, 100, BLACK);
      }
    }
    EndDrawing();
  }

  UnloadTexture(texture);

  CloseWindow();

  return 0;
}
