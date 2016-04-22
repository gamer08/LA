// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "LostAgeEnum.h"
#include "LostAgeObserverInterface.h"
#include "LostAgeObserver.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivateParent);

class ULostAgeSubject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTAGE_API ULostAgeObserver : public UActorComponent , public ILostAgeObserverInterface
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<ULostAgeSubject*,int8> _subjects;
	
protected:
	void UpdateState(ULostAgeSubject* subject, int8 value);

	int8 CheckAllSubjects();
	
	UFUNCTION(Server, Reliable, WithValidation, Category = "Observer" )
	void UnregisterFromAllSubjects();

	UFUNCTION(BlueprintCallable, Category = "Observer" )
	void ActivateParent();

	UFUNCTION(Server, Reliable, WithValidation, Category = "Observer" )
	virtual void OnNotify_Internal(ULostAgeSubject* subject, EObserverEvent::Type call);

public:			
	ULostAgeObserver();

	virtual void BeginPlay() override;

	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
		
	UFUNCTION()
	virtual void OnNotify(ULostAgeSubject* subject, EObserverEvent::Type call);

	UPROPERTY(BlueprintAssignable, Category="Observer")
	FOnActivateParent OnActivateParent;
};