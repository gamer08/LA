// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeGameInstance.h"

ULostAgeGameInstance::ULostAgeGameInstance()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		//_saveManager = NewObject<UBrainSaveManager>(this, FName("SaveManager"));
	}
}


