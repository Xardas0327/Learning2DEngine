#pragma once

#include <string>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/Render/IResolutionRefresher.h>
#include <Learning2DEngine/Render/Resolution.h>
#include <Learning2DEngine/EventSystem/ResolutionEventItem.h>

class BackgroundController : public virtual Learning2DEngine::System::Component, private Learning2DEngine::Render::IResolutionRefresher
{
    friend class Learning2DEngine::System::GameObject;
private:
    void RefreshResolution(const Learning2DEngine::Render::Resolution& resolution) override;

protected:
    Learning2DEngine::Render::SpriteRenderer* renderer;
    Learning2DEngine::EventSystem::ResolutionEventItem resolutionEventItem;
    const std::string textureId;

    BackgroundController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId,
        const Learning2DEngine::Render::Resolution& resolution);

    void Init() override;
    void Destroy() override;
};