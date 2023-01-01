// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBarComponent.h"

#include "Core/Logger.h"
#include "UI/HealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


UHealthBarComponent::UHealthBarComponent()
{
	ConstructorHelpers::FClassFinder<UHealthBar> HBarClassFinder(TEXT("/Game/Blueprint/GUI/Widgets/WBP_HealthBar.WBP_HealthBar_C"));

	//LOG_INFO(TEXT("HealthBar found: %i"), HBarClassFinder.Succeeded()) 
	TSubclassOf<UHealthBar> HBarClass = HBarClassFinder.Class;
	
	SetWidgetSpace(EWidgetSpace::Screen);
	SetWidgetClass(HBarClass);
	SetDrawAtDesiredSize(true);
	SetUsingAbsoluteLocation(true);
}

void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto pWorld = GetWorld();
	if (!pWorld)
	{
		//LOG_WARNING(TEXT("Invalid world!"));
		return;
	}

	auto pCamManager = UGameplayStatics::GetPlayerCameraManager(pWorld, 0);
	if (!pCamManager)
	{
		//LOG_WARNING(TEXT("Invalid cam manager!"));
		return;
	}
	
	auto const& playerTfm = pCamManager->GetTransform();
	if (!playerTfm.IsValid())
	{
		LOG_WARNING(TEXT("Invalid player transform!"));
		return;
	}
	
	FVector const Up = playerTfm.TransformVector({0,0,1});

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Axis is: %s"), *Up.ToString()));
	
	SetWorldLocation(GetOwner()->GetActorLocation() + Up * 420);
}
