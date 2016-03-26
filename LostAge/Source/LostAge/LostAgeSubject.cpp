// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeSubject.h"


// Sets default values for this component's properties
ULostAgeSubject::ULostAgeSubject()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
}

// Called when the game starts
void ULostAgeSubject::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* act : _observerActors)
	{
		TArray<UActorComponent*> comps;
		act->GetComponents(comps);

		for (UActorComponent* comp : comps)
		{
			if (comp->GetClass()->ImplementsInterface(ULostAgeObserverInterface::StaticClass()))
			{
				if (ILostAgeObserverInterface* o = Cast<ILostAgeObserverInterface>(comp))
				{
					TScriptInterface<ILostAgeObserverInterface> t;
					t.SetObject(Cast<UObject>(o));
					t.SetInterface(o);
					RegisterObserver(t);
				}

			}
		}

		NotifyAll(EObserverEvent::HELLO);
	}
}

// Called every frame
void ULostAgeSubject::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void ULostAgeSubject::NotifyAll_Implementation(EObserverEvent::Type call)
{
	if (GetOwner()->HasAuthority())
	{
		for (TScriptInterface<ILostAgeObserverInterface> obs : _observers)
			obs->OnNotify(this,call);
	}
}

bool ULostAgeSubject::NotifyAll_Validate(EObserverEvent::Type call)
{
	return true;
}

void ULostAgeSubject::RegisterObserver_Implementation(const TScriptInterface<ILostAgeObserverInterface>& o)
{
	
	if (GetOwner()->HasAuthority())
	{
		if (!_observers.Contains(o))
			_observers.Add(o);
	}
}

bool ULostAgeSubject::RegisterObserver_Validate(const TScriptInterface<ILostAgeObserverInterface>& o)
{
	return true;
}
//

void ULostAgeSubject::UnregisterObserver_Implementation(const TScriptInterface<ILostAgeObserverInterface>& o)
{
	if (GetOwner()->HasAuthority())
	{
		if (_observers.Contains(o))
			_observers.Remove(o);
	}
}

bool ULostAgeSubject::UnregisterObserver_Validate(const TScriptInterface<ILostAgeObserverInterface>& o)
{
	return true;
}

void ULostAgeSubject::SubjectActivate_Implementation()
{
	if (GetOwner()->HasAuthority())
		OnActivateSubject.Broadcast();
}

bool ULostAgeSubject::SubjectActivate_Validate()
{
	return true;
}

void ULostAgeSubject::SubjectDesactivate_Implementation()
{
	if (GetOwner()->HasAuthority())
		OnDesactivateSubject.Broadcast();
}

bool ULostAgeSubject::SubjectDesactivate_Validate()
{
	return true;
}