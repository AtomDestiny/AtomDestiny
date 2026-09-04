#pragma once

#include <AtomDestiny/AtomDestiny.h>
#include <AtomDestiny/Logic/UnitLogicBase.h>

#include "UnitLogic.generated.h"

///
/// Represents base unit behaviour and AI.
/// It controls over animation, weapon and movement.
///
UCLASS(ClassGroup=(AtomDestiny), Blueprintable, meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API UUnitLogic final : public UUnitLogicBase
{
    GENERATED_BODY()

public:
    explicit UUnitLogic(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    // Recalculates parameters params
    virtual void UpdateParameters() override;

    // Sets destination to unit from actor
    virtual void SetDestination(AActor* destination) override;

    // Sets destination to unit from point
    virtual void SetDestinationByPoint(const FVector& destination) override;

    // Delay AI/navigation until setup placement is finished (Training map)
    void PrepareForSetupPlacement();

    // Start AI after army setup ends
    void ActivateAfterSetup();

    virtual void ResetForPoolReuse() override;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* func) override;

private:
    void CheckTargetDistance();

    // Creates default navigation
    void CreateDestination();

    // Base destination unit checking
    void CheckNavigation();

    // Reset navigation
    void SetDefaultDestination();

    // Updates navigation data
    void UpdateNavigationTarget() const;
    void MoveNearestEnemyIfCan();

    // Try to search enemy at possible distance
    void ScanEnemy();
    void TryToAttack(float deltaTime);
    void Aim(const TScriptInterface<IWeapon>& weapon, float deltaTime);

    // Checks unit navigation and animation
    void CheckBehaviour(const TScriptInterface<IWeapon>& weapon) const;

    // searches nearest enemy
    TWeakObjectPtr<AActor> FindEnemy(double minScanDistance, double scanDistance) const;

    bool m_deferLogicUntilReady = false;
};
