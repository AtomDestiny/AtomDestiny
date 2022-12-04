#pragma once

#include "GameObject.h"

namespace AtomDestiny
{
    ///
    /// Represents parameters that can be exists at base Atom Destiny object
    ///
    enum class ObjectParameters
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
        enum class Modifier : uint8_t
        {
            Plus,
            PlusCoefficient,
            Minus,
            MinusCoefficient
        };
        
        GameObject enhancementObject;
        float value;
        Modifier modifier;
    };
    
} // namespace AtomDestiny
