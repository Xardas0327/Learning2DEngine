#include "RendererComponentHandler.h"

namespace Learning2DEngine
{
    namespace Render
    {
		RendererComponentHandler::RendererComponentHandler()
			: renderers(), renderData(), renderDataMapping(), rendererMutex(), dataMutex()
		{
		}

		void RendererComponentHandler::Add(const std::string& id, IRenderer* renderer, bool mustOverride)
		{
			bool isFound = renderers.find(id) != renderers.end();
			if (!isFound || mustOverride)
			{
				if (isFound)
					renderers[id] = std::make_tuple(renderer, std::get<1>(renderers[id]));
				else
					renderers[id] = std::make_tuple(renderer, 0);
			}
		}

		void RendererComponentHandler::Add(const std::string& id, IRenderer* renderer, bool mustOverride, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(rendererMutex);
				Add(id, renderer, mustOverride);
			}
			else
			{
				Add(id, renderer, mustOverride);
			}
		}


		void RendererComponentHandler::Add(const std::string& id, IRenderData* data, int layer)
		{
			renderData[layer][id].push_back(data);
			renderDataMapping[data] = std::make_tuple(id, layer);
			++std::get<1>(renderers[id]);
		}

		void RendererComponentHandler::Add(const std::string& id, IRenderData* data, int layer, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(rendererMutex);
				std::lock_guard<std::mutex> lock(dataMutex);
				Add(id, data, layer);
			}
			else
			{
				Add(id, data, layer);
			}
		}

		void RendererComponentHandler::ChangeLayer(IRenderData* data, int newLayer)
		{
			bool isFound = renderDataMapping.find(data) != renderDataMapping.end();
			if (!isFound)
				return;

			const int oldLayer = std::get<1>(renderDataMapping[data]);
			const std::string& id = std::get<0>(renderDataMapping[data]);

			renderData[oldLayer][id].erase(
				std::remove(renderData[oldLayer][id].begin(), renderData[oldLayer][id].end(), data), renderData[oldLayer][id].end()
			);
			renderData[newLayer][id].push_back(data);
			renderDataMapping[data] = std::make_tuple(id, newLayer);
		}

		void RendererComponentHandler::ChangeLayer(IRenderData* data, int newLayer, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(dataMutex);
				ChangeLayer(data, newLayer);
			}
			else
			{
				ChangeLayer(data, newLayer);
			}
		}

		void RendererComponentHandler::Remove(IRenderData* data)
		{
			bool isFound = renderDataMapping.find(data) != renderDataMapping.end();
			if (!isFound)
				return;

			const int layer = std::get<1>(renderDataMapping[data]);
			const std::string& id = std::get<0>(renderDataMapping[data]);

			renderData[layer][id].erase(
				std::remove(renderData[layer][id].begin(), renderData[layer][id].end(), data), renderData[layer][id].end()
			);
			renderDataMapping.erase(data);
			--std::get<1>(renderers[id]);

			if (std::get<1>(renderers[id]) == 0)
				renderers.erase(id);
		}

		void RendererComponentHandler::Remove(IRenderData* data, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(rendererMutex);
				std::lock_guard<std::mutex> lock(dataMutex);
				Remove(data);
			}
			else
			{
				Remove(data);
			}
		}

		void RendererComponentHandler::Run()
		{
			for (auto& data : renderData)
			{
				for (auto& dataPair : data.second)
				{
					auto& renderer = std::get<0>(renderers[dataPair.first]);
					renderer->Draw(dataPair.second);
				}
			}
		}

		void RendererComponentHandler::Clear()
		{
			renderers.clear();
			renderData.clear();
			renderDataMapping.clear();
		}
    }
}