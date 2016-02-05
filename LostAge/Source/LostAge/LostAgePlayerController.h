// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "LostAgeCharacter.h"
#include "LostAgePlayerController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LOSTAGE_API ALostAgePlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:

	//ALostAgeCharacter* _pawn;

	float _cameraRotationSpeed;

	void MoveForward(float value);
	void MoveSide(float value);
	void Turn(float value);
	void LookUp(float value);
	void Jump();
	void StopJumping();
	void LeaveToMainMenu();
	
	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay() override;



public:

	ALostAgePlayerController();

	FORCEINLINE FRotator GetCameraRotation() const
	{ 
		return PlayerCameraManager->GetCameraRotation(); 
	}
	
	virtual void Possess(APawn* aPawn) override;

	UFUNCTION(Server, Reliable, WithValidation, Category = "PlayableClass" )
	void RequestReleasePlayableClass(AController* controller);
};
