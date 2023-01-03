#pragma once

#include <AtomDestiny/Unit/UnitParameters.h>
#include <AtomDestiny/ObjectState/Shield.h>

#include <UE5Coro/Public/UE5Coro.h>

#include "ShieldObjectStateBase.generated.h"

///
/// Represents base class for any object with shield.
/// It stores all parameters for shields.
///
UCLASS(Abstract)
class UShieldObjectStateBase : public UUnitParameters, public IShield
{
    GENERATED_BODY()
    
public:
    
    // Returns current shield value
    virtual double GetUnitShield() const override;
    
    // Returns shield type
    virtual EShieldType GetShieldType() const override;
    
    // Returns current max shield value
    virtual double GetMaxUnitShield() const override;

    // Returns shield absorbation
    virtual double GetShieldAbsorbation() const override;
    
    // Returns start max shield value
    virtual double GetBaseMaxUnitShield() const override;
    
    // Returns start shield absorbation value
    virtual double GetBaseShieldAbsorbation() const override;
    
    // Returns additional balance parameters, by default its empty
    virtual const FBalanceParameters& GetShieldAdditionalParameters() const override;

protected:
    virtual void InitializeComponent() override;
    virtual void BeginPlay() override;

    // ADObject interface
    virtual void RecalculateParameter(EObjectParameters parameter) override;
    virtual void ZeroizeParameter(EObjectParameters parameter) override;
    
    void RegenerateShield();
    FAsyncCoroutine StartShieldCoolDown();
    
    // Calculates damage with shield additional balance parameters
    double GetDamageAfterShieldParameters(EWeaponType type, double damage) const;
    
    // Current shield value
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayName = "Current shield value"))
    double m_currentShieldValue = 0;
    
    // Shield max value
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max shield value"))
    double m_maxShieldValue = 100.0;

    ///
    /// Shield absorbation value.
    /// Similar to defence parameter value.
    ///
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shield absorbation"))
    double m_shieldAbsorbation = 0;

    ///
    /// Shield regeneration value.
    /// Used by every tick, if possible by regeneration time.
    /// 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shield regeneration value"))
    double m_shieldRegenerateValue = 0.5;
    
    // Shield regeneration cool dawn. Time between possible regeneration.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shield regeneration cool dawn"))
    double m_shieldRegenerateTime = 0.1;
    
    // Regeneration state access
    bool m_regenerationAccess = true;
    
    // Object shield type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shield type"))
    EShieldType m_shieldType = EShieldType::Electrical;
    
    // Shield additional parameters for unit shield balance, in percents
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shield balance parameters"))
    FBalanceParameters m_shieldBalanceParameters;
    
    // Current object max shield value
    double m_currentMaxShield = 0;
    
    // Current shield absorbation value
    double m_currentAbsorbation = 0;
};
