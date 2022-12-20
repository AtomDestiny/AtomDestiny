#pragma once

#include "BalanceParameters.generated.h"

///
/// Data struct for defence additional parameters.
/// You can setup them later to 'balance' unit by weapon type.
///
USTRUCT(BlueprintType)
struct FBalanceParameters
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Ballistic resistance"))
    double ballisticResistance = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Energy resistance"))
    double energyResistance = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Explosive resistance"))
    double explosiveResistance = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Elemental resistance"))
    double elementalResistance = 0;
};

// Represents balance values
namespace AtomDestiny::Balance
{
    /// 
    /// Min damage value that can be added to unit
    /// 
    inline static constexpr double MinDamageValue = 1.0f;
    
    // ballistic vs defence
    inline static constexpr double BallisticVsLightCoefficient = 1.1;
    inline static constexpr double BallisticVsMediumCoefficient = 1.0;
    inline static constexpr double BallisticVsHeavyCoefficient = 0.9;

    // fire vs defence
    inline static constexpr double FireVsLightCoefficient = 0.8;
    inline static constexpr double FireVsMediumCoefficient = 1.0;
    inline static constexpr double FireVsHeavyCoefficient = 1.2;

    // laser vs defence
    inline static constexpr double LaserVsLightCoefficient = 0.7;
    inline static constexpr double LaserVsMediumCoefficient = 1.0;
    inline static constexpr double LaserVsHeavyCoefficient = 1.3;

    // lightning vs defence
    inline static constexpr double LightningVsLightCoefficient = 0.9;
    inline static constexpr double LightningVsMediumCoefficient = 1.0;
    inline static constexpr double LightningVsHeavyCoefficient = 1.1;

    // rocket vs defence
    inline static constexpr double RocketVsLightCoefficient = 1.4;
    inline static constexpr double RocketVsMediumCoefficient = 1.1;
    inline static constexpr double RocketVsHeavyCoefficient = 0.6;

    // for defence type only
    inline static constexpr double VsLightDefenceCoefficient = 1.0;
    inline static constexpr double VsMediumDefenceCoefficient = 0.9;
    inline static constexpr double VsHeavyDefenceCoefficient = 0.8;
    
} // namespace AtomDestiny::Balance
