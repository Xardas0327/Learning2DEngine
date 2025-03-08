#include "RendererComponentHandler.h"

namespace Learning2DEngine
{
    namespace Render
    {
		RendererComponentHandler::RendererComponentHandler()
			: renderers(), renderData(), renderDataMapping(), rendererMutex(), dataMutex()
		{
		}

		bool RendererComponentHandler::IsRendererExist(const std::string& id, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(rendererMutex);
				return renderers.find(id) != renderers.end();
			}
			else
			{
				return renderers.find(id) != renderers.end();
			}
		}

		void RendererComponentHandler::AddRenderer(const std::string& id, IRenderer* renderer, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(rendererMutex);
				renderers[id] = renderer;
			}
			else
			{
				renderers[id] = renderer;
			}
		}

		void RendererComponentHandler::RemoveRenderer(const std::string& id, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(rendererMutex);
				renderers.erase(id);
			}
			else
			{
				renderers.erase(id);
			}
		}

		void RendererComponentHandler::AddData(const std::string& id, RenderData* data, int layer)
		{
			renderData[layer][id].push_back(data);
			renderDataMapping[data] = std::make_tuple(id, layer);
		}

		void RendererComponentHandler::AddData(const std::string& id, RenderData* data, int layer, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(dataMutex);
				AddData(id, data, layer);
			}
			else
			{
				AddData(id, data, layer);
			}
		}

		void RendererComponentHandler::ChangeLayer(RenderData* data, int newLayer)
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

		void RendererComponentHandler::ChangeLayer(RenderData* data, int newLayer, bool isThreadSafe)
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

		void RendererComponentHandler::RemoveData(RenderData* data)
		{
			bool isFound = renderDataMapping.find(data) != renderDataMapping.end();
			if (!isFound)
				return;

			const int layer = std::get<1>(renderDataMapping[data]);
			const std::string& id = std::get<0>(renderDataMapping[data]);

			renderData[layer][id].erase(
				std::remove(renderData[layer][id].begin(), renderData[layer][id].end(), data), renderData[layer][id].end()
			);
			if (renderData[layer][id].size() == 0)
			{
				if(renderData[layer].size() == 0)
					renderData.erase(layer);
				else
					renderData[layer].erase(id);

			}

			renderDataMapping.erase(data);
		}

		void RendererComponentHandler::RemoveData(RenderData* data, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(dataMutex);
				RemoveData(data);
			}
			else
			{
				RemoveData(data);
			}
		}

		void RendererComponentHandler::Run()
		{
			for (auto& data : renderData)
			{
				for (auto& dataPair : data.second)
				{
					renderers[dataPair.first]->Draw(dataPair.second);
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