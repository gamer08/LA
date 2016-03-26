// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeCharacterDwarf.h"
#include "LostAgeGameInstance.h"
#include "LostAgePlayerController.h"
#include "LostAgePlayerCameraManager.h"
#include "SaveSystem/LostAgeSaveManager.h"

ALostAgeCharacterDwarf::ALostAgeCharacterDwarf()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		_playableClassName = FString("Dwarf");
		_cameraLocation = FVector(0.0f, 0.0f, 64.0f);
	}
}

void ALostAgeCharacterDwarf::BeginPlay()
{
	Super::BeginPlay();

	if (this->GetClass()->ImplementsInterface(ULostAgeSaveInterface::StaticClass()))
		Load();
}

void ALostAgeCharacterDwarf::Save(FLostAgeSaveData& saveData)
{
	FLostAgeDwarfSaveData dataToSave;

	dataToSave._loadFromfile = true;
	dataToSave._location = GetActorLocation();
	dataToSave._rotation = _saveRotation;

	FLostAgeCameraSaveData cameraData;
	
	cameraData._loadFromfile = true;
	cameraData._rotation = _saveCameraRotation;
	dataToSave._cameraData = cameraData;

	saveData.AddDataToSave(dataToSave);
}

void ALostAgeCharacterDwarf::Load()
{	
	if (!GetName().IsEmpty())
	{
		if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
		{
			if (ULostAgeSaveManager* saveManager = gameInstance->GetSaveManager())
			{
				FLostAgeDwarfSaveData savedData = saveManager->GetDataFromSave<FLostAgeDwarfSaveData>(GetName());

				if (savedData._loadFromfile)
				{
					SetActorLocation(savedData._location);
					_saveRotation = savedData._rotation;

					ALostAgePlayerController* pc = Cast<ALostAgePlayerController>(GetController());
					if (pc)
					{
						pc->SetControlRotation(_saveRotation);
						ALostAgePlayerCameraManager* camera = Cast<ALostAgePlayerCameraManager>(pc->PlayerCameraManager);
						camera->SetRotation(savedData._cameraData._rotation);
					}
				}
			}
		}
	}
}