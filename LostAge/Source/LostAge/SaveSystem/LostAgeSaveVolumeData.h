#pragma once 

#include "LostAge.h"
#include "LostAgeSaveVolumeData.generated.h"

USTRUCT()
struct FLostAgeSaveVolumeData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool _isAlreadyActivated;

	UPROPERTY()
	bool _loadFromfile;

	FLostAgeSaveVolumeData()
	{
		_isAlreadyActivated = false;
		_loadFromfile = false;
	}
};

FORCEINLINE FArchive &operator << (FArchive &archive, FLostAgeSaveVolumeData& data)
{
	archive << data._isAlreadyActivated;
	archive << data._loadFromfile;

	return archive;
}