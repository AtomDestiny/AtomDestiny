// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

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

    /*m_box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    SetRootComponent(m_box);*/

    m_mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
    m_mesh->SetupAttachment(RootComponent);

    /*m_bar = CreateDefaultSubobject<UHealthBar>(TEXT("HealthBar"));
    m_bar->SetupAttachment(m_box);*/

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

    UE_LOG(LogTemp, Display, TEXT("TestActor ticked"));

   // m_mesh->AddRelativeRotation(FRotator(0, 0, 2));

    /*if (m_healthBar != nullptr)
        m_healthBar->SetRelativeLocation(GetActorLocation() + FVector(20, 0, 0));*/
}

