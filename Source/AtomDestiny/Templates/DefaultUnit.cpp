#include "DefaultUnit.h"

#include <Components/BoxComponent.h>
#include <Components/WidgetComponent.h>

#include <AtomDestiny/Unit/UnitState.h>
#include <AtomDestiny/Unit/UnitParameters.h>
#include <AtomDestiny/Unit/UnitMovementComponent.h>

#include <AtomDestiny/Logic/UnitLogic.h>
#include <AtomDestiny/ObjectState/UnitBasicDestroy.h>

#include <Kismet/GameplayStatics.h>

ADefaultUnit::ADefaultUnit(const FObjectInitializer& objectInitializer):
    APawn(objectInitializer)
{
    
    
    if (RootComponent == nullptr)
    {
        m_boxComponent = objectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxCollider"));
        RootComponent = m_boxComponent;
    }

    m_groundPoint = objectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("GroundPoint"));
    m_groundPoint->SetupAttachment(RootComponent);
    
    m_healthBar = objectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("HealthBar"));
    m_healthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    m_healthBar->SetUsingAbsoluteLocation(true);
    
    m_unitState = objectInitializer.CreateDefaultSubobject<UUnitState>(this, TEXT("UnitState"));
    m_unitParameters = objectInitializer.CreateDefaultSubobject<UUnitParameters>(this, TEXT("UnitParameters"));
    m_unitLogic = objectInitializer.CreateDefaultSubobject<UUnitLogic>(this, TEXT("UnitLogic"));
    m_unitMovement = objectInitializer.CreateDefaultSubobject<UUnitMovementComponent>(this, TEXT("UnitMovement"));
    m_unitDestroy = objectInitializer.CreateDefaultSubobject<UUnitBasicDestroy>(this, TEXT("UnitDestroy"));

    // setup ground point and default AI controller
    m_unitState->SetGroundPoint(MakeWeakObjectPtr(m_groundPoint.Get()));
    AIControllerClass = ANavigator::StaticClass();
}

void ADefaultUnit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    auto const& playerTfm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransform();
    if (!playerTfm.IsValid())
        UE_LOG(LogTemp, Warning, TEXT("Invalid player transform!"));
	
    FVector const Up = playerTfm.TransformVector({0,0,1});

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Axis is: %s"), *Up.ToString()));
	
    m_healthBar->SetWorldLocation(GetActorLocation() + Up * 420);
}
