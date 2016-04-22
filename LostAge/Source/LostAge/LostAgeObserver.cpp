// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeObserver.h"
#include "LostAgeSubject.h"

ULostAgeObserver::ULostAgeObserver()
{
	//Empecher qu'Unreal cree des objets templates doublons pour les backup
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		bWantsBeginPlay = true;
		PrimaryComponentTick.bCanEverTick = false;
	}
}

// Called when the game starts
void ULostAgeObserver::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ULostAgeObserver::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void ULostAgeObserver::UpdateState(ULostAgeSubject* subject, int8 value)
{
	if (GetOwner()->HasAuthority())
	{
		if (_subjects.Contains(subject))
		_subjects[subject] = value;

		int8 canActivate = CheckAllSubjects();
		if (canActivate == 1)
		{
			UnregisterFromAllSubjects();
			ActivateParent();
		}
	}
}

int8 ULostAgeObserver::CheckAllSubjects()
{
	//technically this function is not even call when the observers has 0 subjects, but it's for manage weird case with unreal
	if (_subjects.Num() == 0)
		return 0;

	for (auto& Elem : _subjects)
	{
		if (Elem.Value == 0)
			return 0;	
	}
	
	return 1;
}

void ULostAgeObserver::OnNotify(ULostAgeSubject* subject, EObserverEvent::Type call)
{
	OnNotify_Internal(subject, call);
}

void ULostAgeObserver::OnNotify_Internal_Implementation(ULostAgeSubject* subject, EObserverEvent::Type call)
{
	if (GetOwner()->HasAuthority())
	{
		switch (call)
		{
			case EObserverEvent::HELLO:
				_subjects.Emplace(subject, 0);
				break;

			case EObserverEvent::EVENTON:
				UpdateState(subject, 1);
				break;

			case EObserverEvent::EVENTOFF:
				UpdateState(subject, 0);
				break;

			default:
				break;
		}
	}
}

bool ULostAgeObserver::OnNotify_Internal_Validate(ULostAgeSubject* subject, EObserverEvent::Type call)
{
	return true;
}

void ULostAgeObserver::UnregisterFromAllSubjects_Implementation()
{
	if (GetOwner()->HasAuthority())
	{
		for (auto& Elem : _subjects)
		{
			if (ILostAgeObserverInterface* o = Cast<ILostAgeObserverInterface>(this))
			{
				TScriptInterface<ILostAgeObserverInterface> t;
				t.SetObject(Cast<UObject>(o));
				t.SetInterface(o);
				Elem.Key->UnregisterObserver(t);
			}
		}
			
	}
}

bool ULostAgeObserver::UnregisterFromAllSubjects_Validate()
{
	return true;
}

void ULostAgeObserver::ActivateParent()
{
	OnActivateParent.Broadcast();
}
