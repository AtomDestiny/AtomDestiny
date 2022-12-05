#pragma once

#include "AtomDestiny/Unit/Unit.h"
#include "AtomDestiny/Weapon/Weapon.h"
#include "AtomDestiny/Core/AtomDestinyCore.h"

namespace AtomDestiny::Logic
{
    using namespace Weapon;

    ///
    /// Unit base logic interface
    ///
    struct ILogic
    {
        virtual ~ILogic() = default;

        // Returns current unit side
        virtual Core::GameSide GetSide() const = 0;

        // Returns current unit type
        virtual Unit::UnitType GetUnitType() const = 0;

        // Returns current unit size
        virtual Unit::UnitSize GetSize() const = 0;

        // Returns current unit cost
        virtual int GetCost() const = 0;

        // Returns logic binded weapon collections.
        // Do not cache it. Use and drop reference then.
        virtual TArray<IWeapon*>& GetAllWeapon() = 0;

        // Returns max base unit velocity
        virtual float GetVelocity() const = 0;

        // Sets side to unit
        virtual void SetSide(Core::GameSide side) = 0;

        // Sets destination to other Actor/Point
        virtual void SetDestination(const GameObjectWeak& destination) = 0;

        // Overloaded setting destination to logic as point
        virtual void SetDestination(FVector destination) = 0;

        // Recalculates base unit parameters
        virtual void UpdateParameters() = 0;
    };

} // namespace AtomDestiny::Logic

