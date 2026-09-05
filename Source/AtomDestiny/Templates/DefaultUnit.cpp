#include "DefaultUnit.h"

#include <Components/BoxComponent.h>

#include "AtomDestiny/Behaviour/DestroyBase.h"
#include "AtomDestiny/Logic/UnitLogic.h"
#include "AtomDestiny/Unit/UnitMovementComponent.h"
#include "AtomDestiny/Unit/UnitParameters.h"
#include "AtomDestiny/Unit/UnitScrapDestroy.h"
#include "AtomDestiny/Unit/UnitSideColorDetails.h"
#include "AtomDestiny/Unit/UnitState.h"
#include "AtomDestiny/UI/HealthBarComponent.h"

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
    m_sideColorDetails = objectInitializer.CreateDefaultSubobject<UUnitSideColorDetails>(this, TEXT("SideColorDetails"));

    // Setup ground point and default AI controller
    m_unitState->SetGroundPoint(MakeWeakObjectPtr(m_groundPoint.Get()));
    AIControllerClass = ANavigator::StaticClass();
}

UUnitLogic* ADefaultUnit::ResolveUnitLogic() const
{
    return m_unitLogic != nullptr ? m_unitLogic.Get() : FindComponentByClass<UUnitLogic>();
}

void ADefaultUnit::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (m_unitLogic == nullptr)
    {
        m_unitLogic = FindComponentByClass<UUnitLogic>();
    }
}

void ADefaultUnit::BeginPlay()
{
    Super::BeginPlay();

    // Setup Health bar here because HealthBarComponent creates it after BeginPlay
    const auto healthBar = Cast<UHealthBar>(m_healthBar->GetUserWidgetObject());
    healthBar->SetEnergyVisible(false);

    m_unitParameters->SetHealthBarWidget(healthBar);
}

void ADefaultUnit::OnAcquiredFromPool(const EGameSide side, const EUnitPoolAcquireMode mode)
{
    UUnitLogic* unitLogic = ResolveUnitLogic();

    if (m_bReleasedToPoolOnce)
    {
        if (unitLogic != nullptr)
            unitLogic->ResetForPoolReuse();

        if (m_unitParameters != nullptr)
            m_unitParameters->ResetForPoolReuse();
    }

    if (m_unitDestroy != nullptr)
        m_unitDestroy->ResetForPoolReuse();

    if (unitLogic != nullptr)
    {
        unitLogic->SetSide(side);
        unitLogic->ReregisterWithGameState();

        if (mode == EUnitPoolAcquireMode::SetupPlacement)
            unitLogic->PrepareForSetupPlacement();
    }

    if (m_sideColorDetails != nullptr)
        m_sideColorDetails->ApplyForSide(side);

    m_bPoolAcquirePending = false;
}

void ADefaultUnit::OnReleasedToPool()
{
    m_bReleasedToPoolOnce = true;
    m_bPoolAcquirePending = true;

    if (UUnitLogic* unitLogic = ResolveUnitLogic())
    {
        unitLogic->NotifyPoolReleased();
        unitLogic->PrepareForSetupPlacement();
    }
}
