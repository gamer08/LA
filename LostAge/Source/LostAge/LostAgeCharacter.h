// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "SaveSystem/LostAgeSaveInterface.h"
#include "LostAgeCharacter.generated.h"

class UInputComponent;
class ALostAgeSaveVolume;

UCLASS(config=Game)
class ALostAgeCharacter : public ACharacter/*, public ILostAgeSaveInterface*/
{
	friend class ALostAgePlayerController;
	GENERATED_BODY()

public:
	ALostAgeCharacter();

	FORCEINLINE FVector GetCameraLocation() const
	{ 
		return _cameraLocation; 
	}

	FORCEINLINE FString GetPlayableClassName() const
	{ 
		return _playableClassName; 
	}
	
	UFUNCTION(BlueprintCallable, Category = "Character Speed")
	float GetSpeed();

	UFUNCTION(Server, Reliable, WithValidation)
	void RequestSavingServer();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void SendSaveOrder();

	UFUNCTION()
	void UpdateSaveRotation(FRotator newRotation);

	UFUNCTION()
	void UpdateSaveCameraRotation(FRotator newRotation);
	
	UFUNCTION(Server, Unreliable, WithValidation)
	void UpdateSaveRotationOnServer(FRotator newRotation);

	UFUNCTION(Server, Unreliable, WithValidation)
	void UpdateSaveCameraRotationOnServer(FRotator newRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void UpdateSaveVolumeStateOnServeur(ALostAgeSaveVolume* volume, bool value);

	FORCEINLINE FRotator GetSaveRotation() const
	{
		return _saveRotation;
	}

	UFUNCTION(BlueprintCallable, Category = "Camera")
	FRotator GetSaveCameraRotation() const
	{
		return _saveCameraRotation;
	}

protected:
	FString _playableClassName;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector _cameraLocation;

	UPROPERTY(Replicated)
	FRotator _saveRotation;

	UPROPERTY(Replicated)
	FRotator _saveCameraRotation;

	void BeginPlay() override;

	void MoveForward(float value);

	void MoveSide(float value);

	void Jump();

	void StopJumping();
	
	void Tick(float deltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Character Speed")
	void SetSpeed(float speed);

private:

	float _minPitch, _maxPitch;

	UPROPERTY(Replicated)
	float _characterSpeed;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void CallStopJumpOnServer();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MultiCastStopJumpToClients();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void SetSpeedServer(float speed);
};