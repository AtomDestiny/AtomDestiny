#pragma once

#include "AtomDestiny/Core/GameObject.h"

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

    ///
    /// Interface implementation can change Atom destiny object parameters
    ///
    struct IParameterizable
    {
        virtual ~IParameterizable() = 0;
        
        // Adds additional value to parameter
        virtual void AddParameter(ObjectParameters parameter, const ParameterEnhancement& enhancement) = 0;
        
        // Removes additional value from parameter
        virtual void RemoveParameter(ObjectParameters parameter, const GameObject& enhancementObject) = 0;
        
        // Sets parameter to zero or non zero
        virtual void ZeroParameter(ObjectParameters parameter, const ParameterZeroPack& pack) = 0;
    };

} // namespace AtomDestiny
