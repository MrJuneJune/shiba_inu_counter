#include "draw.h"
#include <raylib.h>

void drawEntity(const Entity &entity) {
  DrawRectangle(entity.position->x, entity.position->y, 100, 100, BLUE);
}

void drawEntity(const Entity &entity, const int maxWith, const int maxWidth) {
  if (entity.position->x < 0 || entity.position->x >= maxWith) {
    entity.velocity->x = -entity.velocity->x;
  }
  if (entity.position->y < 0 || entity.position->y >= maxWith) {
    entity.velocity->y = -entity.velocity->y;
  }
  DrawRectangle(entity.position->x, entity.position->y, 5, 5, BLUE);
}

void drawEntity(const Entity &entity, const int maxWidth, const int maxHeight,
                const Texture2D &texture) {
  if (entity.position->x < 0 ||
      entity.position->x >= (maxWidth - texture.width)) {
    entity.velocity->x = -entity.velocity->x;
  }
  if (entity.position->y < texture.height ||
      entity.position->y >= (maxHeight - texture.height)) {
    entity.velocity->y = -entity.velocity->y;
  }
  DrawTexture(texture, entity.position->x, entity.position->y, WHITE);
}
