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

	if (!_isAxeInHand)
	{
		FVector l = _axeDwarf->GetActorLocation();
		FRotator r = _axeDwarf->GetActorRotation();

		if (_axeDwarf)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("axe location: x: %f, y: %f, z: %f"), l.X, l.Y, l.Z));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("axe rotation: y: %f, p: %f, r: %f"), r.Yaw, r.Pitch, r.Roll));

			FLostAgeAxeDwarfSaveData axeData;
			axeData._loadFromfile = true;
			axeData._location = _axeDwarf->GetActorLocation();
			axeData._rotation = _axeDwarf->GetActorRotation();
			dataToSave._axeData = axeData;
		}
		
		
	}

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
				
				/*else
				{
					ALostAgePlayerController* pc = Cast<ALostAgePlayerController>(GetController());
					FRotator rot = pc->GetControlRotation();
					ALostAgePlayerCameraManager* camera = Cast<ALostAgePlayerCameraManager>(pc->PlayerCameraManager);
					camera->SetRotation(rot);
				}*/
				
				SpawnAxe(savedData._axeData._location, savedData._axeData._rotation, savedData._axeData._loadFromfile);

			}
		}
	}
}

void ALostAgeCharacterDwarf::UpdateisInHand_Implementation(bool value)
{
	_isAxeInHand = value;
}

bool ALostAgeCharacterDwarf::UpdateisInHand_Validate(bool value)
{
	return true;
}

void ALostAgeCharacterDwarf::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ALostAgeCharacterDwarf, _axeDwarf);
	DOREPLIFETIME(ALostAgeCharacterDwarf, _isAxeInHand);
}