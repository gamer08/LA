// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostAgeCharacter.h"
#include "SaveSystem/LostAgeSaveInterface.h"
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
	
private:
	void BeginPlay() override;

	void Load();
};
