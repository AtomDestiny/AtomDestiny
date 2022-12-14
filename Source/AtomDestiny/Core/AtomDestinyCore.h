#pragma once

#include <memory>

#include <Core/Public/Templates/UnrealTemplate.h>

#include <AtomDestiny/Unit/Unit.h>

UENUM(BlueprintType)
enum class EGameSide : uint8
{
    None,
    Rebels,
    Federation,
    Neutral,
    NeutralBuildings,
};

namespace AtomDestiny
{
    ///
    /// Represents super base core class for AtomDestiny game.
    /// It has Singleton realization, be aware to store some data at Core.
    /// To has a better compile time and scaling it stores data as pointer implementation.
    ///
    class Core : public FNoncopyable
    {
    protected:
        Core();
        ~Core();

    public:
        
        // Returns Core object as Meyers singleton
        static Core& Instance();

        void AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side);
        void RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side);

    private:
        
        ///
        /// Event callbacks
        ///
        
        void OnUnitCreated(const TWeakObjectPtr<AActor>& actor, EGameSide side, EUnitType);
        void OnUnitDestroyed(const TWeakObjectPtr<AActor>& actor, EGameSide side, EUnitType);

    private:
        struct CoreData;
        std::unique_ptr<CoreData> m_impl;
    };
    
} // namespace AtomDestiny
