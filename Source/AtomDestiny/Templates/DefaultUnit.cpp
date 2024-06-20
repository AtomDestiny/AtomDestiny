#include "DefaultUnit.h"

#include <Components/BoxComponent.h>

#include <AtomDestiny/Unit/UnitState.h>
#include <AtomDestiny/Unit/UnitParameters.h>
#include <AtomDestiny/Unit/UnitMovementComponent.h>
#include <AtomDestiny/Unit/UnitScrapDestroy.h>
#include <AtomDestiny/UI/HealthBarComponent.h>

#include <AtomDestiny/Logic/UnitLogic.h>

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
    
    m_healthBar = objectInitializer.CreateDefaultSubobject<UHealthBarComponent>(this, TEXT("HealthBar"));
    m_healthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    
    m_unitState = objectInitializer.CreateDefaultSubobject<UUnitState>(this, TEXT("UnitState"));
    m_unitParameters = objectInitializer.CreateDefaultSubobject<UUnitParameters>(this, TEXT("UnitParameters"));
    m_unitLogic = objectInitializer.CreateDefaultSubobject<UUnitLogic>(this, TEXT("UnitLogic"));
    m_unitMovement = objectInitializer.CreateDefaultSubobject<UUnitMovementComponent>(this, TEXT("UnitMovement"));
    m_unitDestroy = objectInitializer.CreateDefaultSubobject<UUnitScrapDestroy>(this, TEXT("UnitScrapDestroy"));
    
    // setup ground point and default AI controller
    m_unitState->SetGroundPoint(MakeWeakObjectPtr(m_groundPoint.Get()));
    AIControllerClass = ANavigator::StaticClass();
}

void ADefaultUnit::BeginPlay()
{
    Super::BeginPlay();
    
    // Setup Health bar here because HealthBarComponent creates it after BeginPlay 
    const auto healthBar = Cast<UHealthBar>(m_healthBar->GetUserWidgetObject());
    healthBar->SetEnergyVisible(false);
    
    m_unitParameters->SetHealthBarWidget(healthBar);
}
