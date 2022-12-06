#pragma once

#include "AtomDestiny/Core/GameObject.h"

///
/// Represents parameters that can be exists at base Atom Destiny object
///
UENUM()
enum class ObjectParameters : uint8
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

///
/// Stores all params for parameters enhancement.
/// And modifiers to increase/decrease some stats or params.
///
struct ParameterEnhancement
{
    UENUM()
    enum class Modifier : uint8
    {
        Plus,
        PlusCoefficient,
        Minus,
        MinusCoefficient
    };

    GameObjectWeak enhancementObject;
    float value;
    Modifier modifier;
};

///
/// Stores data to understand how to zero parameters
///
struct ParameterZeroPack
{
    GameObjectWeak zeroObject;
    bool status;
};
