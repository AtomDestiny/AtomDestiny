// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTestNavagation.h"
#include "AIController.h"

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

	AActor* actor = nullptr;
	
	for (const auto a : actors)
	{
		if (a->GetActorLabel() == TEXT("TargetPoint"))
			actor = a;
	}

	APawn* pawn = CastChecked<APawn>(GetOwner());
	AController* ai = pawn->Controller;
	AAIController* controller = CastChecked<AAIController>(ai);

	if (actor)
	{
		controller->SetPawn(pawn);

		if (const auto res = controller->MoveToActor(actor); res == EPathFollowingRequestResult::Type::Failed)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to move actor"));
		}
	}
}

// Called every frame
void UMyTestNavagation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
