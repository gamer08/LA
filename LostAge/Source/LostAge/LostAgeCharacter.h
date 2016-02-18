// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "LostAgeCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class ALostAgeCharacter : public ACharacter
{
	friend class ALostAgePlayerController;
	GENERATED_BODY()

public:
	ALostAgeCharacter();

	FORCEINLINE FVector GetCameraLocation() const
	{ 
		return _cameraLocation; 
	}

protected:

	FString _playableClassName;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector _cameraLocation;

	void MoveForward(float value);

	void MoveSide(float value);

	void Jump();

	void StopJumping();

private:

	float _minPitch, _maxPitch;

	void Tick(float deltaTime) override;

	void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void CallStopJumpOnServer();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MultiCastStopJumpToClients();
};

