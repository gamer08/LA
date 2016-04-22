// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeSaveVolume.h"
#include "LostAgeCharacter.h"
#include "LostAgeGameInstance.h"
#include "SaveSystem/LostAgeSaveManager.h"

ALostAgeSaveVolume::ALostAgeSaveVolume()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		SetReplicates(true);
		_isAlreadyActivated = false;
	}
}

void ALostAgeSaveVolume::BeginPlay()
{
	Super::BeginPlay();

	if (this->GetClass()->ImplementsInterface(ULostAgeSaveInterface::StaticClass()))
		Load();
}

void ALostAgeSaveVolume::Save(FLostAgeSaveData& saveData)
{
	FLostAgeSaveVolumeData dataToSave;

	dataToSave._loadFromfile = true;
	dataToSave._isAlreadyActivated = _isAlreadyActivated;
	
	saveData.AddDataToSave(GetName(),dataToSave);
}

void ALostAgeSaveVolume::Load()
{
	if (!GetName().IsEmpty())
	{
		if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
		{
			if (ULostAgeSaveManager* saveManager = gameInstance->GetSaveManager())
			{
				FLostAgeSaveVolumeData savedData = saveManager->GetDataFromSave<FLostAgeSaveVolumeData>(GetName());

				if (savedData._loadFromfile)
					_isAlreadyActivated = savedData._isAlreadyActivated;
			}
		}
	}
}

void ALostAgeSaveVolume::NotifyActorBeginOverlap(AActor* otherActor)
{
	if (!_isAlreadyActivated)
	{
		//Hack pour gérer le fait que dans la version packagée, l'évènement se produit avant que le chrgement des données soient fait
        //Cela produisait un save même si le volume avait déjà été utilisé car, ses données ne sont pas encore chargées.
		bool savedValue = false;
		if (ULostAgeGameInstance* gameInstance = Cast<ULostAgeGameInstance>(GetGameInstance()))
		{
			if (ULostAgeSaveManager* saveManager = gameInstance->GetSaveManager())
			{
				FLostAgeSaveVolumeData savedData = saveManager->GetDataFromSave<FLostAgeSaveVolumeData>(GetName());

				if (savedData._loadFromfile)
					savedValue = savedData._isAlreadyActivated;
			}
		}

		if (!savedValue)
		{
			if (ALostAgeCharacter* character = Cast<ALostAgeCharacter>(otherActor))
			{	
				if (HasAuthority())
				{
					character->UpdateSaveVolumeStateOnServeur(this, true);
					character->RequestSavingServer();
				}
			}
		}
	}
}

void ALostAgeSaveVolume::UpdateActivationState(bool value)
{
	if (this->Role == ROLE_Authority)
		_isAlreadyActivated = value;
	else
		UpdateActivationStateOnServer(value);
}

void ALostAgeSaveVolume::UpdateActivationStateOnServer_Implementation(bool value)
{
	UpdateActivationStateMulticast(value);
}

bool ALostAgeSaveVolume::UpdateActivationStateOnServer_Validate(bool value)
{
	return true;
}

void ALostAgeSaveVolume::UpdateActivationStateMulticast_Implementation(bool value)
{
	_isAlreadyActivated = value;
}

bool ALostAgeSaveVolume::UpdateActivationStateMulticast_Validate(bool value)
{
	return true;
}

void ALostAgeSaveVolume::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ALostAgeSaveVolume, _isAlreadyActivated);
}