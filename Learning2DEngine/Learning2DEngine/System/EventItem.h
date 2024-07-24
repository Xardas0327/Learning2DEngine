#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		template<class ...Params>
		class EventItem
		{
		public:
			virtual ~EventItem() {}

			virtual void Call(Params... params) = 0;
		};
	}
}