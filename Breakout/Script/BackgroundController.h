#pragma once

#include <string>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SimpleSpriteRenderComponent.h>
#include <Learning2DEngine/Render/Resolution.h>

class BackgroundController : public Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    Learning2DEngine::Render::SimpleSpriteRenderComponent* renderer;
    const std::string textureId;

    BackgroundController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId,
        const Learning2DEngine::Render::Resolution& resolution);

    void Init() override;
};