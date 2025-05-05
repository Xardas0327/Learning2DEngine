#pragma once

#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/LateUpdaterComponent.h>

class PlayerController : public virtual Learning2DEngine::System::LateUpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
	float cameraDistanceX;
    Learning2DEngine::Physics::Rigidbody* rigidbody;

    PlayerController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void LateUpdate() override;
};