// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LostAge.h"
#include "LostAgeCharacter.h"
#include "LostAgePlayerController.h"
#include "LostAgeGameInstance.h"
#include "SaveSystem/LostAgeSaveManager.h"
#include "LostAgeSaveVolume.h"
#include "Animation/AnimInstance.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

ALostAgeCharacter::ALostAgeCharacter()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

		_minPitch = -89.0f;
		_maxPitch = 89.0f;
		
		bUseControllerRotationYaw = true;
		bUseControllerRotationPitch = true;
		
		//Désactiver pour l'instant à réactiver au besoin
		PrimaryActorTick.bCanEverTick = false;
	}
}

void ALostAgeCharacter::BeginPlay()
{
	Super::BeginPlay();

	_characterSpeed = 1.0f;

	//FRotator rot = FRotator(0);
	//SetActorRotation(rot);

	//if (this->GetClass()->ImplementsInterface(ULostAgeSaveInterface::StaticClass()))
	//	Load();
}

void ALostAgeCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void ALostAgeCharacter::MoveForward(float value)
{		
	const FRotator Rotation = Cast<ALostAgePlayerController>(Controller)->GetCameraRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, value * _characterSpeed);
}

void ALostAgeCharacter::MoveSide(float value)
{
	const FRotator Rotation = Cast<ALostAgePlayerController>(Controller)->GetCameraRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, value * _characterSpeed);
}

void ALostAgeCharacter::Jump()
{
	ACharacter::Jump();
}

// si HasAuthority retourne true c'est la version serveur sinon client (Dans la pluspart des cas mais pas tout le temps)
void ALostAgeCharacter::StopJumping()
{
	/*if (HasAuthority())
		MultiCastStopJumpToClients();
	else
	{*/
		ACharacter::StopJumping();
	/*	CallStopJumpOnServer();
	}*/
}

//Stop Jump exemple de runOnServer et multiCast
void ALostAgeCharacter::CallStopJumpOnServer_Implementation()
{
	ACharacter::StopJumping();
}

bool ALostAgeCharacter::CallStopJumpOnServer_Validate()
{
	return true;
}

void ALostAgeCharacter::MultiCastStopJumpToClients_Implementation()
{
	ACharacter::StopJumping();
}

bool ALostAgeCharacter::MultiCastStopJumpToClients_Validate()
{
	return true;
}

void ALostAgeCharacter::SetSpeedServer_Implementation(float speed)
{
	_characterSpeed = speed;
}

bool ALostAgeCharacter::SetSpeedServer_Validate(float speed)
{
	return true;
}

void ALostAgeCharacter::UpdateSaveRotation(FRotator newRotation)
{
	if (this->Role == ROLE_Authority)
		_saveRotation = newRotation;
	else
		UpdateSaveRotationOnServer(newRotation);
}

void ALostAgeCharacter::UpdateSaveCameraRotation(FRotator newRotation)
{
	if (this->Role == ROLE_Authority)
		_saveCameraRotation = newRotation;
	else
		UpdateSaveCameraRotationOnServer(newRotation);
}

void ALostAgeCharacter::UpdateSaveRotationOnServer_Implementation(FRotator newRotation)
{
	_saveRotation = newRotation;
}

bool ALostAgeCharacter::UpdateSaveRotationOnServer_Validate(FRotator newRotation)
{
	return true;
}

void ALostAgeCharacter::UpdateSaveCameraRotationOnServer_Implementation(FRotator newRotation)
{
	_saveCameraRotation = newRotation;
}

bool ALostAgeCharacter::UpdateSaveCameraRotationOnServer_Validate(FRotator newRotation)
{
	return true;
}

float ALostAgeCharacter::GetSpeed()
{
	return _characterSpeed;
}

void ALostAgeCharacter::SetSpeed(float speed)
{
	SetSpeedServer(speed);
}

void ALostAgeCharacter::RequestSavingServer_Implementation()
{
	SendSaveOrder();
}

bool ALostAgeCharacter::RequestSavingServer_Validate()
{
	return true;
}

void ALostAgeCharacter::SendSaveOrder_Implementation()
{	
	if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
	{
		if (ULostAgeSaveManager* saveManager = gameInstance->GetSaveManager())
		{
			bool isSaveSuccessfull = saveManager->Save();
			if (isSaveSuccessfull)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("SAVE COMPLETED"));
		}
	}
}

bool ALostAgeCharacter::SendSaveOrder_Validate()
{
	return true;
}

void ALostAgeCharacter::UpdateSaveVolumeStateOnServeur_Implementation(ALostAgeSaveVolume* volume, bool value)
{
	volume->UpdateActivationStateOnServer(value);
}

bool ALostAgeCharacter::UpdateSaveVolumeStateOnServeur_Validate(ALostAgeSaveVolume* volume, bool value)
{
	return true;
}

/* Ajouté par Quentin pour supporter la réplication de _characterSpeed */
void ALostAgeCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ALostAgeCharacter, _characterSpeed);
	DOREPLIFETIME(ALostAgeCharacter, _saveRotation);
	DOREPLIFETIME(ALostAgeCharacter, _saveCameraRotation);
}