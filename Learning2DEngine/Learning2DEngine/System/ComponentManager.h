#pragma once

#include "Singleton.h"
#include "UpdaterComponentHandler.h"
#include "LateUpdaterComponentHandler.h"
#include "BaseUpdaterComponent.h"
#include "BaseLateUpdaterComponent.h"
#include "../Render/BaseRendererComponent.h"
#include "../Render/RendererComponentHandler.h"
#include "../Physics/ColliderComponentHandler.h"
#include "../Physics/BaseBoxColliderComponent.h"
#include "../Physics/BaseCircleColliderComponent.h"

namespace Learning2DEngine
{

    namespace System
    {
        class ComponentManager final : public virtual Singleton<ComponentManager>
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

            //Render

            inline void AddToRenderer(Render::BaseRendererComponent* component)
            {
                rendererComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromRenderer(Render::BaseRendererComponent* component)
            {
                rendererComponentHandler.Remove(component, isThreadSafe);
            }

            inline void NeedReorderRenderers()
            {
                rendererComponentHandler.NeedReorder();
            }

            inline void Render()
            {
                rendererComponentHandler.Run();
            }

            //LateRender

            inline void AddToLateRenderer(Render::BaseRendererComponent* component)
            {
                lateRendererComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromLateRenderer(Render::BaseRendererComponent* component)
            {
                lateRendererComponentHandler.Remove(component, isThreadSafe);
            }

            inline void NeedReorderLateRenderers()
            {
                lateRendererComponentHandler.NeedReorder();
            }

            inline void LateRender()
            {
                lateRendererComponentHandler.Run();
            }

            //All

            inline void SetThreadSafe(bool value)
            {
                isThreadSafe = value;
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