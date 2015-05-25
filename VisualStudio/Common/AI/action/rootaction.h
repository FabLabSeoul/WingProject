// 루트 액션.
#pragma once

#include "action.h"


namespace ai
{
	class IArtificialIntelligence;

	class cRootAction : public cAction
	{
	public:
		cRootAction(IArtificialIntelligence *obj)
			: cAction(obj, "Root", "", ACTION_TYPE::ROOT)
		{
		}
	};

}
