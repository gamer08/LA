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

protected:

	void MoveForward(float value);

	void MoveSide(float value);

	void Turn(float value);

	void LookUp(float value);

	void Jump();

	void StopJumping();

private:

	
	float _minPitch, _maxPitch;

	void Tick(float deltaTime) override;

	void BeginPlay() override;
	
	void InitActionOnObjectDelegate();

	void LimitPitch(FRotator& rotation, float minPitch, float maxPitch);

};

