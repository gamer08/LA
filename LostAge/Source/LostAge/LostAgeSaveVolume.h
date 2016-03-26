// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Volume.h"
#include "SaveSystem/LostAgeSaveInterface.h"
#include "LostAgeSaveVolume.generated.h"

/**
 * 
 */
UCLASS()
class LOSTAGE_API ALostAgeSaveVolume : public AVolume, public ILostAgeSaveInterface
{
	GENERATED_BODY()
	
private:	
	UPROPERTY(Replicated)
	bool _isAlreadyActivated;

	void BeginPlay() override;
	void Load();

	UFUNCTION()
	void UpdateActivationState(bool value);
	
public:
	ALostAgeSaveVolume();
	
	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;

	void Save(FLostAgeSaveData& saveData);

	UFUNCTION(Server, Reliable, WithValidation)
	void UpdateActivationStateOnServer(bool value);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void UpdateActivationStateMulticast(bool value);

};
