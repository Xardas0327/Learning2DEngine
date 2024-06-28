#pragma once

#include <list>

namespace Learning2DEngine
{
	namespace System
	{
		template<class ...Params>
		class EventHandler
		{
		public:
			using EventFunction = void(*)(Params...);

		private:
			std::list<EventFunction> events;

		public:
			inline void Add(const EventFunction e)
			{
				events.push_back(e);
			}

			inline void Remove(const EventFunction e)
			{
				events.remove(e);
			}

			inline void Clear()
			{
				events.clear();
			}

			inline void Invoke(Params... params)
			{
				for (EventFunction func : events)
				{
					func(params...);
				}
			}
		};
	}
}