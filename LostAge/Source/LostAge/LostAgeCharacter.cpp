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
	/*if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{*/
		GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

		_minPitch = -89.0f;
		_maxPitch = 89.0f;

		
		PrimaryActorTick.bCanEverTick = true;
	//}
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

void ALostAgeCharacter::Turn(float value)
{
	FRotator actorRotation = GetActorRotation();
	actorRotation.Yaw += value;
	SetActorRotation(actorRotation);
}

void ALostAgeCharacter::LookUp(float value)
{
	FRotator actorRotation = GetActorRotation();
	actorRotation.Pitch += value;
	LimitPitch(actorRotation, _minPitch, _maxPitch);
	SetActorRotation(actorRotation);
}

void ALostAgeCharacter::Jump()
{
	ACharacter::Jump();
}

void ALostAgeCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void ALostAgeCharacter::LimitPitch(FRotator& rotation, float minPitch, float maxPitch)
{
	rotation.Pitch = FMath::ClampAngle(rotation.Pitch, minPitch, maxPitch);
	rotation.Pitch = FRotator::ClampAxis(rotation.Pitch);
}
