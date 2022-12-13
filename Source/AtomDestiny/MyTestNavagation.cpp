// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTestNavagation.h"

#include "AIController.h"
#include "DetourCrowdAIController.h"

#include "Actions/PawnAction_Move.h"

#include <Runtime/NavigationSystem/Public/NavigationPath.h>
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>

#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyTestNavagation::UMyTestNavagation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyTestNavagation::BeginPlay()
{
	static size_t counter = 0;
	
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);
	
	AActor* target = nullptr;
	
	for (const auto a : actors)
	{
		if (a->GetActorLabel() == TEXT("TargetPoint"))
			target = a;
	}
	
	APawn* pawn = CastChecked<APawn>(GetOwner());
	AController* ai = pawn->Controller;
	AAIController* controller = CastChecked<AAIController>(ai);
	
	if (target)
	{
		controller->MoveToActor(target, 5000);
	}
	
		UE_LOG(LogTemp, Log, TEXT("Setup timer for second unit"));
		
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		FTimerHandle handler;
		timerManager.SetTimer(handler, [controller = controller]()
		{
			controller->StopMovement();
			UE_LOG(LogTemp, Log, TEXT("Movement should be stopped"));
		}, 2.0f, false);
}

// Called every frame
void UMyTestNavagation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// TArray<AActor*> actors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);
	//
	// AActor* target = nullptr;
	//
	// for (const auto a : actors)
	// {
	// 	if (a->GetActorLabel() == TEXT("TargetPoint"))
	// 		target = a;
	// }
	//
	// APawn* pawn = CastChecked<APawn>(GetOwner());
	// AController* ai = pawn->Controller;
	// AAIController* controller = CastChecked<AAIController>(ai);
	//
	// if (target)
	// {
	// 	controller->UpdateControlRotation(DeltaTime);
	// 	controller->MoveToActor(target, 10);
	// }
	// ...
}
