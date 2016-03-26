// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeSaveManager.h"
#include "LostAgeSaveInterface.h"
#include "EngineUtils.h"
#include <cstdlib>
#include <string>
#include <ctime>

ULostAgeSaveManager::ULostAgeSaveManager()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		_isASaveLoaded = false;

		std::string userProfile, saveDirectory;
		userProfile.clear();
		saveDirectory.clear();
		
		userProfile = std::getenv("USERPROFILE");
		saveDirectory = "\\Documents\\LostAge\\Saves";
		userProfile.append(saveDirectory);
		
		_saveDirectory = FString(userProfile.c_str());
		
		FString saveFileFound = GetSaveFile();
		
		if (saveFileFound.IsEmpty())
			_saveFilePath = _saveDirectory + "\\" + "LostAge.save";
		else
			_saveFilePath = _saveDirectory + "\\" + saveFileFound;
		
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_saveDirectory))
			FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*_saveDirectory);
	}
}

FString ULostAgeSaveManager::GetSaveFile()
{
	TArray<FString> saveFilesfound;
	FString saveFile = FString();
	
	FString pathNFilter = _saveDirectory + "/" + "*.save";
	IFileManager::Get().FindFiles(saveFilesfound, *pathNFilter, true, true);

	if (saveFilesfound.Num() != 0)
	{
		_isASaveWasFound = true;
		saveFile = saveFilesfound[0];
	}

	return saveFile;
}

void ULostAgeSaveManager::SaveNLoadData(FArchive& archive, FLostAgeSaveData& data)
{
	archive << data;
}

bool ULostAgeSaveManager::Save(/*FString saveName*/)
{
	bool saveSuccessfull = false;

	//FString saveFileName;

	//if (saveName.IsEmpty())
	//	saveFileName = GenerateSaveFileName();
	//else
	//	saveFileName = saveName;

	//FString saveFilePath = _saveDirectory + "\\" + saveFileName;
	
	FLostAgeSaveData saveData = FLostAgeSaveData();

	UWorld* world = GetOuter()->GetWorld();

	if (world)
	{
		saveData._levelName = world->GetName();

		for (TObjectIterator<UObject> obj; obj; ++obj)
		{
			if (obj 
				&& obj->GetClass()->ImplementsInterface(ULostAgeSaveInterface::StaticClass())
				&& !obj->HasAnyFlags( RF_ClassDefaultObject | RF_ArchetypeObject )
				&& !obj->GetOuter()->HasAnyFlags( RF_ClassDefaultObject | RF_ArchetypeObject ))
			{
				Cast<ILostAgeSaveInterface>(*obj)->Save(saveData);
			}	
		}

		FBufferArchive toSave = FBufferArchive();

		SaveNLoadData(toSave, saveData);

		if (toSave.Num() <= 0)
			return false;

		if (FFileHelper::SaveArrayToFile(toSave, *_saveFilePath))
		{
			_isASaveWasFound = true;
			saveSuccessfull = true;
		}
			

		toSave.FlushCache();
		toSave.Empty();
		toSave.Close();
	}
	return saveSuccessfull;
}

bool ULostAgeSaveManager::Load(/*FString SaveToLoad*/)
{
	bool loadSuccessfull = false;

	_isASaveLoaded = LoadInternal(/*SaveToLoad*/);

	if (_isASaveLoaded)
	{
		UWorld* world = GetOuter()->GetWorld();
		if (world)
		{
			_saveFileLevelName = FName(*_currentCachedData._levelName);
			loadSuccessfull = true;
			//UGameplayStatics::OpenLevel(world, FName(*_currentCachedData._levelName));
		}
	}

	return loadSuccessfull;
}

bool ULostAgeSaveManager::LoadInternal(/*FString SaveToLoad*/)
{
	bool loadSuccessfull = false;

	TArray<uint8> binaryData;

	_currentCachedData = FLostAgeSaveData();

	/*FString saveFilePath = _saveDirectory + "\\" + SaveToLoad;*/

	if ((FFileHelper::LoadFileToArray(binaryData, *_saveFilePath)) && (binaryData.Num() >0))
	{
		FMemoryReader toData = FMemoryReader(binaryData, true);
		toData.Seek(0);
		SaveNLoadData(toData, _currentCachedData);

		toData.FlushCache();
		binaryData.Empty();
		
		loadSuccessfull = true;
	}

	return loadSuccessfull;
}

//FString ULostAgeSaveManager::GenerateSaveFileName()
//{
//	time_t nowT = time(0);
//	struct tm* now = localtime(&nowT);
//	char buffer[100];
//	strftime(buffer, sizeof(buffer), "%d-%m-%Y %Hh%Mm%S", now);
//	
//	return FString("BRN ").Append(buffer).Append(".save"); 
//}

void ULostAgeSaveManager::FlushCachedSaveData()
{
	_isASaveLoaded = false;
	_currentCachedData = FLostAgeSaveData();
}