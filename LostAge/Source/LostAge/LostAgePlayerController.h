// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "LostAgeCharacter.h"
#include "LostAgeEnum.h"
//#include "SaveSystem/LostAgeSaveInterface.h"
#include "LostAgePlayerController.generated.h"

class ULostAgeSubject;

UCLASS(Blueprintable)
class LOSTAGE_API ALostAgePlayerController : public APlayerController/*, public ILostAgeSaveInterface*/
{
	GENERATED_BODY()
	
private:

	float _cameraRotationSpeed;

	void MoveForward(float value);
	void MoveSide(float value);
	void Turn(float value);
	void LookUp(float value);
	void Jump();
	void StopJumping();
	
	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay() override;

	virtual void SpawnPlayerCameraManager() override;

public:

	ALostAgePlayerController();

	FORCEINLINE FRotator GetCameraRotation() const
	{ 
		return PlayerCameraManager->GetCameraRotation(); 
	}
	
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void GiveControlsToUI(bool value);

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void LeaveToMainMenu();

	FVector GetTargetCameraLocation() const;
	
	virtual void Possess(APawn* aPawn) override;

	UFUNCTION(Server, Reliable, WithValidation, Category = "PlayableClass" )
	void RequestReleasePlayableClass(AController* controller);

	UFUNCTION(Blueprintcallable, Server, Reliable, WithValidation, Category = "PlayableClass" )
	void RequestLoadNextMap(const FString& levelName);

	UFUNCTION(Blueprintcallable, Server, Reliable, WithValidation, Category = "Notification" )
	void NeedSubjectNotify(ULostAgeSubject* subjectToNotify, EObserverEvent::Type evnt);

	UFUNCTION(Blueprintcallable, Server, Reliable, WithValidation, Category = "Subject" )
	void ActivateSubject(ULostAgeSubject* subjectToActivate);

	UFUNCTION(Blueprintcallable, Server, Reliable, WithValidation, Category = "Subject" )
	void DesactivateSubject(ULostAgeSubject* subjectToDesactivate);

	UFUNCTION(NetMulticast, Reliable, WithValidation, Category = "PlayableClass" )
	void SendLeaveOrderToEveryone();

	UFUNCTION(Client, Reliable, WithValidation, Category = "SaveSystem" )
	void LoadSaveOnClient(const FString& playerClass);

	void SetControllerSavedInfo(FLostAgeElfSaveData data);
	void SetControllerSavedInfo(FLostAgeDwarfSaveData data);
};
