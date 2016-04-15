#pragma once 

#include "LostAge.h"
#include "LostAgeDwarfSaveData.h"
#include "LostAgeElfSaveData.h"
//#include "LostAgeCameraSaveData.h"
#include "LostAgeSaveVolumeData.h"
#include "LostAgeCubeElfSaveData.h"

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

	/*UPROPERTY()
	TMap<FString,FLostAgeCameraSaveData> _camerasData;*/

	UPROPERTY()
	TMap<FString,FLostAgeSaveVolumeData> _saveVolumesData;

	UPROPERTY()
	FLostAgeCubeElfSaveData _cubeElfData;

	FLostAgeSaveData()
	{
		_levelName = FString();
		_dwarfData = FLostAgeDwarfSaveData();
		_elfData = FLostAgeElfSaveData();
	/*	_camerasData.Reset();*/
		_saveVolumesData.Reset();
		_cubeElfData = FLostAgeCubeElfSaveData();
	}
	
	void AddDataToSave(FLostAgeDwarfSaveData data)
	{
		_dwarfData = data;
	}

	void AddDataToSave(FLostAgeElfSaveData data)
	{
		_elfData = data;
	}

	//void AddDataToSave(FString name, FLostAgeCameraSaveData data)
	//{
	//	_camerasData.Emplace(name, data);
	//}

	void AddDataToSave(FString name, FLostAgeSaveVolumeData data)
	{
		_saveVolumesData.Emplace(name, data);
	}

	void AddDataToSave(FLostAgeCubeElfSaveData data)
	{
		_cubeElfData = data;
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
	
	template <> inline FLostAgeCubeElfSaveData GetDataFromSave<FLostAgeCubeElfSaveData>(FString name)
	{
		return _cubeElfData;
	}

	//template <> inline FLostAgeCameraSaveData GetDataFromSave<FLostAgeCameraSaveData>(FString name)
	//{
	//	FLostAgeCameraSaveData data = FLostAgeCameraSaveData();
	//	FLostAgeCameraSaveData* lData = _camerasData.Find(name);
	//	
	//	if (lData)
	//		data = *lData;
	//	
	//	return data;
	//}

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
	/*archive << data._camerasData;*/
	archive << data._saveVolumesData;
	archive << data._cubeElfData;

	return archive;
}