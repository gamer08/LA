#pragma once 

#include "LostAge.h"
#include "LostAgeCameraSaveData.h"
#include "LostAgeAxeDwarfSaveData.h"
#include "LostAgeDwarfSaveData.generated.h"

USTRUCT()
struct FLostAgeDwarfSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector _location;

	UPROPERTY()
	FRotator _rotation;

	UPROPERTY()
	FLostAgeCameraSaveData _cameraData;

	UPROPERTY()
	FLostAgeAxeDwarfSaveData _axeData;

	UPROPERTY()
	bool _loadFromfile;

	FLostAgeDwarfSaveData()
	{
		_location = FVector();
		_rotation = FRotator();
		_cameraData = FLostAgeCameraSaveData();
		_axeData = FLostAgeAxeDwarfSaveData();

		_loadFromfile = false;
	}
};

FORCEINLINE FArchive &operator << (FArchive &archive, FLostAgeDwarfSaveData& data)
{
	archive << data._location;
	archive << data._rotation;
	archive << data._cameraData;
	archive << data._axeData;
	archive << data._loadFromfile;

	return archive;
}