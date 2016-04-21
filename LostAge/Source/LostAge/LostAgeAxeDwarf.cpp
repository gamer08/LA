// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeAxeDwarf.h"

ALostAgeAxeDwarf::ALostAgeAxeDwarf()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		SetReplicates(true);
		SetReplicateMovement(true);
		bAlwaysRelevant = false;
	}
}

void ALostAgeAxeDwarf::BeginPlay()
{
	Super::BeginPlay();
	
}