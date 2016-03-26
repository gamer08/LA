#pragma once

#include "LostAgeEnum.generated.h"

UENUM(BlueprintType)
namespace EObserverEvent
{
	enum Type
	{
		HELLO,
		EVENTON,
		EVENTOFF
	};
}