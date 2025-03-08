#pragma once

#include <map>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>

#include "../System/IComponentHandler.h"
#include "RenderData.h"
#include "IRenderer.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class RendererComponentHandler final : public System::IComponentHandler
        {
        private:
            //The int is, that how many data use this renderer.
            std::map<std::string, IRenderer*> renderers;
            //The int is the layer.
            std::map<int, std::map<std::string, std::vector<RenderData*>>> renderData;
            //Help for find the data faster.
            std::map<RenderData*, std::tuple<std::string, int>> renderDataMapping;
            std::mutex rendererMutex;
            std::mutex dataMutex;

            void AddData(const std::string& id, RenderData* data, int layer);
            void ChangeLayer(RenderData* data, int newLayer);
            void RemoveData(RenderData* data);
        public:
            RendererComponentHandler();

            bool IsRendererExist(const std::string& id, bool isThreadSafe);
            void AddRenderer(const std::string& id, IRenderer* renderer, bool isThreadSafe);
            void RemoveRenderer(const std::string& id, bool isThreadSafe);

            void AddData(const std::string& id, RenderData* data, int layer, bool isThreadSafe);
            void ChangeLayer(RenderData* data, int newLayer, bool isThreadSafe);
            void RemoveData(RenderData* data, bool isThreadSafe);

			void Run() override;
			void Clear() override;
        };
    }
}