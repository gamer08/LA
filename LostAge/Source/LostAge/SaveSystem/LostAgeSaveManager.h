// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LostAgeSaveData.h"
#include "LostAgeSaveManager.generated.h"

UCLASS()
class LOSTAGE_API ULostAgeSaveManager : public UObject
{
	GENERATED_BODY()
	
public:
	ULostAgeSaveManager();
	
	bool Save();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool Load();
	
	inline bool IsASaveLoaded() const
	{
		return _isASaveLoaded;
	}

	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool IsASaveWasFound() const
	{
		return _isASaveWasFound;
	}

	UFUNCTION(BlueprintCallable, Category = "Save System")
	FName GetSaveFileLevelName() const
	{
		return _saveFileLevelName;
	}


	FString GetSaveFile();

	template <typename T>
	T GetDataFromSave(FString name)
	{
		return _currentCachedData.GetDataFromSave(name);
	}

	template <> inline FLostAgeDwarfSaveData GetDataFromSave<FLostAgeDwarfSaveData>(FString name)
	{
		return _currentCachedData.GetDataFromSave<FLostAgeDwarfSaveData>(name);
	}

	template <> inline FLostAgeElfSaveData GetDataFromSave<FLostAgeElfSaveData>(FString name)
	{
		return _currentCachedData.GetDataFromSave<FLostAgeElfSaveData>(name);
	}

	template <> inline FLostAgeSaveVolumeData GetDataFromSave<FLostAgeSaveVolumeData>(FString name)
	{
		return _currentCachedData.GetDataFromSave<FLostAgeSaveVolumeData>(name);
	}

	void FlushCachedSaveData();

private:

	UPROPERTY()
	FLostAgeSaveData _currentCachedData;

	UPROPERTY()
	FString _saveDirectory;

	bool _isASaveLoaded;
	bool _isASaveWasFound;

	void SaveNLoadData(FArchive& archive, FLostAgeSaveData& data);

	bool LoadInternal();

	UPROPERTY()
	FString _saveFilePath;

	UPROPERTY()
	FName _saveFileLevelName;
};