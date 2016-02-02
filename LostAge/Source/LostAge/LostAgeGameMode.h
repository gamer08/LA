// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "LostAgeGameMode.generated.h"

class ALostAgePlayerController;

UCLASS(minimalapi,Blueprintable)
class ALostAgeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALostAgeGameMode();

	void BeginPlay() override;
	//void PostLogin(APlayerController* playerController) override;

	/*UFUNCTION(Server,Reliable,WithValidation)
	void RespawnPlayerForController(APlayerController* controller);*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	TArray<APlayerStart*> _playerStart;

private:
	void SetupGameModeWithCustomClasses();

	//TSubclassOf<class APawn> _pawnClass;

};



