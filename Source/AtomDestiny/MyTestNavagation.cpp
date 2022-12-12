// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTestNavagation.h"
#include "AIController.h"

#include "Actions/PawnAction_Move.h"

#include <Runtime/NavigationSystem/Public/NavigationPath.h>
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>

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
		UNavigationPath* path = UNavigationSystemV1::FindPathToLocationSynchronously(this, pawn->GetActorLocation(), target->GetActorLocation());

		if (path && path->IsValid())
		{
			FAIMoveRequest req(target);
			req.SetAcceptanceRadius(50);
			req.SetUsePathfinding(true);
			
			if (controller)
			{
				controller->RequestMove(req, path->GetPath());
			}
		}
	}
}

// Called every frame
void UMyTestNavagation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
