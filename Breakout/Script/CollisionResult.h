#pragma once

#include <glm/glm.hpp>

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct CollisionResult
{
	bool isCollisoned;
	Direction direction;
    // difference vector center - closest point
    glm::vec2 differenceVector;
};