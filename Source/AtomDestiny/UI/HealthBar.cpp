// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBar.h"

#include "Components/ProgressBar.h"

/*UHealthBar::UHealthBar(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
   // slider = CreateDefaultSubobject<UProgressBar>(TEXT("ValueBar"));
}*/

void UHealthBar::SetPercent(float value)
{
    Slider->SetPercent(value);
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    UE_LOG(LogTemp, Display, TEXT("HealthBar ticked"));
}

