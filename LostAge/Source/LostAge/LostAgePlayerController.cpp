// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgePlayerController.h"
#include "LostAgePlayerCameraManager.h"
#include "LostAgeGameInstance.h"
#include "LostAgeHUD.h"
#include "LostAgeGameMode.h"
#include "LostAgeSubject.h"
#include "GameFramework/InputSettings.h"

ALostAgePlayerController::ALostAgePlayerController()
{
	PlayerCameraManagerClass = ALostAgePlayerCameraManager::StaticClass();
	_cameraRotationSpeed = 100.0f;

	this->bShowMouseCursor = false;
	this->bEnableClickEvents = false;
	this->bEnableMouseOverEvents = false;
}

void ALostAgePlayerController::Possess(APawn* pawn)
{
	Super::Possess(pawn);
}

void ALostAgePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (world && !world->GetName().Equals(FString("MainMenu")))
	{
		this->bShowMouseCursor = false;
		this->bEnableClickEvents = false;
	    this->bEnableMouseOverEvents = false;
		FInputModeGameOnly inputMode = FInputModeGameOnly();
		this->SetInputMode(inputMode);
		ClientSetHUD(ALostAgeHUD::StaticClass());
	}
	else
	{
		this->bShowMouseCursor = true;
		this->bEnableClickEvents = true;
	    this->bEnableMouseOverEvents = true;
		FInputModeUIOnly inputMode = FInputModeUIOnly();
		this->SetInputMode(inputMode);
	}
}

void ALostAgePlayerController::SpawnPlayerCameraManager()
{
	Super::SpawnPlayerCameraManager();
}

void ALostAgePlayerController::ShowPauseMenu()
{	
	if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
	{
		GiveControlsToUI(true);
		gameInstance->CreatePauseMenu();
	}
}

void ALostAgePlayerController::GiveControlsToUI(bool value)
{
	this->bShowMouseCursor = value;
	this->bEnableClickEvents = value;
	this->bEnableMouseOverEvents = value;

	if (value)
	{
		FInputModeUIOnly inputMode = FInputModeUIOnly();
		this->SetInputMode(inputMode);
	}
	else
	{
		FInputModeGameOnly inputMode = FInputModeGameOnly();
		this->SetInputMode(inputMode);
	}
}

void ALostAgePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Jump", IE_Pressed, this, &ALostAgePlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ALostAgePlayerController::StopJumping);

	InputComponent->BindAction("ShowPauseMenu", IE_Pressed, this, &ALostAgePlayerController::ShowPauseMenu);

	InputComponent->BindAxis("MoveForward", this, &ALostAgePlayerController::MoveForward);
	InputComponent->BindAxis("MoveBackward", this, &ALostAgePlayerController::MoveForward);

	InputComponent->BindAxis("MoveLeft", this, &ALostAgePlayerController::MoveSide);
	InputComponent->BindAxis("MoveRight", this, &ALostAgePlayerController::MoveSide);

	InputComponent->BindAxis("Turn", this, &ALostAgePlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ALostAgePlayerController::LookUp);
}

void ALostAgePlayerController::MoveForward(float value)
{
	if ( APawn* pawn = GetPawn())
	{
		if (value != 0.0)
			CastChecked<ALostAgeCharacter>(pawn)->MoveForward(value);
	}
}

void ALostAgePlayerController::MoveSide(float value)
{
	if (APawn* pawn = GetPawn())
	{
		if (value != 0.0)
			CastChecked<ALostAgeCharacter>(pawn)->MoveSide(value);
	}
}

void ALostAgePlayerController::Turn(float value)
{
	if (APawn* pawn = GetPawn())
	{
		if (value != 0.0f)
		{
			Cast<ALostAgePlayerCameraManager>(PlayerCameraManager)->UpdateYaw(value * GetWorld()->GetDeltaSeconds() * _cameraRotationSpeed * InputYawScale);
			FRotator cameraRotation = Cast<ALostAgePlayerCameraManager>(PlayerCameraManager)->GetRotation();

			AddYawInput(value * GetWorld()->GetDeltaSeconds() * _cameraRotationSpeed);

			ALostAgeCharacter* character = Cast<ALostAgeCharacter>(pawn);
			character->UpdateSaveRotation(GetControlRotation());
			character->UpdateSaveCameraRotation(cameraRotation);
		}
	}
}

void ALostAgePlayerController::LookUp(float value)
{
	// Le -1 en début de paramètres est à remplacer par une variable pour donner le choix au joueur plus tard dans les options.

	if (APawn* pawn = GetPawn())
	{
		if (value != 0.0f)
		{
			Cast<ALostAgePlayerCameraManager>(PlayerCameraManager)->UpdatePitch(value * GetWorld()->GetDeltaSeconds() * _cameraRotationSpeed * InputPitchScale);
			FRotator cameraRotation = Cast<ALostAgePlayerCameraManager>(PlayerCameraManager)->GetRotation();
			
			AddPitchInput(value * GetWorld()->GetDeltaSeconds() * _cameraRotationSpeed);
			
			ALostAgeCharacter* character = Cast<ALostAgeCharacter>(pawn);
			character->UpdateSaveRotation(GetControlRotation());
			character->UpdateSaveCameraRotation(cameraRotation);
		}
	}
}

