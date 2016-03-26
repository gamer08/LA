// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgePlayerCameraManager.h"
#include "LostAgePlayerController.h"
#include "LostAgeCharacter.h"

ALostAgePlayerCameraManager::ALostAgePlayerCameraManager()
{
	//Empecher qu'Unreal cree des objets templates doublons pour les backup
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		_minPitch = -89.0f;
		_maxPitch = 89.0f;
		_rotation = FRotator(0.0f);
	}
}

void ALostAgePlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	if (ALostAgePlayerController* pc = Cast<ALostAgePlayerController>(GetOwningPlayerController()))
		_relativePosition = pc->GetTargetCameraLocation();
	else
		_relativePosition = FVector(0.0f, 0.0f, 64.0f);
}

void ALostAgePlayerCameraManager::UpdateViewTarget(FTViewTarget& outVT, float deltaTime)
{
	//Construit une matice de rotation a partir de Yaw et Pitch pur calculer la nouvelle position relative 
	FRotationMatrix cameraRotation(FRotator(_rotation.Pitch, _rotation.Yaw, 0.0f));
	
	outVT.POV.Location = outVT.Target->GetActorLocation() + cameraRotation.TransformPosition(_relativePosition);
	outVT.POV.Rotation = _rotation;
}

void ALostAgePlayerCameraManager::UpdateYaw(float value)
{
	_rotation.Yaw += value;
}

void ALostAgePlayerCameraManager::UpdatePitch(float value)
{
	_rotation.Pitch += value;
	LimitPitch(_rotation, _minPitch, _maxPitch);
}

void ALostAgePlayerCameraManager::LimitPitch(FRotator& rotation, float minPitch, float maxPitch)
{
	rotation.Pitch = FMath::ClampAngle(rotation.Pitch, minPitch, maxPitch);
	rotation.Pitch = FRotator::ClampAxis(rotation.Pitch);
}

void ALostAgePlayerCameraManager::SetRotation(FRotator newRotation)
{
	_rotation = newRotation;
}