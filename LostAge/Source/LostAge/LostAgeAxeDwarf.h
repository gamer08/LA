// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LostAgeAxeDwarf.generated.h"

UCLASS()
class LOSTAGE_API ALostAgeAxeDwarf : public AActor
{
	GENERATED_BODY()
	
public:	
	ALostAgeAxeDwarf();

	virtual void BeginPlay() override;
};