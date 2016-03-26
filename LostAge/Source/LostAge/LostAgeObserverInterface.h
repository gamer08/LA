
#pragma once

#include "LostAgeEnum.h"
#include "LostAgeObserverInterface.generated.h"

class ULostAgeSubject;
UINTERFACE(MinimalAPI)
class ULostAgeObserverInterface : public UInterface
{
    GENERATED_UINTERFACE_BODY()
};

class ILostAgeObserverInterface
{    
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION()
	virtual void OnNotify(ULostAgeSubject* subject, EObserverEvent::Type call) = 0;
};