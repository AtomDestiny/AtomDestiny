#pragma once

#include "BalanceParameters.generated.h"

///
/// Data struct for defence additional parameters.
/// You can setup them later to 'balance' unit by weapon type.
///
USTRUCT()
struct FBalanceParameters
{
    GENERATED_BODY()

    double ballisticResistance;
    double energyResistance;
    double explosiveResistance;
    double elementalResistance;
};
