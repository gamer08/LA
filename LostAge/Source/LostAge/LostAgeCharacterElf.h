// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostAgeCharacter.h"
#include "SaveSystem/LostAgeSaveInterface.h"
#include "LostAgeCubeElf.h"
#include "LostAgeCharacterElf.generated.h"

/**
 * 
 */
UCLASS()
class LOSTAGE_API ALostAgeCharacterElf : public ALostAgeCharacter, public ILostAgeSaveInterface
{
	GENERATED_BODY()
	
public:
	ALostAgeCharacterElf();
	void Save(FLostAgeSaveData& saveData);

	UPROPERTY(Replicated, BlueprintReadwrite, Category = "Cube", meta = (DisplayName = CubeElfReference))
	ALostAgeCubeElf* _cubeElf;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Owner")
	void SetOwner();
	
private:
	TSubclassOf<class ALostAgeCubeElf> _cubeBlueprintClass;
	
	void BeginPlay() override;

	void Load();

	UFUNCTION(Server, Reliable, WithValidation)
	void RequestSpawnCubeOnLoadServer(FLostAgeCubeElfSaveData cubeData);
};
