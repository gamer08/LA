#pragma once 

#include "LostAge.h"
#include "LostAgeCameraSaveData.h"
#include "LostAgeElfSaveData.generated.h"

USTRUCT()
struct FLostAgeElfSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector _location;

	UPROPERTY()
	FRotator _rotation;

	UPROPERTY()
	FLostAgeCameraSaveData _cameraData;

	UPROPERTY()
	bool _loadFromfile;

	FLostAgeElfSaveData()
	{
		_location = FVector();
		_rotation = FRotator();
		_cameraData = FLostAgeCameraSaveData();

		_loadFromfile = false;
	}
};

FORCEINLINE FArchive &operator << (FArchive &archive, FLostAgeElfSaveData& data)
{
	archive << data._location;
	archive << data._rotation;
	archive << data._cameraData;
	archive << data._loadFromfile;

	return archive;
}