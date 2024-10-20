#include "entity.h"
#include <cstdlib>
#include <iostream>
#include <string>

Entity::Entity() {
  memory_block =
      new char[sizeof(Vector3) * 3 + sizeof(int) * 2 + sizeof(std::string)];

  position = new (memory_block) Vector3;
  velocity = new (static_cast<char *>(memory_block) + sizeof(Vector3)) Vector3;
  acceleration =
      new (static_cast<char *>(memory_block) + sizeof(Vector3) * 2) Vector3;
  health = new (static_cast<char *>(memory_block) + sizeof(Vector3) * 3)
      int32_t(100);
  attackPower = new (static_cast<char *>(memory_block) + sizeof(Vector3) * 3 +
                     sizeof(int)) int32_t(10);
  state = new (static_cast<char *>(memory_block) + sizeof(Vector3) * 3 +
               sizeof(int) * 2) std::string("idle");
}

Entity::~Entity() {
  // Call destructors explicitly
  position->~Vector3();
  velocity->~Vector3();

  // Free the memory block
  delete[] memory_block;
}

void Entity::update() {
  // pos = a + vt + 1/2 * a * t^2
  // velocity = v + a*t
  this->position->x += this->velocity->x;
  this->position->y += this->velocity->y;
  this->position->z += this->velocity->z;

  this->velocity->x += this->acceleration->x;
  this->velocity->y += this->acceleration->y;
  this->velocity->z += this->acceleration->z;

  this->acceleration->x *=
      this->acceleration->x / (1.0f + this->acceleration->x);
  this->acceleration->y *=
      this->acceleration->y / (1.0f + this->acceleration->x);
  this->acceleration->z *=
      this->acceleration->z / (1.0f + this->acceleration->x);
}

void Entity::printDetails() {
  std::cout << "Position: (" << position->x << ", " << position->y << ", "
            << position->z << ")\n";
  std::cout << "Velocity: (" << velocity->x << ", " << velocity->y << ", "
            << velocity->z << ")\n";
  std::cout << "Health: " << *health << "\n";
  std::cout << "Attack Power: " << *attackPower << "\n";
  std::cout << "State: " << *state << "\n";
}
