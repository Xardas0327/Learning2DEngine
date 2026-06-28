#pragma once

#include <string>

#include "Singleton.h"
#include "GameObjectManager.h"
#include "UpdaterComponentHandler.h"
#include "LateUpdaterComponentHandler.h"
#include "UpdaterComponent.h"
#include "LateUpdaterComponent.h"
#include "../DebugTool/Log.h"
#include "../Render/IRenderer.h"
#include "../Render/RenderData.h"
#include "../Render/RendererComponentHandler.h"
#include "../Render/RendererMode.h"
#include "../Physics/ColliderComponentHandler.h"
#include "../Physics/BoxColliderComponent.h"
#include "../Physics/CircleColliderComponent.h"

namespace Learning2DEngine
{

    namespace System
    {
        class ComponentManager final : public Singleton<ComponentManager>
        {
            friend class Singleton<ComponentManager>;
        private:
            UpdaterComponentHandler updaterComponentHandler;
            LateUpdaterComponentHandler lateUpdaterComponentHandler;
            Physics::ColliderComponentHandler colliderComponentHandler;
            Render::RendererComponentHandler rendererComponentHandler;

            bool isThreadSafe;

            ComponentManager()
                : updaterComponentHandler(), lateUpdaterComponentHandler(), colliderComponentHandler(),
                rendererComponentHandler(), isThreadSafe(false)
            {

            }
        public:
            //Update

            inline void AddToUpdate(UpdaterComponent* component)
            {
                updaterComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromUpdate(UpdaterComponent* component)
            {
                updaterComponentHandler.Remove(component, isThreadSafe);
            }

            inline void Update()
            {
                updaterComponentHandler.Run();
            }

            //LateUpdate

            inline void AddToLateUpdate(LateUpdaterComponent* component)
            {
                lateUpdaterComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromLateUpdate(LateUpdaterComponent* component)
            {
                lateUpdaterComponentHandler.Remove(component, isThreadSafe);
            }

            inline void LateUpdate()
            {
                lateUpdaterComponentHandler.Run();
            }

            //Collider

            inline void AddToCollider(Physics::BoxColliderComponent* component)
            {
                colliderComponentHandler.Add(component, isThreadSafe);
            }

            inline void AddToCollider(Physics::CircleColliderComponent* component)
            {
                colliderComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromCollider(Physics::BoxColliderComponent* component)
            {
                colliderComponentHandler.Remove(component, isThreadSafe);
            }

            inline void RemoveFromCollider(Physics::CircleColliderComponent* component)
            {
                colliderComponentHandler.Remove(component, isThreadSafe);
            }

            inline void CheckCollision()
            {
                colliderComponentHandler.Run();
            }

            //Render

            inline bool IsRendererExist(const std::string& id)
            {
                return rendererComponentHandler.IsRendererExist(id, isThreadSafe);
            }

            inline void AddRenderer(const std::string& id, Render::IRenderer* renderer)
            {
                rendererComponentHandler.AddRenderer(id, renderer, isThreadSafe);
            }

            inline void RemoveRenderer(const std::string& id)
            {
                rendererComponentHandler.RemoveRenderer(id, isThreadSafe);
            }

            inline void AddRenderData(Render::RendererMode mode, const std::string& id, Render::RenderData* data, int layer)
            {
                rendererComponentHandler.AddData(id, mode, data, layer, isThreadSafe);
            }

            inline void ChangeRenderLayer(Render::RenderData* data, int newLayer)
            {
                rendererComponentHandler.ChangeLayer(data, newLayer, isThreadSafe);
            }

            inline void RemoveRenderData(Render::RenderData* data)
            {
                rendererComponentHandler.RemoveData(data, isThreadSafe);
            }

            inline void SetDataToRenderers()
            {
                rendererComponentHandler.SetDataToRenderers();
            }

            inline void Render()
            {
                rendererComponentHandler.Run(Render::RendererMode::RENDER);
            }

            inline void LateRender()
            {
                rendererComponentHandler.Run(Render::RendererMode::LATERENDER);
            }

            //All

            //It set the thread safe mode the ComponentManager and the GameObjectManager too.
            inline void SetThreadSafe(bool value)
            {
                isThreadSafe = value;
                GameObjectManager::GetInstance().SetThreadSafe(value);
            }

            inline bool GetThreadSafe()
            {
                return isThreadSafe;
            }

            void Clear()
            {
                updaterComponentHandler.Clear();
                lateUpdaterComponentHandler.Clear();
                colliderComponentHandler.Clear();
                rendererComponentHandler.Clear();
            }
        };
    }
}