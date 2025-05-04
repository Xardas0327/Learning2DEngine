#pragma once

#include <Learning2DEngine/System/LateUpdaterComponent.h>
#include <Learning2DEngine/System/GameObject.h>

class PlayerController : public virtual Learning2DEngine::System::LateUpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
	float cameraDistanceX;

    PlayerController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void LateUpdate() override;
};