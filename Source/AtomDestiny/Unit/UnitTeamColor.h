#pragma once

#include <Components/ActorComponent.h>

#include "AtomDestiny/AtomDestiny.h"

#include "UnitTeamColor.generated.h"

namespace AtomDestiny::TeamColor
{
    ///
    /// Paints all actor primitives with the side team color.
    /// Only materials with "TeamColor" parameter marked as Custom Primitive Data are affected.
    ///
    ATOMDESTINY_API void Apply(const AActor* actor, EGameSide side);

} // namespace AtomDestiny::TeamColor

///
/// Keeps unit painted with its side team color (Project Settings -> Conflict Sides).
/// Repaints the unit when the logic side changes.
///
UCLASS(ClassGroup = (AtomDestiny), meta = (BlueprintSpawnableComponent))
class ATOMDESTINY_API UUnitTeamColor : public UActorComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
    void OnSideChanged(EGameSide side) const;

    FDelegateHandle m_sideChangedHandle;
};
