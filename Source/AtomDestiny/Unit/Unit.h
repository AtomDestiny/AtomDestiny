#pragma once

namespace AtomDestiny::Unit
{
    ///
    /// All units in game.
    /// Add new unit types here.
    ///
    UENUM(BlueprintType)
    enum class UnitType : uint8
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
    enum class UnitSize : uint8
    {
        None,
        Small = 1,
        Medium = 2,
        Huge = 3
    };

} // namespace AtomDestiny::Unit
