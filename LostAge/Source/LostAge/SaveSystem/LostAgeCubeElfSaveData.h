#pragma once 

#include "LostAge.h"
#include "LostAgeCubeElfSaveData.generated.h"

USTRUCT()
struct FLostAgeCubeElfSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector _location;

	UPROPERTY()
	FRotator _rotation;

	UPROPERTY()
	FVector _scale;

	UPROPERTY()
	bool _loadFromfile;

	FLostAgeCubeElfSaveData()
	{
		_location = FVector();
		_rotation = FRotator();
		_scale = FVector();
		_loadFromfile = false;
	}
};

FORCEINLINE FArchive &operator << (FArchive &archive, FLostAgeCubeElfSaveData& data)
{
	archive << data._location;
	archive << data._rotation;
	archive << data._scale;
	archive << data._loadFromfile;

	return archive;
}