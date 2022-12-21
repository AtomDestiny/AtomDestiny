#pragma once

///
/// All units in game.
/// Add new unit types here.
///
UENUM(BlueprintType)
enum class EUnitType : uint8
{
    Shooter,
    Lancer,
    None
};

///
/// Unit death match size.
/// Also it uses at Battle mode, as Unit energy consumption.
///
UENUM(BlueprintType)
enum class EUnitSize : uint8
{
    Small = 0,
    Medium,
    Huge,
    None
};

///
/// Defines to unit what to do.
/// Means work for navigation.
///
UENUM(BlueprintType)
enum class EUnitBehaviour : uint8
{
    MoveToPoint,
    MoveToTransform,
    Standing
};

namespace AtomDestiny::Unit
{
    constexpr static double MinRandomScan = 0.020;
    constexpr static double MaxRandomScan = 0.180;
    
} // namespace AtomDestiny::Unit
