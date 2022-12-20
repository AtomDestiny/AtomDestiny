// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBar.h"

#include "Components/ProgressBar.h"
#include "Math/Vector.h"

/*UHealthBar::UHealthBar(const FObjectInitializer& ObjectInitializer, AActor* root) : UUserWidget(ObjectInitializer)
{
	ActorRoot = root;
}*/

void UHealthBar::SetPercent(float value)
{
    ProgressBar->SetPercent(value);
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Owner Location is: %s"), *ActorRoot->GetActorLocation().ToString()));
	//UE_LOG(LogTemp, Display, ActorRoot->GetActorLocation().ToText());
}

