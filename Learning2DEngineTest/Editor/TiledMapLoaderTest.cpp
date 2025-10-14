#include "CppUnitTest.h"

#include <rapidxml/rapidxml_utils.hpp>
#include <Learning2DEngine/Editor/TiledMapLoader.h>

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

			TEST_METHOD(LoadMapAttributes)
			{
				auto doc = std::make_unique<rapidxml::xml_document<>>();
				rapidxml::file<> xmlFile("TestData/Editor/MapTest.tmx");
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
		};

	}
}
