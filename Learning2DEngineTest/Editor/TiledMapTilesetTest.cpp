#include "CppUnitTest.h"

#include <Learning2DEngine/Editor/TiledMapTileset.h>

#include "../Test/CompareFloat.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	using namespace System;
	using namespace Test;

	namespace Editor
	{
		TEST_CLASS(TiledMapTilesetTest)
		{
		public:
			TEST_METHOD(HasNumber)
			{
				TiledMapTileset tileset;
				tileset.firstGid = 5;
				tileset.tileCount = 10;

				Assert::IsTrue(tileset.HasNumber(5));
				Assert::IsTrue(tileset.HasNumber(8));
				Assert::IsTrue(tileset.HasNumber(14));

				Assert::IsFalse(tileset.HasNumber(4));
				Assert::IsFalse(tileset.HasNumber(15));
			}

			TEST_METHOD(GetLocalId)
			{
				TiledMapTileset tileset;
				tileset.firstGid = 5;
				tileset.tileCount = 10;

				Assert::AreEqual(tileset.GetLocalId(5), 0);
				Assert::AreEqual(tileset.GetLocalId(8), 3);
				Assert::AreEqual(tileset.GetLocalId(14), 9);
			}

			TEST_METHOD(GetUV)
			{
				TiledMapTileset tileset;
				tileset.firstGid = 5;
				tileset.tileCount = 10;
				tileset.columns = 5;
				tileset.spacing = 2;
				tileset.margin = 1;
				tileset.tileSize = glm::vec2(4.0f, 4.0f);
				tileset.imageSize = glm::vec2(32.0f, 32.0f);
				tileset.tileOffset = glm::vec2(0.5f, 0.75f);

				glm::mat4x2 result{
					0.4375f, 0.25f,
					0.53125f, 0.25f,
					0.53125f, 0.34375f,
					0.4375f, 0.34375f
				};


				Assert::IsTrue(CompareFloat::IsEqual(tileset.GetUV(12), result));
			}
		};

	}
}
