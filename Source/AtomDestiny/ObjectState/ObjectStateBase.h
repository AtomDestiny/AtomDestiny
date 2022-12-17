#pragma once

#include <Runtime/UMG/Public/Components/Widget.h>
#include <Runtime/UMG/Public/Components/Slider.h>

#include <AtomDestiny/Core/ADObject/ADObject.h>
#include <AtomDestiny/ObjectState/ObjectState.h>

#include "ObjectStateBase.generated.h"

///
/// Represents base class for any object
/// to have a parameters in Atom Destiny world.
///
UCLASS(Abstract, Blueprintable)
class UObjectStateBase : public UADObject, public IObjectState
{
    GENERATED_BODY()
    
public:
    
    // Adds damage to this object
    virtual void AddDamage(double damage, EWeaponType type, AActor* owner) ABSTRACT_METHOD;
    
    // Healers and hear buffs only
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void RegenerateHealth(double health) override;
    
    // Returns health max flag
    virtual bool IsHealthMax() const override;
    
    // Returns object current health
    virtual double GetHealth() const override;
    
    // Returns object defence type
    virtual EDefenceType GetDefenceType() const override;
    
    // Returns current defence
    virtual double GetDefence() const override;
    
    // Returns current max health
    virtual double GetMaxHealth() const override;
    
    // Returns start max health
    virtual double GetBaseMaxHealth() const override;
    
    // Returns start defence
    virtual double GetBaseDefence() const override;
    
    // Returns additional params
    virtual const FBalanceParameters& GetDefenceAdditionalParameters() const override;

private:

    virtual void InitializeComponent() override;
    
protected:
    
    // Calculates damage with defence additional parameters
    double GetDamageAfterDefenceParameters(EWeaponType type, double damage) const;
    
    /// Calculates damage after defence type
    double GetDamageAfterDefenceType(float damage) const;
    
    // Recalculates speed parameter
    virtual void RecalculateParameter(EObjectParameters parameter) override;
    
    // Sets parameter to zero value
    virtual void ZeroizeParameter(EObjectParameters parameter) override;
    
    // Current object health
    double m_currentHealth = 0;
    
    // Object max health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max health"))
    double m_maxHealth = 10.0f;
    
    // Object defence parameter
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Defence"))
    double m_defence = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Heath bar blueprint"))
    TWeakObjectPtr<UWidget> m_healthBlueprint;
    TWeakObjectPtr<USlider> m_healthBar = nullptr;
    
    // Show/hide bar when current health is max
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Hide bar"))
    bool m_hideBar = false;
    
    // Object defence type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Defence type"))
    EDefenceType m_defenceType = EDefenceType::Medium;
    
    // Additional parameters for unit defence balance, values should represent percents
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Balance parameters"))
    FBalanceParameters m_balanceParameters;
    
    // Dead state
    bool m_isDead = false;
    
    // Current object defence
    double m_currentDefence = 0;
    
    // Current object max health
    double m_currentMaxHealth = 0;
};
