// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LostAge.h"
#include "LostAgeGameMode.h"
#include "LostAgeHUD.h"
#include "LostAgeCharacter.h"
#include "LostAgePlayerController.h"
#include "LostAgeMainMenuWidget.h"

ALostAgeGameMode::ALostAgeGameMode()
: Super()
{
	if (UWorld* world = GetWorld())
	{
		if (world->GetName().Equals(FString("MainMenu")))
		{
			DefaultPawnClass = APawn::StaticClass();
			PlayerControllerClass = APlayerController::StaticClass();
		}
		else
			SetupGameModeWithCustomClasses();
	}
	else
		SetupGameModeWithCustomClasses();
}

void ALostAgeGameMode::SetupGameModeWithCustomClasses()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/t"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	_pawnClass = PlayerPawnClassFinder.Class;
	PlayerControllerClass = ALostAgePlayerController::StaticClass();


	// use our custom HUD class
	HUDClass = ALostAgeHUD::StaticClass();
}

//void ALostAgeGameMode::PostLogin(APlayerController* playerController)
//{
//	Super::PostLogin(playerController);
//
//	if (UWorld* world = GetWorld())
//	{
//		if (Role == ROLE_Authority)
//		{
//			//if (!world->GetName().Equals(FString("MainMenu")))
//			//{
//				if (_playerStart.Num() == 0)
//				{
//					for (TActorIterator<APlayerStart> ActorItr(world); ActorItr; ++ActorItr)
//						_playerStart.Add(*ActorItr);
//				}
//				
//				if (_playerStart.Num() != 0)
//					RespawnPlayerForController(playerController);
//			//}
//		}
//		
//	}
//}

void ALostAgeGameMode::BeginPlay()
{
	Super::BeginPlay();
	UWorld* world = GetWorld();
	if (world && world->GetName().Equals(FString("MainMenu")))
	{
		APlayerController* pc = world->GetFirstPlayerController();
		pc->bShowMouseCursor = true;
		pc->bEnableClickEvents = true;
		pc->bEnableMouseOverEvents = true;
		DisableInput(pc);
		
		TSubclassOf<ULostAgeMainMenuWidget> mainMenuWidgetClass = LoadClass<ULostAgeMainMenuWidget>(nullptr, TEXT("/Game/FirstPerson/Menus/MainMenu.MainMenu_C"), nullptr, LOAD_None, nullptr);

		if (mainMenuWidgetClass)
		{
			ULostAgeMainMenuWidget* mainMenuWidget = CreateWidget<ULostAgeMainMenuWidget>(GetWorld(), mainMenuWidgetClass);
			mainMenuWidget->AddToViewport();
		}
	}
}


//void ALostAgeGameMode::RespawnPlayerForController_Implementation(APlayerController* controller)
//{
//	if (UWorld* world = GetWorld())
//	{
//		if (Role == ROLE_Authority)
//		{
//			APawn* controlledPawn = controller->GetPawn();
//			if (controlledPawn)
//			{
//				controlledPawn->Destroy();
//			}
//
//			FVector spawnLocation = FVector(0);
//			if (_playerStart.Num() != 0)
//				spawnLocation = _playerStart[0]->GetActorLocation();
//
//			FActorSpawnParameters spawnParams = FActorSpawnParameters();
//			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
//
//			ALostAgeCharacter* newPawn = world->SpawnActor<ALostAgeCharacter>(_pawnClass,spawnLocation, FRotator(0),spawnParams);
//			if (newPawn)
//				controller->Possess(newPawn);
//		}
//	}
//}
//
//bool ALostAgeGameMode::RespawnPlayerForController_Validate(APlayerController* controller)
//{
//	if (controller)
//		return true;
//	else
//		return false;
//}