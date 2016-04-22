// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeCubeElf.h"


// Sets default values
ALostAgeCubeElf::ALostAgeCubeElf()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		SetReplicates(true);
		SetReplicateMovement(true);
		bAlwaysRelevant = true;
	}
}

// Called when the game starts or when spawned
void ALostAgeCubeElf::BeginPlay()
{
	Super::BeginPlay();
}

