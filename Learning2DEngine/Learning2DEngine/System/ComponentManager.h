#pragma once

#include "Singleton.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class RendererComponent;
    }

    namespace System
    {
        class UpdaterComponent;

        class ComponentManager : public virtual Singleton<ComponentManager>
        {
            friend class Singleton<ComponentManager>;
        private:
            // TODO: We will define it later
            ComponentManager() {};
		public:
            // TODO: We will define it later
            void Add(const UpdaterComponent* component) {};
            // TODO: We will define it later
            void Remove(const UpdaterComponent* component) {};
            // TODO: We will define it later
            void Add(const Render::RendererComponent* component) {};
            // TODO: We will define it later
            void Remove(const Render::RendererComponent* component) {};
            // TODO: We will define it later
            void Clear() {};
        };
    }
}