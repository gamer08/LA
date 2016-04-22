#pragma once 

#include "LostAge.h"
#include "LostAgeCharacterSaveData.generated.h"

USTRUCT()
struct FLostAgeCharacterSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector _location;

	UPROPERTY()
	FRotator _rotation;

	UPROPERTY()
	bool _loadFromfile;

	FLostAgeCharacterSaveData()
	{
		_location = FVector();
		_rotation = FRotator();

		_loadFromfile = false;
	}
};

FORCEINLINE FArchive &operator << (FArchive &archive, FLostAgeCharacterSaveData& data)
{
	archive << data._location;
	archive << data._rotation;
	archive << data._loadFromfile;

	return archive;
}