#pragma once

#include <AtomDestiny/AtomDestiny.h>
#include <AtomDestiny/Logic/UnitLogicBase.h>

#include "UnitLogic.generated.h"

///
/// Represents base unit behaviour and AI.
/// It controls over animation, weapon and movement.
///
UCLASS(Abstract, Blueprintable)
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

protected:

    virtual void BeginPlay() override;
    virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* func) override;
    
private:

    // Creates default navigation
    void CreateDestination();
    
    // Base destination unit checking
    void CheckNavigation();
    
    // Reset navigation
    void SetDefaultDestination();
    
    // Updates navigation data
    void UpdateNavigationTarget();
    void MoveNearestEnemyIfCan();
    
    // Try to search enemy at possible distance
    void ScanEnemy();
    void TryToAttack(float deltaTime);
    void Aim(const TScriptInterface<IWeapon>& weapon, float deltaTime);
    
    // Checks unit navigation and animation
    void CheckBehaviour(const TScriptInterface<IWeapon>& weapon);

    // searches nearest enemy
    TWeakObjectPtr<AActor> FindEnemy(double minScanDistance, double scanDistance) const;
};
