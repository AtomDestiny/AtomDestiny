#pragma once

#include <memory>

#include <Core/Public/Templates/UnrealTemplate.h>

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

        UENUM(BlueprintType)
        enum class GameSide : uint8
        {
            None,
            Rebels,
            Federation,
            Neutral,
            NeutralBuildings,
        };
        
        // Returns Core object as Meyers singleton
        static Core& Instance();

    private:
        struct CoreData;
        std::unique_ptr<CoreData> m_impl;
    };
    
} // namespace AtomDestiny
