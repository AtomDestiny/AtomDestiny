// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

#include "AtomDestiny/UI/HealthBar.h"

// Sets default values
ATestActor::ATestActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (RootComponent == nullptr)
    {
        m_box = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Box"));
        RootComponent = m_box;
    }

    m_mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
    m_mesh->SetupAttachment(RootComponent);

    m_healthBar = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("HealthBar"));
    m_healthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    m_healthBar->SetUsingAbsoluteLocation(true);
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D screenPos;

	auto const* player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	auto result = UGameplayStatics::ProjectWorldToScreen(
			player,
			GetActorLocation(),
			screenPos
		);
	
	if (result)
	{
		FVector worldPos, worldDir;
		screenPos -= {0, 20};
		
		UGameplayStatics::DeprojectScreenToWorld(
				player,
				screenPos,
				worldPos,
				worldDir
			);
		m_healthBar->SetWorldLocation(worldPos);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("HealthBar is not updated!"));
}
