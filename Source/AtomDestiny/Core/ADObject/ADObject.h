#pragma once

#include <tuple>
#include <vector>
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
        using GameObjectPairParameterList = std::pair<std::vector<GameObject>, std::vector<ParameterEnhancement>>;

        ///
        /// Type for any Atom Destiny parameters
        ///
        using ObjectEnhancementParameters = std::unordered_map<ObjectParameters, GameObjectPairParameterList>;

    public:
        using UActorComponent::UActorComponent;

    protected:
        
        // Recalculates parameter for implementor
        virtual void RecalculateParameter(ObjectParameters parameter) = 0;
        
        // Set parameter to zero
        virtual void ZeroizeParameter(ObjectParameters parameter) = 0;
        
        // Returns current parameters key types
        std::vector<ObjectParameters> GetParameterTypes() const;
        std::vector<ParameterEnhancement>& GetParameterEnhancementList(ObjectParameters parameter);
        
        // Adds parameter to object parameters
        void AddNewParameter(ObjectParameters parameter);
        void AddNewParameters(const std::vector<ObjectParameters>& parameters);
        
        // Adds enhancement value to parameter
        void AddToParameter(ObjectParameters parameter, const ParameterEnhancement& enhancement);
        
        /// <summary>
        /// Removes enhancement value from parameter
        /// </summary>
        /// <param name="parameter"></param>
        /// <param name="enhanceObject"></param>
        /// <returns></returns>
        private void RemoveFromParameter(Core.UnitParameters parameter, GameObject enhanceObject)
        {
            if (parameters.ContainsKey(parameter))
            {
                var list = GetParameterEnhanceList(parameter);
                var index = list.FindIndex(obj => ReferenceEquals(obj.enhancementObject, enhanceObject));

                if (index != Core.NotFound)
                {
                    list.RemoveAt(index);

                    if (GetParameterAvailable(parameter))
                        Recalculate(parameter);
                }
            }
            else
                Debug.Log(string.Format("{0} does not have {1} parameter", GetType().Name, parameter.ToString()));
        }
        
        /// <summary>
        /// Returns parameter zero list
        /// </summary>
        /// <param name="parameter"></param>
        /// <returns></returns>
        protected List<GameObject> GetParameterZeroList(Core.UnitParameters parameter)
        {
            return parameters[parameter].First;
        }

        /// <summary>
        /// Returns status of parameter
        /// </summary>
        /// <param name="parameter"></param>
        /// <returns></returns>
        protected bool GetParameterAvailable(Core.UnitParameters parameter)
        {
            if (parameters.ContainsKey(parameter))
            {
                parameters[parameter].First.RemoveAll(obj => obj == null);

                return !System.Convert.ToBoolean(parameters[parameter].First.Count);
            }

            return false;
        }

        /// <summary>
        /// Removes all parameters additional values which are null
        /// </summary>
        /// <param name="parameter"></param>
        private void RemoveParameterNulls(Core.UnitParameters parameter)
        {
            if (parameters.ContainsKey(parameter))
                parameters[parameter].Second.RemoveAll(obj => obj.enhancementObject = null);
        }

        /// <summary>
        /// Recalculates any parameter
        /// </summary>
        /// <param name="parameter"></param>
        private void Recalculate(Core.UnitParameters parameter)
        {
            RemoveParameterNulls(parameter);

            if (GetParameterAvailable(parameter))
                RecalculateParameter(parameter);
        }

        /// <summary>
        /// Interprets and returns float value
        /// </summary>
        /// <param name="baseValue"></param>
        /// <param name="enhancement"></param>
        /// <returns></returns>
        public static float InterpretParameterModifier(float baseValue, Core.ParameterEnhancement enhancement)
        {
            var result = 0.0f;
            var value = Mathf.Abs(enhancement.value);

            switch (enhancement.modifier)
            {
                case Core.ParameterModifier.Plus:
                    result = value;
                    break;

                case Core.ParameterModifier.PlusCoeff:
                    result = baseValue * value;
                    break;

                case Core.ParameterModifier.Minus:
                    result = -value;
                    break;

                case Core.ParameterModifier.MinusCoeff:
                    result = -(baseValue * value);
                    break;

                default:
                    break;
            }

            return result;
        }

        /// <summary>
        /// Adds additional value to parameter
        /// </summary>
        /// <param name="param"></param>
        /// <param name="value"></param>
        public void AddParameter(Core.UnitParameters parameter, Core.ParameterEnhancement enhancement)
        {
            if (enhancement.enhancementObject != null && enhancement.value >= 0)
                AddToParameter(parameter, enhancement);
        }

        /// <summary>
        /// Removes additional value from parameter
        /// </summary>
        /// <param name="parameter"></param>
        /// <param name="enhancementObject"></param>
        /// <returns></returns>
        public void RemoveParameter(Core.UnitParameters parameter, GameObject enhancementObject)
        {
            if (enhancementObject != null)
                RemoveFromParameter(parameter, enhancementObject);
        }

        /// <summary>
        /// Sets parameter to zero or unzero
        /// </summary>
        /// <param name="parameter"></param>
        public void ZeroParameter(Core.UnitParameters parameter, Core.ParameterZeroPack pack)
        {
            if (parameters.ContainsKey(parameter) && pack.zeroObject != null)
            {
                var zeroList = parameters[parameter].First;

                if (pack.status)
                {
                    if (!zeroList.Contains(pack.zeroObject))
                    {
                        zeroList.Add(pack.zeroObject);
                        ZeroizeParameter(parameter);
                    }
                }
                else
                    zeroList.Remove(pack.zeroObject);

                Recalculate(parameter);
            }
            else
                Debug.Log(string.Format("{0} does not have {1} parameter", GetType().Name, parameter.ToString()));
        }

    private:
        ObjectEnhancementParameters m_enhancementParameters;
    };

} // namespace AtomDestiny
