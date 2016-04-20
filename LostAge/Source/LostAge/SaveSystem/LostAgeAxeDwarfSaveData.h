#pragma once 

#include "LostAge.h"
#include "LostAgeAxeDwarfSaveData.generated.h"

USTRUCT()
struct FLostAgeAxeDwarfSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector _location;

	UPROPERTY()
	FRotator _rotation;

	UPROPERTY()
	bool _loadFromfile;

	FLostAgeAxeDwarfSaveData()
	{
		_location = FVector();
		_rotation = FRotator();
		_loadFromfile = false;
	}
};

FORCEINLINE FArchive &operator << (FArchive &archive, FLostAgeAxeDwarfSaveData& data)
{
	archive << data._location;
	archive << data._rotation;
	archive << data._loadFromfile;

	return archive;
}