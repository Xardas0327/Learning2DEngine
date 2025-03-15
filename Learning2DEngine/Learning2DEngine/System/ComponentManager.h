#pragma once

#include <string>

#include "Singleton.h"
#include "UpdaterComponentHandler.h"
#include "LateUpdaterComponentHandler.h"
#include "BaseUpdaterComponent.h"
#include "BaseLateUpdaterComponent.h"
#include "../Render/IRenderer.h"
#include "../Render/RenderData.h"
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

            // If it is bigger then 0, than every component handler will be thread safe.
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

            // If it is bigger then 0, than every component handler will be thread safe.
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

            // If it is bigger then 0, than every component handler will be thread safe.
            // But if it is 0, the thread safe will not be turn off automatically.
            void SetMaxColliderPerThread(unsigned int value)
            {
                if (value > 0)
                    SetThreadSafe(true);

                colliderComponentHandler.SetMaxColliderPerThread(value);
            }

            //Render

            inline bool IsRendererExistInRender(const std::string& id)
            {
                return rendererComponentHandler.IsRendererExist(id, isThreadSafe);
            }

            inline void AddRendererToRender(const std::string& id, Render::IRenderer* renderer)
            {
                rendererComponentHandler.AddRenderer(id, renderer, isThreadSafe);
            }

            inline void RemoveRendererFromRender(const std::string& id)
            {
                rendererComponentHandler.RemoveRenderer(id, isThreadSafe);
            }

            inline void AddDataToRender(const std::string& id, Render::RenderData* data, int layer)
            {
                rendererComponentHandler.AddData(id, data, layer, isThreadSafe);
            }

            inline void ChangeLayerInRender(Render::RenderData* data, int newLayer)
            {
                rendererComponentHandler.ChangeLayer(data, newLayer, isThreadSafe);
            }

            inline void RemoveDataFromRender(Render::RenderData* data)
            {
                rendererComponentHandler.RemoveData(data, isThreadSafe);
            }

            inline void Render()
            {
                rendererComponentHandler.Run();
            }

            //LateRender

            inline bool IsRendererExistInLateRender(const std::string& id)
            {
                return lateRendererComponentHandler.IsRendererExist(id, isThreadSafe);
            }

            inline void AddRendererToLateRender(const std::string& id, Render::IRenderer* renderer)
            {
                lateRendererComponentHandler.AddRenderer(id, renderer, isThreadSafe);
            }

            inline void RemoveRendererFromLateRender(const std::string& id)
            {
                lateRendererComponentHandler.RemoveRenderer(id, isThreadSafe);
            }

            inline void AddDataToLateRender(const std::string& id, Render::RenderData* data, int layer)
            {
                lateRendererComponentHandler.AddData(id, data, layer, isThreadSafe);
            }

            inline void ChangeLayerInLateRender(Render::RenderData* data, int newLayer)
            {
                lateRendererComponentHandler.ChangeLayer(data, newLayer, isThreadSafe);
            }

            inline void RemoveDataFromLateRender(Render::RenderData* data)
            {
                lateRendererComponentHandler.RemoveData(data, isThreadSafe);
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