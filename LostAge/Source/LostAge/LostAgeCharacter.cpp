// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LostAge.h"
#include "LostAgeCharacter.h"
#include "LostAgePlayerController.h"
#include "LostAgeGameInstance.h"
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

	FRotator rot = FRotator(0);
	SetActorRotation(rot);
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
	AddMovementInput(Direction, value);
}

void ALostAgeCharacter::MoveSide(float value)
{
	const FRotator Rotation = Cast<ALostAgePlayerController>(Controller)->GetCameraRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, value);
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