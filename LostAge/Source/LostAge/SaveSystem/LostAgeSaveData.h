#pragma once 

#include "LostAge.h"
#include "LostAgeDwarfSaveData.h"
#include "LostAgeElfSaveData.h"
#include "LostAgeSaveVolumeData.h"

#include "LostAgeSaveData.generated.h"

USTRUCT()
struct FLostAgeSaveData
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY()
	FString _levelName;

	UPROPERTY()
	FLostAgeDwarfSaveData _dwarfData;

	UPROPERTY()
	FLostAgeElfSaveData _elfData;

	UPROPERTY()
	TMap<FString,FLostAgeSaveVolumeData> _saveVolumesData;

	FLostAgeSaveData()
	{
		_levelName = FString();
		_dwarfData = FLostAgeDwarfSaveData();
		_elfData = FLostAgeElfSaveData();
		_saveVolumesData.Reset();
	}
	
	void AddDataToSave(FLostAgeDwarfSaveData data)
	{
		_dwarfData = data;
	}

	void AddDataToSave(FLostAgeElfSaveData data)
	{
		_elfData = data;
	}

	void AddDataToSave(FString name, FLostAgeSaveVolumeData data)
	{
		_saveVolumesData.Emplace(name, data);
	}

	template <typename T>
	T GetDataFromSave(FString name);

	template <> inline FLostAgeDwarfSaveData GetDataFromSave<FLostAgeDwarfSaveData>(FString name)
	{
		return _dwarfData;
	}

	template <> inline FLostAgeElfSaveData GetDataFromSave<FLostAgeElfSaveData>(FString name)
	{
		return _elfData;
	}
	
	template <> inline FLostAgeSaveVolumeData GetDataFromSave<FLostAgeSaveVolumeData>(FString name)
	{
		FLostAgeSaveVolumeData data = FLostAgeSaveVolumeData();
		FLostAgeSaveVolumeData* lData = _saveVolumesData.Find(name);
		
		if (lData)
			data = *lData;
		
		return data;
	}
};

FORCEINLINE FArchive &operator << (FArchive &archive, FLostAgeSaveData& data)
{
	archive << data._levelName;
	archive << data._dwarfData;
	archive << data._elfData;
	archive << data._saveVolumesData;

	return archive;
}