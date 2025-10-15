#include "CppUnitTest.h"

#include <Learning2DEngine/System/Property.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	namespace System
	{
		TEST_CLASS(PropertyTest)
		{
		public:
			TEST_METHOD(None)
			{
				Property p;

				Assert::IsTrue(p.GetType() == PropertyType::NONE);
			}

			TEST_METHOD(Bool)
			{
				Property p(true);

				Assert::IsTrue(p.GetType() == PropertyType::BOOL);
				Assert::AreEqual(p.GetBool(), true);
			}

			TEST_METHOD(Color)
			{
				glm::vec4 color(1.0f, 0.5f, 0.25f, 0.75f);
				Property p(color);

				Assert::IsTrue(p.GetColor() == color);
			}

			TEST_METHOD(Float)
			{
				Property p(3.14f);

				Assert::IsTrue(p.GetType() == PropertyType::FLOAT);
				Assert::AreEqual(p.GetFloat(), 3.14f);
			}

			TEST_METHOD(Int)
			{
				Property p(42);

				Assert::IsTrue(p.GetType() == PropertyType::INT);
				Assert::AreEqual(p.GetInt(), 42);
			}

			TEST_METHOD(String)
			{
				std::string text = "Text";
				Property p(text);

				Assert::IsTrue(p.GetType() == PropertyType::STRING);
				Assert::AreEqual(p.GetString(), text);
			}

			TEST_METHOD(File)
			{
				std::string path = "C:/Path/To/File.txt";
				Property p(path, true);

				Assert::IsTrue(p.GetType() == PropertyType::FILE);
				Assert::AreEqual(p.GetFile(), path);
			}
		};

	}
}
