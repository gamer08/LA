// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LostAge.h"
#include "LostAgeGameMode.h"
#include "LostAgeHUD.h"
#include "LostAgeCharacter.h"
#include "LostAgePlayerController.h"
#include "LostAgeMainMenuWidget.h"
#include "LostAgeGameInstance.h"

ALostAgeGameMode::ALostAgeGameMode()
: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/t"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	PlayerControllerClass = ALostAgePlayerController::StaticClass();
	HUDClass = ALostAgeHUD::StaticClass();


	static ConstructorHelpers::FClassFinder<ALostAgeCharacter> elf(TEXT("/Game/FirstPersonCPP/Blueprints/elf"));
	static ConstructorHelpers::FClassFinder<ALostAgeCharacter> dwarf(TEXT("/Game/FirstPersonCPP/Blueprints/dwarf"));

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