#include "CppUnitTest.h"

#include <Learning2DEngine/EventSystem/EventHandler.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	namespace EventSystem
	{
		TEST_CLASS(EventHandlerTest)
		{
			class EventItemTest : public EventItem<int>
			{
			public:
				int value;

				EventItemTest(): value(0)
				{
				}

				void Call(int value) override
				{
					this->value = value;
				};
			};
		public:
			TEST_METHOD(Invoke)
			{
				EventHandler<int> handler;
				EventItemTest item1;

				handler.Invoke(42);
				Assert::AreNotEqual(42, item1.value);
			}

			TEST_METHOD(Add)
			{
				EventHandler<int> handler;
				EventItemTest item1;
				EventItemTest item2;
				handler.Add(&item1);
				handler.Add(&item2);

				handler.Invoke(42);
				Assert::AreEqual(42, item1.value);
				Assert::AreEqual(42, item2.value);
			}

			TEST_METHOD(Remove)
			{
				EventHandler<int> handler;
				EventItemTest item1;
				EventItemTest item2;
				handler.Add(&item1);
				handler.Add(&item2);

				handler.Invoke(42);
				Assert::AreEqual(42, item1.value);
				Assert::AreEqual(42, item2.value);

				handler.Remove(&item1);
				handler.Invoke(5);
				Assert::AreNotEqual(5, item1.value);
				Assert::AreEqual(5, item2.value);
			}

			TEST_METHOD(Clear)
			{
				EventHandler<int> handler;
				EventItemTest item1;
				EventItemTest item2;
				handler.Add(&item1);
				handler.Add(&item2);

				handler.Invoke(42);
				Assert::AreEqual(42, item1.value);
				Assert::AreEqual(42, item2.value);

				handler.Clear();
				handler.Invoke(5);
				Assert::AreNotEqual(5, item1.value);
				Assert::AreNotEqual(5, item2.value);
			}
		};
	}
}