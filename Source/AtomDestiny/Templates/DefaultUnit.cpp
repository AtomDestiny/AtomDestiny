#include "DefaultUnit.h"

#include <Runtime/Engine/Classes/Components/BoxComponent.h>

#include <AtomDestiny/Unit/UnitState.h>
#include <AtomDestiny/Unit/UnitParameters.h>
#include <AtomDestiny/Unit/UnitMovementComponent.h>

#include <AtomDestiny/Logic/UnitLogic.h>
#include <AtomDestiny/ObjectState/UnitBasicDestroy.h>

ADefaultUnit::ADefaultUnit(const FObjectInitializer& objectInitializer):
    APawn(objectInitializer)
{
    m_boxComponent = objectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxCollider"));
    
    if (RootComponent == nullptr)
    {
        RootComponent = m_boxComponent;
    }

    m_groundPoint = objectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("GroundPoint"));
    m_unitState = objectInitializer.CreateDefaultSubobject<UUnitState>(this, TEXT("UnitState"));
    m_unitParameters = objectInitializer.CreateDefaultSubobject<UUnitParameters>(this, TEXT("UnitParameters"));
    m_unitLogic = objectInitializer.CreateDefaultSubobject<UUnitLogic>(this, TEXT("UnitLogic"));
    m_unitMovement = objectInitializer.CreateDefaultSubobject<UUnitMovementComponent>(this, TEXT("UnitMovement"));
    m_unitDestroy = objectInitializer.CreateDefaultSubobject<UUnitBasicDestroy>(this, TEXT("UnitDestroy"));

    // setup ground point and default AI controller
    m_unitState->SetGroundPoint(MakeWeakObjectPtr(m_groundPoint.Get()));
    AIControllerClass = ANavigator::StaticClass();
}
