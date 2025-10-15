#include "CppUnitTest.h"

#include <rapidxml/rapidxml_utils.hpp>
#include <Learning2DEngine/Editor/TiledMapLoader.h>
#include <Learning2DEngine/System/ResourceManager.h>

#include "../Test/CompareFloat.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	using namespace System;
	using namespace Test;

	namespace Editor
	{
		TEST_CLASS(TiledMapLoaderTest)
		{
		public:
			TEST_METHOD(ConvertStringToColor)
			{
				glm::vec4 c1(0.0f, 1.0f, 0.9411f, 1.0f);
				Assert::IsTrue(CompareFloat::IsEqual(
					TiledMapLoader::ConvertStringToColor("#00fff0"),
					c1
				));

				glm::vec4 c2(0.0f, 1.0f, 0.9411f, 0.502f);
				Assert::IsTrue(CompareFloat::IsEqual(
					TiledMapLoader::ConvertStringToColor("#8000fff0"),
					c2
				));
			}

			TEST_METHOD(LoadMapBackground)
			{
				rapidxml::file<> xmlFile("TestData/Editor/MapTest.tmx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto mapNode = doc->first_node(TiledMapNodeMap);

				Assert::AreEqual(TiledMapLoader::LoadMapBackground(mapNode), true);
			}

			TEST_METHOD(LoadMapAttributes)
			{
				rapidxml::file<> xmlFile("TestData/Editor/MapTest.tmx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto mapNode = doc->first_node(TiledMapNodeMap);
				Assert::IsNotNull(mapNode);

				TiledMap map;
				TiledMapLoader::LoadMapAttributes(map, mapNode);
				
				Assert::AreEqual(map.GetVersion(), std::string("1.10"));
				Assert::AreEqual(map.GetTiledVersion(), std::string("1.11.2"));
				Assert::AreEqual(map.GetOrientation(), std::string("orthogonal"));
				Assert::AreEqual(map.GetWidth(), 36);
				Assert::AreEqual(map.GetHeight(), 20);
				Assert::AreEqual(map.GetTileWidth(), 8);
				Assert::AreEqual(map.GetTileHeight(), 8);
				Assert::AreEqual(map.IsInfinite(), false);
				Assert::IsTrue(CompareFloat::IsEqual(map.GetBackgroundColor(), TiledMapLoader::ConvertStringToColor("#0099e6")));

			}

			TEST_METHOD(LoadPropertiesWithoutProperties)
			{
				rapidxml::file<> xmlFile("TestData/Editor/Red.tsx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto tilesetNode = doc->first_node(TiledMapNodeTileset);
				std::map<std::string, System::Property> properties;
				TiledMapLoader::LoadProperties(properties, tilesetNode);


				Assert::AreEqual(properties.size(), size_t(0));
			}

			TEST_METHOD(LoadPropertiesWithProperties)
			{
				rapidxml::file<> xmlFile("TestData/Editor/Green.tsx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto tilesetNode = doc->first_node(TiledMapNodeTileset);
				std::map<std::string, System::Property> properties;
				TiledMapLoader::LoadProperties(properties, tilesetNode, "C:/Images/");

				Assert::AreEqual(properties.size(), size_t(6));

				Assert::AreEqual(properties["Bool"].GetBool(), true);

				Assert::IsTrue(properties["Color"].GetType() == PropertyType::COLOR);
				Assert::IsTrue(CompareFloat::IsEqual(properties["Color"].GetColor(), TiledMapLoader::ConvertStringToColor("#ff008000")));

				Assert::IsTrue(properties["File"].GetType() == PropertyType::FILE);
				Assert::AreEqual(properties["File"].GetFile(), std::string("C:/Images/MapTest.tmx"));

				Assert::IsTrue(properties["Float"].GetType() == PropertyType::FLOAT);
				Assert::AreEqual(properties["Float"].GetFloat(), 1.2f);

				Assert::IsTrue(properties["Int"].GetType() == PropertyType::INT);
				Assert::AreEqual(properties["Int"].GetInt(), 5);

				Assert::IsTrue(properties["String"].GetType() == PropertyType::STRING);
				Assert::AreEqual(properties["String"].GetString(), std::string("Green"));
				
			}

			TEST_METHOD(LoadTilesPropertiesWithoutProperties)
			{
				rapidxml::file<> xmlFile("TestData/Editor/Red.tsx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto tilesetNode = doc->first_node(TiledMapNodeTileset);
				std::map<int, std::map<std::string, System::Property>> properties;
				TiledMapLoader::LoadTilesProperties(properties, tilesetNode, "SourceName");

				Assert::AreEqual(properties.size(), size_t(0));
			}

			TEST_METHOD(LoadTilesPropertiesWithProperties)
			{
				rapidxml::file<> xmlFile("TestData/Editor/Green.tsx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto tilesetNode = doc->first_node(TiledMapNodeTileset);
				std::map<int, std::map<std::string, System::Property>> properties;
				TiledMapLoader::LoadTilesProperties(properties, tilesetNode, "SourceName");

				Assert::AreEqual(properties.size(), size_t(2));

				Assert::AreEqual(properties.count(0), size_t(0));
				Assert::AreEqual(properties.count(2), size_t(1));
				Assert::AreEqual(properties.count(3), size_t(1));

				Assert::IsTrue(properties[2]["Type"].GetType() == PropertyType::STRING);
				Assert::AreEqual(properties[2]["Type"].GetString(), std::string("Dark"));
				Assert::IsTrue(properties[3]["Type"].GetType() == PropertyType::STRING);
				Assert::AreEqual(properties[3]["Type"].GetString(), std::string("Light"));
			}

			TEST_METHOD(LoadObjectItemsWithoutObjects)
			{
				rapidxml::file<> xmlFile("TestData/Editor/Red.tsx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto tileNode = doc->first_node(TiledMapNodeTileset)->first_node(TiledMapNodeTile);

				std::vector<ObjectItem> objectItems;
				TiledMapLoader::LoadObjectItems(objectItems, tileNode);
				Assert::AreEqual(objectItems.size(), size_t(0));
			}

			TEST_METHOD(LoadObjectItemsWithObjects)
			{
				rapidxml::file<> xmlFile("TestData/Editor/Green.tsx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto tileNode = doc->first_node(TiledMapNodeTileset)->first_node(TiledMapNodeTile)
					->next_sibling(TiledMapNodeTile); //the second tile

				std::vector<ObjectItem> objectItems;
				TiledMapLoader::LoadObjectItems(objectItems, tileNode);
				Assert::AreEqual(objectItems.size(), size_t(1));

				Assert::IsTrue(objectItems[0].type == ObjectType::BOX);
				auto box = static_cast<const ObjectBox*>(objectItems[0].GetData());
				Assert::IsNotNull(box);
				Assert::IsTrue(CompareFloat::IsEqual(box->position, glm::vec2(0.0f, 0.0f)));
				Assert::IsTrue(CompareFloat::IsEqual(box->size, glm::vec2(8.0f, 8.0f)));
			}

			TEST_METHOD(LoadTilesObjectItemsWithoutObjects)
			{
				rapidxml::file<> xmlFile("TestData/Editor/Red.tsx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto tilesetNode = doc->first_node(TiledMapNodeTileset);

				std::map<int, std::vector<ObjectItem>> objectItems;
				TiledMapLoader::LoadTilesObjectItems(objectItems, tilesetNode, "SourceName");
				Assert::AreEqual(objectItems.size(), size_t(0));
			}

			TEST_METHOD(LoadTilesObjectItemsWithObjects)
			{
				rapidxml::file<> xmlFile("TestData/Editor/Green.tsx");
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				doc->parse<0>(xmlFile.data());

				auto tilesetNode = doc->first_node(TiledMapNodeTileset);

				std::map<int, std::vector<ObjectItem>> objectItems;
				TiledMapLoader::LoadTilesObjectItems(objectItems, tilesetNode, "SourceName");
				Assert::AreEqual(objectItems.size(), size_t(1));

				Assert::AreEqual(objectItems.count(0), size_t(0));
				Assert::AreEqual(objectItems.count(3), size_t(1));


				Assert::IsTrue(objectItems[3][0].type == ObjectType::BOX);
				auto box = static_cast<const ObjectBox*>(objectItems[3][0].GetData());
				Assert::IsNotNull(box);
				Assert::IsTrue(CompareFloat::IsEqual(box->position, glm::vec2(0.0f, 0.0f)));
				Assert::IsTrue(CompareFloat::IsEqual(box->size, glm::vec2(8.0f, 8.0f)));
			}

			TEST_METHOD(LoadTileset)
			{
				std::map<std::string, std::string> textureMap;
				textureMap["Green"] = "Green";
				TiledMapTileset tileset;
				TiledMapLoader::LoadTileset("TestData/Editor/", "Green.tsx", textureMap, tileset);

				Assert::IsTrue(CompareFloat::IsEqual(tileset.tileSize, glm::vec2(8.0f, 8.0f)));
				Assert::IsTrue(CompareFloat::IsEqual(tileset.tileOffset, glm::vec2(0.0f, 0.0f)));
				Assert::AreEqual(tileset.columns, 2);
				Assert::AreEqual(tileset.tileCount, 4);
				Assert::AreEqual(tileset.spacing, 0);
				Assert::AreEqual(tileset.margin, 0);

			}
		};

	}
}
