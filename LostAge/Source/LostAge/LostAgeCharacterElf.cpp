// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeCharacterElf.h"

ALostAgeCharacterElf::ALostAgeCharacterElf()
{
	_playableClassName = FString("Elf");
	_cameraLocation = FVector(0.0f, 0.0f, 64.0f);
}

