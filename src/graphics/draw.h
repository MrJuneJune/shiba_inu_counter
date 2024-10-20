#pragma once
#include "entity.h"

void drawEntity(const Entity &entity);

void drawEntity(const Entity &entity, const int maxWith, const int maxWidth);

void drawEntity(const Entity &entity, const int maxWith, const int maxWidth,
                const Texture2D &texture);
