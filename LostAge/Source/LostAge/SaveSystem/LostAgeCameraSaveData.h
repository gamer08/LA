#pragma once 

#include "LostAge.h"
#include "LostAgeCameraSaveData.generated.h"

USTRUCT()
struct FLostAgeCameraSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FRotator _rotation;

	UPROPERTY()
	bool _loadFromfile;

	FLostAgeCameraSaveData()
	{
		_rotation = FRotator();
		_loadFromfile = false;
	}
};

FORCEINLINE FArchive &operator << (FArchive &archive, FLostAgeCameraSaveData& data)
{
	archive << data._rotation;
	archive << data._loadFromfile;

	return archive;
}