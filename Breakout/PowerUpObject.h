#pragma once

#include <string>

#include <glm/glm.hpp>

#include "PowerUpType.h"

struct PowerUpObject
{
    PowerUpType type;
    float duration;
    std::string textureId;
    glm::vec3 color;
};

const PowerUpObject PowerUpSpeed
{
    PowerUpType::SPEED,
    0.0f,
    "powerup_speed",
    glm::vec3(0.5f, 0.5f, 1.0f)
};

const PowerUpObject PowerUpSticky
{
    PowerUpType::STICKY,
    20.0f,
    "powerup_sticky",
    glm::vec3(1.0f, 0.5f, 1.0f)
};

const PowerUpObject PowerUpPassThrough
{
    PowerUpType::PASS_THROUGH,
    10.0f,
    "powerup_passthrough",
    glm::vec3(0.5f, 1.0f, 0.5f)
};

const PowerUpObject PowerUpPadSizeIncrease
{
    PowerUpType::PAD_SIZE_INCREASE,
    0.0f,
    "powerup_increase",
    glm::vec3(1.0f, 0.6f, 0.4f)
};

const PowerUpObject PowerUpConfuse
{
    PowerUpType::CONFUSE,
    15.0f,
    "powerup_confuse",
    glm::vec3(1.0f, 0.3f, 0.3f)
};

const PowerUpObject PowerUpChaos
{
    PowerUpType::CHAOS,
    15.0f,
    "powerup_chaos",
    glm::vec3(0.9f, 0.25f, 0.25f)
};