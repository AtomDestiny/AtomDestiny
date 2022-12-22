#pragma once

#include <Runtime/Core/Public/Delegates/Delegate.h>

#include <AtomDestiny/Logic/Logic.h>
#include <AtomDestiny/Core/ADObject/ADObject.h>
#include <AtomDestiny/Unit/Animation.h>
#include <AtomDestiny/Navigation/Navigator.h>

#include "UnitLogicBase.generated.h"

///
/// Represents abstract unit logic with common values.
/// Should be derived by concrete logic.
///
UCLASS(Abstract)
class ATOMDESTINY_API UUnitLogicBase : public UADObject, public ILogic
{
    GENERATED_BODY()
    
    // Any unit actions
    UDELEGATE()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUnitAction, AActor*, actor, EGameSide, side, EUnitType, unitType);
    
public:
    
    // Returns list of all unit weapons
    virtual const TArray<TScriptInterface<IWeapon>>& GetAllWeapon() const override;
    
    // Returns unit size (for energy and death match mode)
    virtual EUnitSize GetSize() const override;
    
    // Returns unit side
    virtual EGameSide GetSide() const override;
    
    // Returns unit cost
    virtual int32 GetCost() const override;
    
    // Returns unit type
    virtual EUnitType GetUnitType() const override;
    
    // Returns current velocity
    virtual double GetVelocity() const override;
    
    // Sets side
    virtual void SetSide(EGameSide side) override;
    
    // Updates unit needs
    virtual void UpdateParameters() ABSTRACT_METHOD;
    
    // Sets destination to transform
    virtual void SetDestination(AActor* destination) ABSTRACT_METHOD;
    
    /// Sets destination to vector point
    virtual void SetDestinationByPoint(const FVector& destination) ABSTRACT_METHOD;

    ///
    /// events
    ///
    
    // Emits when unit created
    inline static FUnitAction unitCreated;

    // Emits when unit destroyed
    inline static FUnitAction unitDestroyed;

protected:

    virtual void InitializeComponent() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
    
    // Checks and rotates actor to target
    void RotateToTarget(float deltaTime);
    
    // Checks hashed scan delay
    void CheckScanDelay(float deltaTime);
    
    // Calculates logic distances values
    void CalculateDistances();
    
    // Recalculates speed parameter
    virtual void RecalculateParameter(EObjectParameters parameter) override;
    
    // Sets parameter to zero value
    virtual void ZeroizeParameter(EObjectParameters parameter) override;
    
    void CreateMessage() const;
    void DestroyMessage() const;
    
    // All unit weapons references
    TArray<TScriptInterface<IWeapon>> m_weapons;
    
    // Scan distance (calculated from max attack range + attack delta range)
    double m_scanDistance;
    
    // Minimal scan distance (choose from min attack range from all weapons)
    double m_minScanDistance;
    
    // Started unit speed
    double m_speed = 0;
    
    // Current unit speed value
    double m_currentSpeed = 0;
    
    // Scan delta for unit scan range
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Attack delta range"))
    double m_attackDeltaRange = 5.0;
    
    // Max angle between unit forward and enemy
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Attack angle"))
    double m_attackAngle = 5.0;
    
    // Default stop distance (from navigation stop distance)
    double m_defaultStopDistance = 0;
    
    // Delta range for unit attack
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Try attack delta"))
    double m_tryAttackDelta = 1.2;
    
    // Rotate angular speed when unit controls by script not by navigation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Rotate speed"))
    double m_rotateSpeed = 2.0;
    
    // Game unit side
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Game side"))
    EGameSide m_side = EGameSide::Rebels;
    
    // Game unit type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit type"))
    EUnitType m_unitType;
    
    // Game unit size
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit size"))
    EUnitSize m_unitSize;
    
    // Unit mineral cost
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit cost"))
    int32 m_unitCost = 1;
    
    // Enemy scan delay base
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Scan delay"))
    double m_scanDelay = 0.125;
    
    // Stores main destination reference
    TWeakObjectPtr<AActor> m_mainDestination;
    
    // Stores current destination reference
    TWeakObjectPtr<AActor> m_currentDestination;
    
    // NavMeshAgent reference
    TWeakObjectPtr<ANavigator> m_navigation;
    
    // Target found state flag
    bool m_isTargetFound = false;
    
    // Attacking state flag
    bool m_isAttacking = false;
    
    // Rotated on target state flag
    bool m_isRotatedOnTarget = false;
    
    // Scan state flag
    bool m_canScan = true;
    
    // Hashed destination point
    FVector m_destinationPoint;
    
    // Animation controller (unit animation interface)
    TScriptInterface<IAnimation> m_animation;
    
    // Stores hashed scan delay time
    double m_scanDelayCounter = 0;
    
    // Defines unit logic behaviour type
    EUnitBehaviour m_behaviour = EUnitBehaviour::MoveToTransform;
};
