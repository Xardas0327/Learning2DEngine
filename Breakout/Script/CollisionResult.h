#pragma once

#include <glm/glm.hpp>

enum OldDirection {
    OLD_UP,
    OLD_RIGHT,
    OLD_DOWN,
    OLD_LEFT
};

struct CollisionResult
{
	bool isCollisoned;
    OldDirection direction;
    // difference vector center - closest point
    glm::vec2 differenceVector;
};