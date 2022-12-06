#pragma once

namespace AtomDestiny::Weapon
{
    ///
    /// Base interface for weapon animation
    ///
    struct IWeaponAnimation
    {
        virtual ~IWeaponAnimation() = default;

        // Returns weapon ready status
        virtual bool IsReady() = 0;

        // Plays animation
        virtual void Animate() = 0;

        // Sets default state of animation
        virtual void SetDefaultState() = 0;
    };

} // namespace AtomDestiny