void ALostAgePlayerController::Jump()
{
	if (APawn* pawn = GetPawn())
		CastChecked<ALostAgeCharacter>(pawn)->Jump();
}

void ALostAgePlayerController::StopJumping()
{
	if (APawn* pawn = GetPawn())
		CastChecked<ALostAgeCharacter>(pawn)->StopJumping();
}

void ALostAgePlayerController::LeaveToMainMenu()
{
	RequestReleasePlayableClass(this);
	if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
	{
		if (HasAuthority())
			SendLeaveOrderToEveryone();
		else
			gameInstance->LoadMainMenu(this);
	}
}

void ALostAgePlayerController::RequestReleasePlayableClass_Implementation(AController* controller)
{
	if (HasAuthority())
	{
		if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
		{
			gameInstance->ReleasePlayableClass(controller);
		}
	}
}

bool ALostAgePlayerController::RequestReleasePlayableClass_Validate(AController* controller)
{
	return true;
}

void ALostAgePlayerController::SendLeaveOrderToEveryone_Implementation()
{
	if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
		gameInstance->LoadMainMenu(this);
}

bool ALostAgePlayerController::SendLeaveOrderToEveryone_Validate()
{
	return true;
}

FVector ALostAgePlayerController::GetTargetCameraLocation() const
{
	if (ALostAgeCharacter* pawn = Cast<ALostAgeCharacter>(GetPawn()))
		return pawn->GetCameraLocation();
	else
		return FVector(0.0f, 0.0f, 64.0f);
}

void ALostAgePlayerController::RequestLoadNextMap_Implementation(const FString& levelName)
{
	if (HasAuthority())
	{
		if (UWorld* world = GetWorld())
		{
			FString UrlString = "/Game/FirstPersonCPP/Maps/" + levelName + "?listen"; 
			world->ServerTravel(UrlString,true);
		}
	}
}

bool ALostAgePlayerController::RequestLoadNextMap_Validate(const FString& levelName)
{
	return true;
}

void ALostAgePlayerController::NeedSubjectNotify_Implementation(ULostAgeSubject* subjectToNotify, EObserverEvent::Type evnt)
{
	if (HasAuthority())
		subjectToNotify->NotifyAll(evnt);
}

bool ALostAgePlayerController::NeedSubjectNotify_Validate(ULostAgeSubject* subjectToNotify, EObserverEvent::Type evnt)
{
	return true;
}

void ALostAgePlayerController::ActivateSubject_Implementation(ULostAgeSubject* subjectToActivate)
{
	if (HasAuthority())
		subjectToActivate->SubjectActivate();
}

bool ALostAgePlayerController::ActivateSubject_Validate(ULostAgeSubject* subjectToActivate)
{
	return true;
}

void ALostAgePlayerController::DesactivateSubject_Implementation(ULostAgeSubject* subjectToDesactivate)
{
	if (HasAuthority())
		subjectToDesactivate->SubjectDesactivate();
}

bool ALostAgePlayerController::DesactivateSubject_Validate(ULostAgeSubject* subjectToDesactivate)
{
	return true;
}

void ALostAgePlayerController::LoadSaveOnClient_Implementation(const FString& playerClass)
{	
	if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
	{
		if (ULostAgeSaveManager* saveManager = gameInstance->GetSaveManager())
		{
			bool isLoadSuccessfull = saveManager->Load();
			if (isLoadSuccessfull)
			{
				if (playerClass.Equals(FString("Elf")))
					SetControllerSavedInfo(saveManager->GetDataFromSave<FLostAgeElfSaveData>(GetName()));

				if (playerClass.Equals(FString("Dwarf")))
					SetControllerSavedInfo(saveManager->GetDataFromSave<FLostAgeDwarfSaveData>(GetName()));
			}
		}
	}
}

bool ALostAgePlayerController::LoadSaveOnClient_Validate(const FString& playerClass)
{
	return true;
}

void ALostAgePlayerController::SetControllerSavedInfo(FLostAgeElfSaveData data)
{
	SetControlRotation(data._rotation);

	if (ALostAgePlayerCameraManager* camera = Cast<ALostAgePlayerCameraManager>(PlayerCameraManager))
		camera->SetRotation(data._cameraData._rotation);
}

void ALostAgePlayerController::SetControllerSavedInfo(FLostAgeDwarfSaveData data)
{
	SetControlRotation(data._rotation);

	if (ALostAgePlayerCameraManager* camera = Cast<ALostAgePlayerCameraManager>(PlayerCameraManager))
		camera->SetRotation(data._cameraData._rotation);
}