
#pragma once

#include "LostAgeSaveData.h"
#include "LostAgeSaveInterface.generated.h"

UINTERFACE(MinimalAPI)
class ULostAgeSaveInterface : public UInterface
{
    GENERATED_UINTERFACE_BODY()
};

class ILostAgeSaveInterface
{    GENERATED_IINTERFACE_BODY()

protected:
	virtual void Load() = 0;

public:
	virtual void Save(FLostAgeSaveData& saveData) = 0;
};