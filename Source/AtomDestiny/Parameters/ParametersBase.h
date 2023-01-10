#pragma once

#include <AtomDestiny/Core/ADObject/ADObject.h>
#include <AtomDestiny/Parameters/Parameters.h>

#include <UI/HealthBar.h>
#include "ParametersBase.generated.h"

///
/// Represents base class for any object
/// to have a parameters in Atom Destiny world.
///
UCLASS(Abstract)
class UParametersBase : public UADObject, public IParameters
{
    GENERATED_BODY()
    
public:
    explicit UParametersBase(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());
    
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

    void SetHealthBarWidget(TWeakObjectPtr<UHealthBar> widget);

protected:

    virtual void InitializeComponent() override;
    virtual void BeginPlay() override;
    
    // Calculates damage with defence additional parameters
    double GetDamageAfterDefenceParameters(EWeaponType type, double damage) const;
    
    /// Calculates damage after defence type
    double GetDamageAfterDefenceType(float damage) const;

    // use this standard algorithm to take a damage from Atom Destiny projectile
    double GetResultDamage(EWeaponType type, double damage) const;
    
    // Recalculates speed parameter
    virtual void RecalculateParameter(EObjectParameters parameter) override;
    
    // Sets parameter to zero value
    virtual void ZeroizeParameter(EObjectParameters parameter) override;
    
    // Current object health
    UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Current health"))
    double m_currentHealth = 0;
    
    // Object max health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max health"))
    double m_maxHealth = 10.0f;
    
    // Object defence parameter
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Defence"))
    double m_defence = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Health bar widget"))
    TWeakObjectPtr<UHealthBar> m_healthBarWidget;
    //TWeakObjectPtr<USlider> m_healthBar = nullptr;
    
    // Show/hide bar when current health is max
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Hide bar"))
    bool m_hideBar = false;
    
    // Object defence type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Defence type"))
    EDefenceType m_defenceType = EDefenceType::Medium;
    
    // Additional parameters for unit defence balance, values should represent percents
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Balance parameters"))
    FBalanceParameters m_balanceParameters;
    
    bool m_isDead = false;
    
    // Current object defence
    double m_currentDefence = 0;
    
    // Current object max health
    double m_currentMaxHealth = 0;
};
