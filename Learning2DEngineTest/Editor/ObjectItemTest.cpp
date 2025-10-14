#include "CppUnitTest.h"

#include <Learning2DEngine/Editor/ObjectItem.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	using namespace System;

	namespace Editor
	{
		TEST_CLASS(ObjectItemTest)
		{
		public:
			TEST_METHOD(Point)
			{
				std::map<std::string, Property> properties;
				properties["prop"] = Property(42);

				glm::vec2 position(1.0f, 2.0f);

				ObjectPoint object(position, std::move(properties));
				ObjectItem item(std::move(object));

				Assert::IsTrue(item.type == ObjectType::POINT);
				const ObjectPoint* o = static_cast<const ObjectPoint*>(item.GetData());

				Assert::IsTrue(o->position == glm::vec2(1.0f, 2.0f));
				Assert::AreEqual(o->visible, true);
				Assert::AreEqual(o->properties.size(), size_t(1));
				Assert::AreEqual(o->properties.at("prop").GetInt(), 42);
			}

			TEST_METHOD(Box)
			{
				std::map<std::string, Property> properties;
				properties["prop"] = Property(42);

				glm::vec2 position(1.0f, 2.0f);
				glm::vec2 size(1.5f, 2.7f);

				ObjectBox object(position, size, std::move(properties));
				ObjectItem item(std::move(object));

				Assert::IsTrue(item.type == ObjectType::BOX);
				const ObjectBox* o = static_cast<const ObjectBox*>(item.GetData());

				Assert::IsTrue(o->position == glm::vec2(1.0f, 2.0f));
				Assert::IsTrue(o->size == glm::vec2(1.5f, 2.7f));
				Assert::AreEqual(o->visible, true);
				Assert::AreEqual(o->properties.size(), size_t(1));
				Assert::AreEqual(o->properties.at("prop").GetInt(), 42);
			}

			TEST_METHOD(Ellipse)
			{
				std::map<std::string, Property> properties;
				properties["prop"] = Property(42);

				glm::vec2 position(1.0f, 2.0f);
				glm::vec2 size(1.5f, 2.7f);

				ObjectEllipse object(position, size, std::move(properties));
				ObjectItem item(std::move(object));

				Assert::IsTrue(item.type == ObjectType::ELLIPSE);
				const ObjectEllipse* o = static_cast<const ObjectEllipse*>(item.GetData());

				Assert::IsTrue(o->position == glm::vec2(1.0f, 2.0f));
				Assert::IsTrue(o->size == glm::vec2(1.5f, 2.7f));
				Assert::AreEqual(o->visible, true);
				Assert::AreEqual(o->properties.size(), size_t(1));
				Assert::AreEqual(o->properties.at("prop").GetInt(), 42);
			}

			TEST_METHOD(Image)
			{
				std::map<std::string, Property> properties;
				properties["prop"] = Property(42);

				glm::vec2 position(1.0f, 2.0f);
				glm::vec2 size(1.5f, 2.7f);

				ObjectImage object(position, size, std::move(properties), 21);
				ObjectItem item(std::move(object));

				Assert::IsTrue(item.type == ObjectType::IMAGE);
				const ObjectImage* o = static_cast<const ObjectImage*>(item.GetData());

				Assert::IsTrue(o->position == glm::vec2(1.0f, 2.0f));
				Assert::IsTrue(o->size == glm::vec2(1.5f, 2.7f));
				Assert::AreEqual(o->visible, true);
				Assert::AreEqual(o->properties.size(), size_t(1));
				Assert::AreEqual(o->properties.at("prop").GetInt(), 42);
				Assert::AreEqual(o->gid, 21);
			}
		};

	}
}
