// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LostAge.h"
#include "LostAgeGameMode.h"
#include "LostAgeHUD.h"
#include "LostAgeCharacter.h"
#include "LostAgePlayerController.h"
#include "LostAgeMainMenuWidget.h"
#include "LostAgeGameInstance.h"
#include "LostAgePlayerCameraManager.h"

ALostAgeGameMode::ALostAgeGameMode()
: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/t"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	PlayerControllerClass = ALostAgePlayerController::StaticClass();
	HUDClass = ALostAgeHUD::StaticClass();


	static ConstructorHelpers::FClassFinder<ALostAgeCharacter> elf(TEXT("/Game/FirstPersonCPP/Blueprints/elf"));
	static ConstructorHelpers::FClassFinder<ALostAgeCharacter> dwarf(TEXT("/Game/FirstPersonCPP/Blueprints/Dwarf/Bob"));

	_playableClassesReferences.Emplace(FString("Elf"), elf.Class);
	_playableClassesReferences.Emplace(FString("Dwarf"), dwarf.Class);
}

void ALostAgeGameMode::BeginPlay()
{
	Super::BeginPlay();
	UWorld* world = GetWorld();
	if (world && world->GetName().Equals(FString("MainMenu")))
	{
		TSubclassOf<ULostAgeMainMenuWidget> mainMenuWidgetClass = LoadClass<ULostAgeMainMenuWidget>(nullptr, TEXT("/Game/FirstPerson/Menus/MainMenu.MainMenu_C"), nullptr, LOAD_None, nullptr);

		if (mainMenuWidgetClass)
		{
			ULostAgeMainMenuWidget* mainMenuWidget = CreateWidget<ULostAgeMainMenuWidget>(GetWorld(), mainMenuWidgetClass);
			mainMenuWidget->AddToViewport();
		}
	}
}

void ALostAgeGameMode::PostLogin(APlayerController* playerController)
{
	Super::PostLogin(playerController);

	UWorld* world = GetWorld();
	if (world && !world->GetName().Equals(FString("MainMenu")))
	{
		if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
		{
			FString playerId = playerController->PlayerState->UniqueId.GetUniqueNetId().Get()->ToString();
			FLostPlayerClassInfo find = gameInstance->GetClassInfoByPlayerID(playerId);
			ULostAgeSaveManager* saveManager = gameInstance->GetSaveManager();

			if (find._ownerRole == PlayerRole::CLIENT && saveManager && saveManager->IsASaveLoaded())
			{
				if (ALostAgePlayerController* pc = Cast<ALostAgePlayerController>(playerController))
				{
					if (ALostAgeCharacter* character = Cast<ALostAgeCharacter>(pc->GetPawn()))
					{
						if (character->GetPlayableClassName().Equals(FString("Elf")))
							pc->SetControllerSavedInfo(saveManager->GetDataFromSave<FLostAgeElfSaveData>(character->GetName()));

						if (character->GetPlayableClassName().Equals(FString("Dwarf")))
							pc->SetControllerSavedInfo(saveManager->GetDataFromSave<FLostAgeDwarfSaveData>(character->GetName()));

						pc->LoadSaveOnClient(character->GetPlayableClassName());
					}
				}
			}
		}
	}
}

void ALostAgeGameMode::CreatePauseMenu()
{
	TSubclassOf<UUserWidget> pauseMenuWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/FirstPerson/Menus/PauseMenu.PauseMenu_C"), nullptr, LOAD_None, nullptr);

	if (pauseMenuWidgetClass)
	{
		UUserWidget* pauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), pauseMenuWidgetClass);
		pauseMenuWidget->AddToViewport();
	}
}

void ALostAgeGameMode::SpawnPlayerChossedClass(ALostAgePlayerController* controller, FString className)
{
	if (UWorld* world = GetWorld())
	{
		APawn* controlledPawn = controller->GetPawn();
		if (controlledPawn)
			controlledPawn->Destroy();

		ALostAgeCharacter* newPawn = world->SpawnActor<ALostAgeCharacter>(_playableClassesReferences[className]);
		if (newPawn)
			controller->Possess(newPawn);
	}
}

TSubclassOf<class ALostAgeCharacter> ALostAgeGameMode::GetClassReferenceByName(FString className)
{
	if (_playableClassesReferences.Contains(className))
		return _playableClassesReferences[className];
	else
		return nullptr;
}

UClass* ALostAgeGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (UWorld* world = GetWorld())
	{
		if (world->GetName().Equals(FString("MainMenu")))
			return Super::GetDefaultPawnClassForController_Implementation(InController);

		if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(world->GetGameInstance()))
		{
			FString id = InController->PlayerState->UniqueId.GetUniqueNetId().Get()->ToString();
			FString cl = gameInstance->GetPawnClass(id, InController);

			if (cl.Equals(FString("none")))
				return Super::GetDefaultPawnClassForController_Implementation(InController);
			else
				return _playableClassesReferences[cl];
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}