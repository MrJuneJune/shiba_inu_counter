#pragma once
#include <raylib.h>
#include <string>

struct Entity {
  char *memory_block = NULL;

  Vector3 *position = NULL;
  Vector3 *velocity = NULL;
  Vector3 *acceleration = NULL;
  int *health = NULL;
  int *attackPower = NULL;
  std::string *state = NULL;

  int num_attributes = 5;

  Entity();

  ~Entity();

  void update();
  void printDetails();
};
