#pragma once

///
/// Represents parameters that can be exists at base Atom Destiny object
///
UENUM(BlueprintType)
enum class EObjectParameters : uint8
{
    Damage,
    CriticalRate,
    CriticalChance,
    Range,
    MinRange,
    ExplosionRadius,
    Reload,
    Defence,
    MaxHealth,
    Health,
    Absorbation,
    MaxShield,
    Shield,
    Velocity
};
