#pragma once

#include <map>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>

#include "../System/IComponentHandler.h"
#include "IRenderData.h"
#include "IRenderer.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class RendererComponentHandler final : public System::IComponentHandler
        {
        private:
			//The int is, that how many data use this renderer.
			std::map<std::string, std::tuple<IRenderer*, int>> renderers;
			//The int is the layer.
			std::map<int, std::map<std::string, std::vector<IRenderData*>>> renderData;
            //Help for find the data faster.
            std::map<IRenderData*, std::tuple<std::string, int>> renderDataMapping;
			std::mutex rendererMutex;
            std::mutex dataMutex;

            void Add(const std::string& id, IRenderer* renderer, bool mustOverride);
            void Add(const std::string& id, IRenderData* data, int layer);
            void ChangeLayer(IRenderData* data, int newLayer);
            void Remove(IRenderData* data);
        public:
            RendererComponentHandler();

            void Add(const std::string& id, IRenderer* renderer, bool mustOverride, bool isThreadSafe);
            void Add(const std::string& id, IRenderData* data, int layer, bool isThreadSafe);
            void ChangeLayer(IRenderData* data, int newLayer, bool isThreadSafe);
            void Remove(IRenderData* data, bool isThreadSafe);

			void Run() override;
			void Clear() override;
        };
    }
}