#pragma once

#include <string>

#include "Singleton.h"
#include "GameObjectManager.h"
#include "UpdaterComponentHandler.h"
#include "LateUpdaterComponentHandler.h"
#include "BaseUpdaterComponent.h"
#include "BaseLateUpdaterComponent.h"
#include "../DebugTool/Log.h"
#include "../Render/IRenderer.h"
#include "../Render/RenderData.h"
#include "../Render/RendererComponentHandler.h"
#include "../Render/RendererMode.h"
#include "../Physics/ColliderComponentHandler.h"
#include "../Physics/BaseBoxColliderComponent.h"
#include "../Physics/BaseCircleColliderComponent.h"

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
            Render::RendererComponentHandler lateRendererComponentHandler;

            bool isThreadSafe;

            ComponentManager()
                : updaterComponentHandler(), lateUpdaterComponentHandler(), colliderComponentHandler(),
                rendererComponentHandler(), lateRendererComponentHandler(), isThreadSafe(false)
            {

            }
        public:
            //Update

            inline void AddToUpdate(BaseUpdaterComponent* component)
            {
                updaterComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromUpdate(BaseUpdaterComponent* component)
            {
                updaterComponentHandler.Remove(component, isThreadSafe);
            }

            inline void Update()
            {
                updaterComponentHandler.Run();
            }

            // If it is bigger then 0, than every component handlers and the GameObjectManager will be thread safe.
            // But if it is 0, the thread safe will not be turn off automatically.
            void SetUpdateMaxComponentPerThread(unsigned int value)
            {
                if (value > 0)
                    SetThreadSafe(true);

                updaterComponentHandler.SetMaxComponentPerThread(value);
            }

            //LateUpdate

            inline void AddToLateUpdate(BaseLateUpdaterComponent* component)
            {
                lateUpdaterComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromLateUpdate(BaseLateUpdaterComponent* component)
            {
                lateUpdaterComponentHandler.Remove(component, isThreadSafe);
            }

            inline void LateUpdate()
            {
                lateUpdaterComponentHandler.Run();
            }

            // If it is bigger then 0, than every component handlers and the GameObjectManager will be thread safe.
            // But if it is 0, the thread safe will not be turn off automatically.
            void SetLateUpdateMaxComponentPerThread(unsigned int value)
            {
                if (value > 0)
                    SetThreadSafe(true);

                lateUpdaterComponentHandler.SetMaxComponentPerThread(value);
            }

            //Collider

            inline void AddToCollider(Physics::BaseBoxColliderComponent* component)
            {
                colliderComponentHandler.Add(component, isThreadSafe);
            }

            inline void AddToCollider(Physics::BaseCircleColliderComponent* component)
            {
                colliderComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromCollider(Physics::BaseBoxColliderComponent* component)
            {
                colliderComponentHandler.Remove(component, isThreadSafe);
            }

            inline void RemoveFromCollider(Physics::BaseCircleColliderComponent* component)
            {
                colliderComponentHandler.Remove(component, isThreadSafe);
            }

            inline void CheckCollision()
            {
                colliderComponentHandler.Run();
            }

            // If it is bigger then 0, than every component handlers and the GameObjectManager will be thread safe.
            // But if it is 0, the thread safe will not be turn off automatically.
            void SetMaxColliderPerThread(unsigned int value)
            {
                if (value > 0)
                    SetThreadSafe(true);

                colliderComponentHandler.SetMaxColliderPerThread(value);
            }

            //Render

            bool IsRendererExist(const Render::RendererMode mode, const std::string& id)
            {
                switch (mode)
                {
                case Render::RendererMode::RENDER:
                    return rendererComponentHandler.IsRendererExist(id, isThreadSafe);
                    break;
                case Render::RendererMode::LATERENDER:
                    return lateRendererComponentHandler.IsRendererExist(id, isThreadSafe);
                    break;
                }

                // this one never should happen
                return false;
            }

            void AddRenderer(const Render::RendererMode mode, const std::string& id, Render::IRenderer* renderer)
            {
                switch (mode)
                {
                case Render::RendererMode::RENDER:
                    rendererComponentHandler.AddRenderer(id, renderer, isThreadSafe);
                    break;
                case Render::RendererMode::LATERENDER:
                    lateRendererComponentHandler.AddRenderer(id, renderer, isThreadSafe);
                    break;
                }
            }

            void RemoveRenderer(const Render::RendererMode mode, const std::string& id)
            {
                switch (mode)
                {
                case Render::RendererMode::RENDER:
                    rendererComponentHandler.RemoveRenderer(id, isThreadSafe);
                    break;
                case Render::RendererMode::LATERENDER:
                    lateRendererComponentHandler.RemoveRenderer(id, isThreadSafe);
                    break;
                }
            }

            void AddRenderData(const Render::RendererMode mode, const std::string& id, Render::RenderData* data, int layer)
            {
                switch (mode)
                {
                case Render::RendererMode::RENDER:
                    rendererComponentHandler.AddData(id, data, layer, isThreadSafe);
                    break;
                case Render::RendererMode::LATERENDER:
                    lateRendererComponentHandler.AddData(id, data, layer, isThreadSafe);
                    break;
                }
            }

            void ChangeRenderLayer(const Render::RendererMode mode, Render::RenderData* data, int newLayer)
            {
                switch (mode)
                {
                case Render::RendererMode::RENDER:
                    rendererComponentHandler.ChangeLayer(data, newLayer, isThreadSafe);
                    break;
                case Render::RendererMode::LATERENDER:
                    lateRendererComponentHandler.ChangeLayer(data, newLayer, isThreadSafe);
                    break;
                }
            }

            void RemoveRenderData(const Render::RendererMode mode, Render::RenderData* data)
            {
                switch (mode)
                {
                case Render::RendererMode::RENDER:
                    rendererComponentHandler.RemoveData(data, isThreadSafe);
                    break;
                case Render::RendererMode::LATERENDER:
                    lateRendererComponentHandler.RemoveData(data, isThreadSafe);
                    break;
                }
            }

            inline void Render()
            {
                rendererComponentHandler.Run();
            }

            inline void LateRender()
            {
                lateRendererComponentHandler.Run();
            }

            //All

            //It set the thread safe mode the ComponentManager and the GameObjectManager too.
            inline void SetThreadSafe(bool value)
            {
                isThreadSafe = value;
                GameObjectManager::GetInstance().SetThreadSafe(value);
#if L2DE_DEBUG
                if (!isThreadSafe && (
                    updaterComponentHandler.GetMaxComponentPerThread() > 0
                    || lateUpdaterComponentHandler.GetMaxComponentPerThread() > 0
                    || colliderComponentHandler.GetMaxColliderPerThread() > 0
                    ))
                {
                    L2DE_LOG_WARNING("COMPONENT MANAGER: The thread safe is turned off. But the component handlers still use threads.");
                }
#endif
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
                lateRendererComponentHandler.Clear();
            }
        };
    }
}