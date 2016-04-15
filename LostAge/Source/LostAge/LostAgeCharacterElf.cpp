// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeCharacterElf.h"
#include "LostAgeGameInstance.h"
#include "LostAgePlayerController.h"
#include "LostAgePlayerCameraManager.h"
#include "SaveSystem/LostAgeSaveManager.h"

ALostAgeCharacterElf::ALostAgeCharacterElf()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		_playableClassName = FString("Elf");
		_cameraLocation = FVector(0.0f, 0.0f, 64.0f);
		
		static ConstructorHelpers::FClassFinder<ALostAgeCubeElf> cube(TEXT("/Game/FirstPersonCPP/Blueprints/Elf/CubeElf"));
		_cubeBlueprintClass = cube.Class;
	}
}

void ALostAgeCharacterElf::BeginPlay()
{
	Super::BeginPlay();

	if (this->GetClass()->ImplementsInterface(ULostAgeSaveInterface::StaticClass()))
		Load();
}

void ALostAgeCharacterElf::Save(FLostAgeSaveData& saveData)
{
	FLostAgeElfSaveData dataToSave;

	dataToSave._loadFromfile = true;
	dataToSave._location = GetActorLocation();
	dataToSave._rotation = _saveRotation;

	FLostAgeCameraSaveData cameraData;
	
	cameraData._loadFromfile = true;
	cameraData._rotation = _saveCameraRotation;
	dataToSave._cameraData = cameraData;

	if (_cubeElf)
	{
		/*FVector l = _cubeElf->GetActorLocation();
		FRotator r = _cubeElf->GetActorRotation();

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("cube location: x: %f, y: %f, z: %f"), l.X, l.Y, l.Z));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("cube rotation: y: %f, p: %f, r: %f"), r.Yaw, r.Pitch, r.Roll));*/
		
		FLostAgeCubeElfSaveData cubeData;
	
		cubeData._loadFromfile = true;
		cubeData._location = _cubeElf->GetActorLocation();
		cubeData._rotation = _cubeElf->GetActorRotation();
		cubeData._scale = _cubeElf->GetActorScale3D();
		dataToSave._cubeData = cubeData;
	}
	
	saveData.AddDataToSave(dataToSave);
}

void ALostAgeCharacterElf::Load()
{
	if (!GetName().IsEmpty())
	{
		if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
		{
			if (ULostAgeSaveManager* saveManager = gameInstance->GetSaveManager())
			{
				FLostAgeElfSaveData savedData = saveManager->GetDataFromSave<FLostAgeElfSaveData>(GetName());

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

					if (savedData._cubeData._loadFromfile)
					{
						if (HasAuthority())
							RequestSpawnCubeOnLoadServer(savedData._cubeData);
					}
				}
			}
		}
	}
}

void ALostAgeCharacterElf::RequestSpawnCubeOnLoadServer_Implementation(FLostAgeCubeElfSaveData cubeData)
{
	_cubeElf = GetWorld()->SpawnActor<ALostAgeCubeElf>(_cubeBlueprintClass, cubeData._location, cubeData._rotation);
	_cubeElf->SetActorScale3D(cubeData._scale);
	_cubeElf->SetOwner(this);
	SetOwner();
}

bool ALostAgeCharacterElf::RequestSpawnCubeOnLoadServer_Validate(FLostAgeCubeElfSaveData cubeData)
{
	return true;
}

void ALostAgeCharacterElf::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ALostAgeCharacterElf, _cubeElf);
}