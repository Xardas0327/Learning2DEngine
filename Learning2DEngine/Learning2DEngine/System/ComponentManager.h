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

            ComponentManager()
                : updaterComponentHandler(), lateUpdaterComponentHandler(), colliderComponentHandler(),
                rendererComponentHandler(), lateRendererComponentHandler()
            {

            }
        public:
            //Update

            inline void AddToUpdate(BaseUpdaterComponent* component)
            {
                updaterComponentHandler.Add(component);
            }

            inline void RemoveFromUpdate(BaseUpdaterComponent* component)
            {
                updaterComponentHandler.Remove(component);
            }

            inline void Update()
            {
                updaterComponentHandler.DoWithAllComponents();
            }

            //LateUpdate

            inline void AddToLateUpdate(BaseLateUpdaterComponent* component)
            {
                lateUpdaterComponentHandler.Add(component);
            }

            inline void RemoveFromLateUpdate(BaseLateUpdaterComponent* component)
            {
                lateUpdaterComponentHandler.Remove(component);
            }

            inline void LateUpdate()
            {
                lateUpdaterComponentHandler.DoWithAllComponents();
            }

            //Render

            inline void AddToRenderer(Render::BaseRendererComponent* component)
            {
                rendererComponentHandler.Add(component);
            }

            inline void RemoveFromRenderer(Render::BaseRendererComponent* component)
            {
                rendererComponentHandler.Remove(component);
            }

            inline void NeedReorderRenderers()
            {
                rendererComponentHandler.NeedReorder();
            }

            inline void Render()
            {
                rendererComponentHandler.DoWithAllComponents();
            }

            //LateRender

            inline void AddToLateRenderer(Render::BaseRendererComponent* component)
            {
                lateRendererComponentHandler.Add(component);
            }

            inline void RemoveFromLateRenderer(Render::BaseRendererComponent* component)
            {
                lateRendererComponentHandler.Remove(component);
            }

            inline void NeedReorderLateRenderers()
            {
                lateRendererComponentHandler.NeedReorder();
            }

            inline void LateRender()
            {
                lateRendererComponentHandler.DoWithAllComponents();
            }

            //Collider

            inline void AddToCollider(Physics::BaseBoxColliderComponent* component)
            {
                colliderComponentHandler.Add(component);
            }

            inline void AddToCollider(Physics::BaseCircleColliderComponent* component)
            {
                colliderComponentHandler.Add(component);
            }

            inline void RemoveFromCollider(Physics::BaseBoxColliderComponent* component)
            {
                colliderComponentHandler.Remove(component);
            }

            inline void RemoveFromCollider(Physics::BaseCircleColliderComponent* component)
            {
                colliderComponentHandler.Remove(component);
            }

            inline void CheckCollision()
            {
                colliderComponentHandler.DoWithAllComponents();
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