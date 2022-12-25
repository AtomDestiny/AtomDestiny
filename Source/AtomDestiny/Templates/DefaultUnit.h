#pragma once

class UUnitParameters;
class UUnitState;
class UUnitLogic;
class USceneComponent;

#include <Runtime/Engine/Classes/GameFramework/Pawn.h>

#include "DefaultUnit.generated.h"

///
/// Represents default unit with basic components.
/// It consists of Box collider, Unit parameters (heath and defence),
/// Unit state (massive API to unit info and control), Ground point, Movement component and
/// Unit logic (AI control component).
///
UCLASS(BlueprintType, Blueprintable, meta=(ShortTooltip="Default unit with basic needed components to participate at Atom Destiny game"))
class ADefaultUnit : public APawn
{
    GENERATED_BODY()
    
public:
    explicit ADefaultUnit(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

protected:
    TObjectPtr<USceneComponent> m_boxComponent;
    TObjectPtr<USceneComponent> m_groundPoint;
    TObjectPtr<UUnitState> m_unitState;
    TObjectPtr<UUnitLogic> m_unitLogic;
    TObjectPtr<UUnitParameters> m_unitParameters;
};
