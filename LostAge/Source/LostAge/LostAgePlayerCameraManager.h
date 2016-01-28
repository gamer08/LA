// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "LostAgePlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class LOSTAGE_API ALostAgePlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
private:
	float _minPitch, _maxPitch;
	FVector _relativePosition;
	FRotator _rotation;

	void BeginPlay() override;
	void LimitPitch(FRotator& rotation, float minPitch, float maxPitch);
	void UpdateViewTarget(FTViewTarget& outVT, float deltaTime) override; 
	
	//plus tard pour le save
	void Load();

public:
	ALostAgePlayerCameraManager();
	
	void UpdatePitch(float value);
	void UpdateYaw(float value);	
};
