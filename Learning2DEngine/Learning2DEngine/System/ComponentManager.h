#pragma once

#include <string>

#include "Singleton.h"
#include "GameObjectManager.h"
#include "UpdaterComponentHandler.h"
#include "LateUpdaterComponentHandler.h"
#include "UpdaterComponent.h"
#include "LateUpdaterComponent.h"
#include "ThreadManager.h"
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
			ThreadManager* threadManager;

            ComponentManager()
                : updaterComponentHandler(), lateUpdaterComponentHandler(), colliderComponentHandler(),
				rendererComponentHandler(), isThreadSafe(false), threadManager(nullptr)
            {

            }

            void CreateThreadManager()
            {
                if (threadManager == nullptr)
                {
                    auto threadCount = std::max(1u, std::thread::hardware_concurrency() - 1u);
                    threadManager = new ThreadManager(threadCount);
                }
            }

            void DestroyThreadManager()
            {
                if (threadManager != nullptr)
                {
                    delete threadManager;
                    threadManager = nullptr;
                }
            }

        public:
            ~ComponentManager()
            {
                DestroyThreadManager();
            }

            void Clear()
            {
                updaterComponentHandler.Clear();
                lateUpdaterComponentHandler.Clear();
                colliderComponentHandler.Clear();
                rendererComponentHandler.Clear();
            }

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

            //The UpdaterComponentHandler will use the thread manager,
            //and it turns on the thread safe mode to the ComponentManager and the GameObjectManager too.
            void UseUpdaterThreads()
            {
                SetThreadSafeMode(true);
                CreateThreadManager();

                updaterComponentHandler.SetThreadManager(threadManager);
			}

			//The UpdaterComponentHandler will not use the thread manager anymore,
			//but the thread manager will not be destroyed and the thread safe mode will not be disabled.
            void StopUpdaterThreads()
            {
                updaterComponentHandler.ClearThreadManager();
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

            //The LateUpdaterComponentHandler will use the thread manager,
            //and it turns on the thread safe mode to the ComponentManager and the GameObjectManager too.
            void UseLateUpdaterThreads()
            {
                SetThreadSafeMode(true);
                CreateThreadManager();

                lateUpdaterComponentHandler.SetThreadManager(threadManager);
            }

            //The LateUpdaterComponentHandler will not use the thread manager anymore,
            //but the thread manager will not be destroyed and the thread safe mode will not be disabled.
            void StopLateUpdaterThreads()
            {
                lateUpdaterComponentHandler.ClearThreadManager();
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

            //Thread

			//It activates the threads for the UpdaterComponentHandler and the LateUpdaterComponentHandler,
            //and it turns on the thread safe mode to the ComponentManager and the GameObjectManager too.
            void UseThreadsEverywhere()
            {
                SetThreadSafeMode(true);
                CreateThreadManager();

				updaterComponentHandler.SetThreadManager(threadManager);
                lateUpdaterComponentHandler.SetThreadManager(threadManager);
            }

            //It deactivates the threads for the UpdaterComponentHandler and the LateUpdaterComponentHandler,
            //and it turns off the thread safe mode to the ComponentManager and the GameObjectManager too.
            void StopThreads(bool destroyThreadManager = false)
            {
				updaterComponentHandler.ClearThreadManager();
				lateUpdaterComponentHandler.ClearThreadManager();

                if (destroyThreadManager)
                    DestroyThreadManager();

                SetThreadSafeMode(false);
			}

            inline bool IsUseThreads() const
            {
                return updaterComponentHandler.IsUseThreads() || lateUpdaterComponentHandler.IsUseThreads();
			}

            //It turns on/off the thread safe mode to the ComponentManager and the GameObjectManager too.
            void SetThreadSafeMode(bool value)
            {
                isThreadSafe = value;
                GameObjectManager::GetInstance().SetThreadSafe(value);

                L2DE_LOG_IF_WARNING(
                    !isThreadSafe && (
                        updaterComponentHandler.IsUseThreads()
                        || lateUpdaterComponentHandler.IsUseThreads()),
                    "COMPONENT MANAGER: The thread safe is turned off. But the component handlers still use threads."
                )
            }

            inline bool IsThreadSafe() const
            {
                return isThreadSafe;
            }
        };
    }
}