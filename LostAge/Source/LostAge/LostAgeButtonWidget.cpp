// Fill out your copyright notice in the Description page of Project Settings.

#include "LostAge.h"
#include "LostAgeButtonWidget.h"

ULostAgeButtonWidget::ULostAgeButtonWidget()
: Super()
{
}

void ULostAgeButtonWidget::CallOnClickedEvent()
{
	OnClicked.Broadcast();
}