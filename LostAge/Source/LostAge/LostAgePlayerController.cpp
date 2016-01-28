// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgePlayerController.h"
#include "LostAgePlayerCameraManager.h"
#include "LostAgeGameInstance.h"
#include "GameFramework/InputSettings.h"

ALostAgePlayerController::ALostAgePlayerController()
{
	PlayerCameraManagerClass = ALostAgePlayerCameraManager::StaticClass();
	_cameraRotationSpeed = 100.0f;
}

void ALostAgePlayerController::Possess(APawn* pawn)
{
	// cast plus rapide, mais lance une exception si null - dans le cas présent je suis toujours sur que pawn est un APawn donc ca va
	_pawn = CastChecked<ALostAgeCharacter>(pawn);

	Super::Possess(pawn);
}

void ALostAgePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly inputMode = FInputModeGameOnly();
	this->SetInputMode(inputMode);
}

void ALostAgePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Jump", IE_Pressed, this, &ALostAgePlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ALostAgePlayerController::StopJumping);

	InputComponent->BindAction("LeaveGame", IE_Pressed, this, &ALostAgePlayerController::LeaveToMainMenu);

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
		//if (value != 0.0f)
		//{
			Cast<ALostAgePlayerCameraManager>(PlayerCameraManager)->UpdateYaw(value * GetWorld()->GetDeltaSeconds() * _cameraRotationSpeed);
			CastChecked<ALostAgeCharacter>(pawn)->Turn(value * GetWorld()->GetDeltaSeconds() * _cameraRotationSpeed);
		//}
	}
}

void ALostAgePlayerController::LookUp(float value)
{
	// Le -1 en début de paramètres est à remplacer par une variable pour donner le choix au joueur plus tard dans les options.

	if (APawn* pawn = GetPawn())
	{
		//if (value != 0)
		//{
			Cast<ALostAgePlayerCameraManager>(PlayerCameraManager)->UpdatePitch(-1 * value * GetWorld()->GetDeltaSeconds() * _cameraRotationSpeed);
			CastChecked<ALostAgeCharacter>(pawn)->LookUp(-1 * value * GetWorld()->GetDeltaSeconds() * _cameraRotationSpeed);
		//}
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
	if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
	{
		gameInstance->LoadMainMenu(this);
	}
}

