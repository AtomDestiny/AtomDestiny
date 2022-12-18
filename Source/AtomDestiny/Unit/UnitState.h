#pragma once

#include <Engine/Classes/Components/ActorComponent.h>

#include <AtomDestiny/Logic/Logic.h>
#include <AtomDestiny/Unit/Animation.h>
#include <AtomDestiny/Unit/UnitStateParameters.h>

#include <AtomDestiny/Core/ADObject/ObjectParameters.h>
#include <AtomDestiny/Core/ADObject/ParameterEnhancement.h>
#include <AtomDestiny/Core/ADObject/ParameterZeroPack.h>

#include <AtomDestiny/Navigation/Navigator.h>

#include "UnitState.generated.h"

///
/// Represents wrapper class that contains all base API for unit.
/// Also it contains some helper methods.
///
UCLASS(Blueprintable)
class ATOMDESTINY_API UUnitState : public UActorComponent
{
    GENERATED_BODY()

public:
    UUnitState() { bWantsInitializeComponent = true; }
    
    // Returns unit logic
    TScriptInterface<ILogic> GetLogic() const { return m_logic; }
    
    // Returns unit parameters
    TScriptInterface<IObjectState> GetObjectState() const { return m_objectState; }
    
    // Returns unit shield
    TScriptInterface<IShield> GetShield() const { return m_shield; }
    
    // Returns unit animation
    TScriptInterface<IAnimation> GetAnimation() const { return m_animation; }
    
    // Returns ground transform
    TWeakObjectPtr<USceneComponent> GetGroundPoint() const;

    // Returns current health
    double GetCurrentHealth() const { return m_objectState->GetHealth(); }
    
    // Returns unit max health
    double GetMaxHealth() const { return m_objectState->GetMaxHealth(); }
    
    // Returns unit defence value
    double GetDefence() const { return m_objectState->GetDefence(); }
    
    // Returns unit defence type
    EDefenceType GetDefenceType() const { return m_objectState->GetDefenceType(); }
    
    // Returns unit current shield value
    double GetCurrentShieldValue() const { return m_shield != nullptr ? m_shield->GetUnitShield(): 0; }
    
    // Returns unit max shield value
    double GetMaxShieldValue() const { return m_shield != nullptr ? m_shield->GetMaxUnitShield(): 0; }
    
    // Returns shield type
    EShieldType GetShieldType() const { return m_shield != nullptr ? m_shield->GetShieldType(): throw std::runtime_error(" Unit has not shield"); }
    
    // Returns true if unit has shield
    bool HasShield() const { return m_shield != nullptr; }
    
    // Returns unit max velocity
    double GetVelocity() const;
    
    // Returns list of weapons
    const TArray<TScriptInterface<IWeapon>>& GetWeapons() const;
    
    // Returns common weapon firing state
    bool IsWeaponFiring() const;
    
    // Returns unit weapon count
    int32 GetWeaponCount() const { return static_cast<int32>(m_logic->GetAllWeapon().Num()); }
    
    // Returns max weapon damage
    double GetMaxDamage() const;
    double GetMinDamage() const;
    
    // Returns max attack range of all weapons
    double GetMaxAttackRange() const;
    double GetMinAttackRange() const;
    
    // Returns true if any weapon can do critical damage
    bool HasAnyWeaponCritical() const;
    
    // Returns true if any weapon is Aoe
    bool HasAnyWeaponAoe() const;
    
    // Returns unit shield absorbation
    double GetShieldAbsorbation() const { return m_shield != nullptr ? m_shield->GetShieldAbsorbation() : 0; }
    
    // Returns unit mineral const
    int32 GetCost() const { return m_logic->GetCost(); }
    
    // Current unit type
    EUnitType GetUnitType() const { return m_logic->GetUnitType(); }
    
    ///
    /// Stores full info about unit.
    /// This method would collect new all data on every call.
    /// 
    FUnitStateParameters GetUnitState() const;
    
    // Returns true if unit current health is max
    bool IsCurrentHealthMax() const { return m_objectState->IsHealthMax(); }
    
    // Returns unit color side
    EGameSide GetSide() const { return m_logic->GetSide(); }
    
    // Returns unit size
    EUnitSize GetSize() const { return m_logic->GetSize(); }
    
    // Returns start max health
    double GetBaseMaxHealth() const { return m_objectState->GetBaseMaxHealth(); }
    
    // Returns base defence
    double GetBaseDefence() const { return m_objectState->GetBaseDefence(); }
    
    // Returns start max shield value. It may returns zero if unit has not shield.
    double GetBaseMaxShieldValue() const { return m_shield != nullptr ? m_shield->GetBaseMaxUnitShield() : 0; }
    
    // Returns start shield absorbation. It may returns zero if unit has not shield.
    double GetBaseShieldAbsorbation() const { return m_shield != nullptr ? m_shield->GetBaseShieldAbsorbation() : 0; }

    ///
    /// Unit state controls
    ///
    
    // Adds damage to current unit
    void AddDamage(double damage, EWeaponType weaponType, AActor* damageOwner);
    
    // Heals unit 'current health' on health value
    void RegenerateCurrentHealth(double health);
    
    void Disable() { SetEnabled(false); }
    void Enable() { SetEnabled(true); }
    
    // Sets value to current parameter for all weapons
    void AddParameter(EObjectParameters parameter, const FParameterEnhancement& enhancement);
    
    // Removes value from parameter
    void RemoveParameter(EObjectParameters parameter, AActor* enhanceObject);
    
    // Sets parameter to zero value
    void ZeroParameter(EObjectParameters parameter, const FParameterZeroPack& pack);

private:
    
    virtual void InitializeComponent() override;
    
    // Activate/deactivate unit
    void SetEnabled(bool enabled);

    // Returns current navigation from Owner actor
    TWeakObjectPtr<ANavigator> GetNavigation() const;
    
    // Hashed unit logic
    TScriptInterface<ILogic> m_logic = nullptr;
    
    // Hashed unit parameters
    TScriptInterface<IObjectState> m_objectState = nullptr;
    
    // Hashed unit shield
    TScriptInterface<IShield> m_shield = nullptr;
    
    // Hashed unit animation
    TScriptInterface<IAnimation> m_animation = nullptr;

    ///
    /// Represents ground point on unit, by default enemy unit uses this point to navigate its projectile.
    /// Use USceneComponent or derived from type only, it would be checked by run-time.
    ///
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Ground point",
        AllowPrivateAccess = "true", UseComponentPicker))
    FComponentReference m_groundPointReference;
    TWeakObjectPtr<USceneComponent> m_groundPoint;
};
