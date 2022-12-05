#pragma once

#include <vector>
#include <utility>
#include <unordered_map>

#include <Engine/Classes/Components/ActorComponent.h>

#include "ObjectParameters.h"

namespace AtomDestiny
{
    ///
    /// Represents Parameterizable Atom Destiny object.
    /// Minimal entity for any Atom Destiny object.
    /// (Units, Buildings, Abilities).
    ///
    class ADObject : public UActorComponent, public IParameterizable
    {
        using GameObjectPairParameterList = std::pair<std::vector<GameObjectWeak>, std::vector<ParameterEnhancement>>;

        ///
        /// Type for any Atom Destiny parameters
        ///
        using ObjectEnhancementParameters = std::unordered_map<ObjectParameters, GameObjectPairParameterList>;

    public:
        using UActorComponent::UActorComponent;
        
        virtual void AddParameter(ObjectParameters parameter, const ParameterEnhancement& enhancement) override;
        virtual void RemoveParameter(ObjectParameters parameter, const GameObject& enhancementObject) override;
        virtual void ZeroParameter(ObjectParameters parameter, const ParameterZeroPack& pack) override;
        
        /// Interprets ParameterEnhancement values and returns resulting value
        static float InterpretParameterModifier(float baseValue, const ParameterEnhancement& enhancement);

    protected:
        
        // Recalculates parameter for implementor
        virtual void RecalculateParameter(ObjectParameters parameter) = 0;
        
        // Set parameter to zero
        virtual void ZeroizeParameter(ObjectParameters parameter) = 0;
        
        // Returns current parameters key types
        std::vector<ObjectParameters> GetParameterTypes() const;
        std::vector<ParameterEnhancement>& GetParameterEnhancementList(ObjectParameters parameter);
        
        // Returns status of parameter
        bool GetParameterAvailable(ObjectParameters parameter);
        std::vector<GameObjectWeak>& GetParameterZeroList(ObjectParameters parameter);
        
        // Adds parameter to object parameters
        void AddNewParameter(ObjectParameters parameter);
        void AddNewParameters(const std::vector<ObjectParameters>& parameters);
        
        // Adds enhancement value to parameter
        void AddToParameter(ObjectParameters parameter, const ParameterEnhancement& enhancement);
        
        // Removes enhancement value from parameter
        void RemoveFromParameter(ObjectParameters parameter, const GameObject& enhanceObject);

    private:
        
        // Removes all parameters additional values which are null
        void RemoveNullParameters(ObjectParameters parameter);
        
        // Recalculates any parameter
        void Recalculate(ObjectParameters parameter);
        
        ObjectEnhancementParameters m_enhancementParameters;
    };

} // namespace AtomDestiny
