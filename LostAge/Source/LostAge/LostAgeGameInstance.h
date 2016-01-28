// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "LostAgeGameInstance.generated.h"

/**
 * 
 */
class ALostAgePlayerController;

UCLASS(Blueprintable)
class LOSTAGE_API ULostAgeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	ULostAgeGameInstance();	
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "MainMenu" )
	void LoadMainMenu(ALostAgePlayerController* controller);
};
