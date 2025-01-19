#pragma once

#include "Singleton.h"
#include "../Render/BaseRendererComponent.h"
#include "../Render/RendererComponentHandler.h"

namespace Learning2DEngine
{

    namespace System
    {
        class UpdaterComponent;

        class ComponentManager final : public virtual Singleton<ComponentManager>
        {
            friend class Singleton<ComponentManager>;
        private:
			Render::RendererComponentHandler rendererComponentHandler;

            ComponentManager()
                : rendererComponentHandler()
            {

            }
		public:
            // TODO: We will define it later
            void Add(const UpdaterComponent* component) {};
            // TODO: We will define it later
            void Remove(const UpdaterComponent* component) {};

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

            void Clear()
            {
				rendererComponentHandler.Clear();
            }
        };
    }
}