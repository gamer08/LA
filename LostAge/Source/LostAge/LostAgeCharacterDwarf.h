// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostAgeCharacter.h"
#include "SaveSystem/LostAgeSaveInterface.h"
#include "LostAgeCharacterDwarf.generated.h"

/**
 * 
 */
UCLASS()
class LOSTAGE_API ALostAgeCharacterDwarf : public ALostAgeCharacter, public ILostAgeSaveInterface
{
	GENERATED_BODY()
	
public:
	ALostAgeCharacterDwarf();

	void Save(FLostAgeSaveData& saveData);
	
private:
	void BeginPlay() override;

	//void Tick(float deltaTime) override;

	void Load();
};
