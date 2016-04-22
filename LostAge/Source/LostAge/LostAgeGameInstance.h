// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "SaveSystem/LostAgeSaveManager.h"
#include "LostAgeGameInstance.generated.h"

class ALostAgePlayerController;

enum class PlayerRole
{
	SERVER = 0,
	CLIENT = 1
};

USTRUCT()
struct FLostPlayerClassInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString _playableClassName;

	UPROPERTY()
	bool _isAvailable;

	UPROPERTY()
	FString _ownerID;

	PlayerRole _ownerRole;

	FLostPlayerClassInfo()
	{
		_playableClassName = FString();
		_isAvailable = true;
		_ownerID = FString();
		_ownerRole = PlayerRole::SERVER;
	}

	FLostPlayerClassInfo(FString className)
	{
		_playableClassName = className;
		_isAvailable = true;
		_ownerID = FString();
		_ownerRole = PlayerRole::SERVER;
	}
};

UCLASS(Blueprintable)
class LOSTAGE_API ULostAgeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	TArray<FLostPlayerClassInfo> _playableClassesInfo;
	//TMap<FString, FString> _playerRoles;

	UFUNCTION(Server, Reliable, WithValidation, Category = "MainMenu" )
	void ReleaseAllPlayableClassesInfo();

	UFUNCTION(Server, Reliable, WithValidation, Category = "MainMenu" )
	void ReleasePlayableClassOfController(AController* controller);

	UPROPERTY()
	ULostAgeSaveManager* _saveManager;


public:
	ULostAgeGameInstance();	

	void CreatePauseMenu();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "MainMenu" )
	void LoadMainMenu(ALostAgePlayerController* controller);

	UFUNCTION(Server, Reliable, WithValidation, Category = "MainMenu" )
	void ReleasePlayableClass(AController* controller);

	////Should be only call when a player host a game
	UFUNCTION(BlueprintCallable, Category = "ClassesSelection")
	void SetClassChossed(AController* controller,FString className);

	FString GetPawnClass(FString playerUniqueID, AController* controller);

	FLostPlayerClassInfo GetClassInfoByPlayerID(FString playerID);

	UFUNCTION(BlueprintCallable, Category = "Save Manager")
	ULostAgeSaveManager* GetSaveManager() const
	{
		return _saveManager;
	}

	void Init() override;

};
