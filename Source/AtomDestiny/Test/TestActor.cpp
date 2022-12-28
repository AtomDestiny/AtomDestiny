// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


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

	auto const& playerTfm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransform();
	if (!playerTfm.IsValid())
		UE_LOG(LogTemp, Warning, TEXT("Invalid player transform!"));
	
	FVector const Up = playerTfm.TransformVector({0,0,1});

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Axis is: %s"), *Up.ToString()));
	
	m_healthBar->SetWorldLocation(GetActorLocation() + Up * 420);
}
