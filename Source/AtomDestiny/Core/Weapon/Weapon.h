#pragma once

#include "WeaponParameters.h"

namespace AtomDestiny::Weapon
{
    ///
    /// Base interface for all weapon controllers
    ///
    struct IWeapon
    {
        virtual ~IWeapon() = default;

        ///
        /// Weapon controls
        ///

        // Sets current target to weapon. Should be called before fire
        virtual void SetTarget(const GameObjectWeak& gameObject) = 0;

        // Weapon attack. Should be called after setTarget
        virtual void Fire() = 0;

        ///
        /// Weapon states
        ///

        // Returns weapon firing state
        virtual bool IsFiring() const = 0;

        // Returns could weapon rotates or not
        virtual bool IsRotated() const = 0;

        // Returns AOE weapon status
        virtual bool IsAoe() const = 0;

        // Returns current weapon type
        virtual WeaponType GetWeaponType() const = 0;

        // Returns weapon state of critical chance
        virtual bool IsCritical() const = 0;

        // Returns target search possibility
        virtual bool IsSeeTarget() const = 0;

        // Should we stop unit when attack? Generally should be used by Unit logic
        virtual bool IsShouldStop() const = 0;

        // Returns weapon use raycast status
        virtual bool IsRaycasted() const = 0;

        // Returns weapon friendly fire status
        virtual bool IsFriendlyFireAvailable() const = 0;

        // Returns some current parameters of weapon
        virtual WeaponParameters GetParameters() const = 0;

        ///
        /// Weapon Base parameters
        ///

        // Returns started critical chance
        virtual float GetBaseCriticalChance() const = 0;

        // Returns started critical rate
        virtual float GetBaseCriticalRate() const = 0;

        // Returns base weapon damage
        virtual float GetBaseDamage() const = 0;

        // Returns base attack range
        virtual float GetBaseAttackRange() const = 0;

        /// Returns started base reload time
        virtual float GetBaseReloadTime() const = 0;
        
        ///
        /// Weapon current parameters
        ///

        // Weapon max attack distance
        virtual float GetAttackRange() const = 0;

        // Weapon min attack distance
        virtual float GetMinAttackRange() const = 0;

        // Returns critical chance
        virtual float GetCriticalChance() const = 0;

        // Returns critical rate
        virtual float GetCriticalRate() const = 0;

        // Returns current weapon firing rate
        virtual float GetFireRate() const = 0;

        // Returns current weapon damage
        virtual float GetDamage() const = 0;

        // Returns reload time
        virtual float GetReloadTime() const = 0;
    };

} // namespace AtomDestiny::Weapon
