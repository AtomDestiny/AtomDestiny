#pragma once

///
/// All units in game.
/// Add new unit types here.
///
UENUM(BlueprintType)
enum class EUnitType : uint8
{
    None = 0,
    Shooter,
    Lancer,
};

///
/// Unit death match size.
/// Also it uses at Battle mode, as Unit energy consumption.
///
UENUM(BlueprintType)
enum class EUnitSize : uint8
{
    None,
    Small = 1,
    Medium = 2,
    Huge = 3
};
