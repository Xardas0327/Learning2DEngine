#pragma once

#include "Singleton.h"
#include "../Render/BaseRendererComponent.h"
#include "../Render/RendererComponentHandler.h"

namespace Learning2DEngine
{

    namespace System
    {
        class ComponentManager final : public virtual Singleton<ComponentManager>
        {
            friend class Singleton<ComponentManager>;
        private:
			Render::RendererComponentHandler rendererComponentHandler;
            Render::RendererComponentHandler lateRendererComponentHandler;

            ComponentManager()
				: rendererComponentHandler(), lateRendererComponentHandler()
            {

            }
		public:
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

            void Clear()
            {
				rendererComponentHandler.Clear();
				lateRendererComponentHandler.Clear();
            }
        };
    }
}