// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LostAgeCubeElf.generated.h"

UCLASS()
class LOSTAGE_API ALostAgeCubeElf : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALostAgeCubeElf();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
