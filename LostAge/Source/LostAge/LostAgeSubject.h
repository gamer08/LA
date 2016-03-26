// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "LostAgeObserverInterface.h"
#include "LostAgeEnum.h"
#include "LostAgeSubject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivateSubject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDesactivateSubject);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTAGE_API ULostAgeSubject : public UActorComponent
{
	GENERATED_BODY()

private:
	TArray<TScriptInterface<ILostAgeObserverInterface>> _observers;

public:	
	ULostAgeSubject();

	virtual void BeginPlay() override;
	
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(Server, Reliable, WithValidation, Category = "Subject" )
	void NotifyAll(EObserverEvent::Type call);

	UFUNCTION(Server, Reliable, WithValidation, Category = "Subject" )
	void SubjectActivate();

	UFUNCTION(Server, Reliable, WithValidation, Category = "Subject" )
	void SubjectDesactivate();
	
	UFUNCTION(Server, Reliable, WithValidation, Category = "Subject" )
	void RegisterObserver(const TScriptInterface<ILostAgeObserverInterface>& o);
	
	UFUNCTION(Server, Reliable, WithValidation, Category = "Subject" )
	void UnregisterObserver(const TScriptInterface<ILostAgeObserverInterface>& o);

	UPROPERTY(EditAnywhere, Category = "Observer", meta = (DisplayName = "Observer Actors"))
	TArray<AActor*> _observerActors;

	UPROPERTY(BlueprintAssignable, Category="Observer")
	FOnActivateSubject OnActivateSubject;

	UPROPERTY(BlueprintAssignable, Category="Observer")
	FOnDesactivateSubject OnDesactivateSubject;
};
