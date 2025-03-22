#include "RendererComponentHandler.h"

#include <set>

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
			renderData[id][layer].push_back(data);
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

			renderData[id][oldLayer].erase(
				std::remove(renderData[id][oldLayer].begin(), renderData[id][oldLayer].end(), data), renderData[id][oldLayer].end()
			);
			renderData[id][oldLayer].push_back(data);
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

			renderData[id][layer].erase(
				std::remove(renderData[id][layer].begin(), renderData[id][layer].end(), data), renderData[id][layer].end()
			);
			if (renderData[id][layer].size() == 0)
			{
				if(renderData[id].size() == 0)
					renderData.erase(id);
				else
					renderData[id].erase(layer);

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
			//Collect active data
			std::set<int> activeLayers;
			std::map<int, std::vector<RenderData*>> activeData;
			std::set<IRenderer*> activeRenderers;
			for (auto& data : renderData)
			{
				for (auto& dataPair : data.second)
				{
					size_t activeDataCount = 0;
					for (const RenderData* renderData : dataPair.second)
					{
						if (renderData->component->isActive && renderData->component->gameObject->isActive)
							activeDataCount++;
					}

					if (activeDataCount > 0)
					{
						activeLayers.insert(dataPair.first);

						if (activeDataCount == dataPair.second.size())
							activeData[dataPair.first] = dataPair.second;
						else
						{
							activeData[dataPair.first].reserve(activeDataCount);
							for (RenderData* renderData : dataPair.second)
							{
								if (renderData->component->isActive && renderData->component->gameObject->isActive)
									activeData[dataPair.first].push_back(renderData);
							}
						}
					}
				}

				if (activeData.size() > 0)
				{
					activeRenderers.insert(renderers[data.first]);

					renderers[data.first]->SetData(activeData);
					activeData.clear();
				}
			}

			//Render
			for (int layer : activeLayers)
			{
				for (auto renderer : activeRenderers)
				{
					renderer->Draw(layer);
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