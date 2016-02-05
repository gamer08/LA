// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "LostAgeGameMode.generated.h"

class ALostAgeCharacter;
class ALostAgePlayerController;

UCLASS(minimalapi,Blueprintable)
class ALostAgeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALostAgeGameMode();

	void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	TArray<APlayerStart*> _playerStart;
	
	//Should be only call when a player host a game
	UFUNCTION(BlueprintCallable, Category = "ClassesSelection")
	void SpawnPlayerChossedClass(ALostAgePlayerController* controller, FString className);

	UFUNCTION(BlueprintCallable, Category = "ClassesSelection")
	TSubclassOf<class ALostAgeCharacter> GetClassReferenceByName(FString className);

	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

private:
	
	UPROPERTY()
	TMap <FString, TSubclassOf<class ALostAgeCharacter>> _playableClassesReferences;
	
};



