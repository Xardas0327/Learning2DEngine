#pragma once

#include "../Render/Resolution.h"
#include "../Render/IResolutionRefresher.h"
#include "../System/EventItem.h"

namespace Learning2DEngine
{
	namespace System
	{
		class ResolutionEventItem final : public virtual EventItem<Render::Resolution>
		{
		private:
			Render::IResolutionRefresher* refresher;
		public:
			ResolutionEventItem(Render::IResolutionRefresher* refresher)
				: refresher(refresher)
			{

			}

			void Call(Render::Resolution resolution) override
			{
				refresher->RefreshResolution(resolution);
			}
		};
	}
}