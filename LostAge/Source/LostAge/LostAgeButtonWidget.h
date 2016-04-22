// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "LostAgeButtonWidget.generated.h"

UCLASS()
class LOSTAGE_API ULostAgeButtonWidget : public UButton
{
	GENERATED_BODY()
	
public:
	ULostAgeButtonWidget();
	
	UFUNCTION(BlueprintCallable, Category = "Events Call" )
	void CallOnClickedEvent();
};