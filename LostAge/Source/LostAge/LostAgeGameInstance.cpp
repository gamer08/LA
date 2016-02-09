// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeGameInstance.h"
#include "Engine.h"

ULostAgeGameInstance::ULostAgeGameInstance()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		//_saveManager = NewObject<UBrainSaveManager>(this, FName("SaveManager"));
				
		_playableClassesInfo.Add(FLostPlayerClassInfo(FString("Elf")));
		_playableClassesInfo.Add(FLostPlayerClassInfo(FString("Dwarf")));
	}
}

void ULostAgeGameInstance::SetClassChossed(AController* controller, FString className)
{
	for (FLostPlayerClassInfo& info : _playableClassesInfo)
	{
		if (info._playableClassName.Equals(className) && info._isAvailable)
		{
			FString playerId = controller->PlayerState->UniqueId.GetUniqueNetId().Get()->ToString();

			//_playerRoles.Emplace(playerId, "Server");
			
			info._isAvailable = false;
			info._ownerID = playerId;
			info._ownerRole = PlayerRole::SERVER;
			break;
		}
	}
}

FString ULostAgeGameInstance::GetPawnClass(FString playerUniqueID, AController* controller)
{
	for (FLostPlayerClassInfo& info : _playableClassesInfo)
	{
		if (info._ownerID.Equals(playerUniqueID))
			return info._playableClassName;
	}

	for (FLostPlayerClassInfo& info : _playableClassesInfo)
	{
		if (info._isAvailable)
		{
			//_playerRoles.Emplace(playerUniqueID, "Client");

			info._isAvailable = false;
			info._ownerID = playerUniqueID;
			info._ownerRole = PlayerRole::CLIENT;
			return info._playableClassName;
		}
	}

	return FString("none");
}

void ULostAgeGameInstance::ReleasePlayableClass_Implementation(AController* controller)
{
	FString playerId = controller->PlayerState->UniqueId.GetUniqueNetId().Get()->ToString();
	FLostPlayerClassInfo find = GetClassInfoByPlayerID(playerId);
	
	if (!find._ownerID.IsEmpty())
	{
		switch (find._ownerRole)
		{
			case PlayerRole::SERVER:
				ReleaseAllPlayableClassesInfo();
				break;

			case PlayerRole::CLIENT:
				ReleasePlayableClassOfController(controller);
				break;

			default:
				break;
		}
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Don't know this player"));


	/*if (_playerRoles.Contains(playerId))
	{
		if (_playerRoles[playerId].Equals(FString("Server")))
			ReleaseAllPlayableClassesInfo();
		else
			ReleasePlayableClassOfController(controller);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Don't know this player"));*/

}

bool ULostAgeGameInstance::ReleasePlayableClass_Validate(AController* controller)
{
	return true;
}


void ULostAgeGameInstance::ReleasePlayableClassOfController_Implementation(AController* controller)
{
	for (FLostPlayerClassInfo& info : _playableClassesInfo)
	{
		FString idToRelease = controller->PlayerState->UniqueId.GetUniqueNetId().Get()->ToString();
		if (info._ownerID.Equals(idToRelease))
		{
			info._isAvailable = true;
			info._ownerID = FString();
			info._ownerRole = PlayerRole::SERVER;
			//_playerRoles.Remove(idToRelease);
			break;
		}
	}
}

bool ULostAgeGameInstance::ReleasePlayableClassOfController_Validate(AController* controller)
{
	return true;
}

void ULostAgeGameInstance::ReleaseAllPlayableClassesInfo_Implementation()
{	
	for (FLostPlayerClassInfo& info : _playableClassesInfo)
	{
		info._isAvailable = true;
		info._ownerID = FString();
		info._ownerRole = PlayerRole::SERVER;
	}

	//_playerRoles.Empty();
}

bool ULostAgeGameInstance::ReleaseAllPlayableClassesInfo_Validate()
{
	return true;
}

FLostPlayerClassInfo ULostAgeGameInstance::GetClassInfoByPlayerID(FString playerID)
{
	for (FLostPlayerClassInfo& info : _playableClassesInfo)
	{
		if (info._ownerID.Equals(playerID))
			return info;
	}

	return FLostPlayerClassInfo();
}