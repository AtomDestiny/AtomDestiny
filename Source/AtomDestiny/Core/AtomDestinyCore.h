#pragma once

#include <memory>

#include <Core/Public/Templates/SharedPointer.h>

#include <Engine/Classes/GameFramework/Actor.h>

namespace AtomDestiny
{
    using GameObject = TSharedPtr<AActor>;
    using GameObjectWeak = TWeakPtr<AActor>;

    ///
    /// Represents super base core class for AtomDestiny game.
    /// It has Singleton realization, be aware to store some data.
    /// To has a better compile time and scaling, stores data as pointer implementation.
    ///
    class Core : public FNoncopyable
    {
    protected:
        Core();
        ~Core();

    public:

        // Returns Core object as Meyers singleton
        static Core& Instance();

    private:
        struct CoreData;
        std::unique_ptr<CoreData> m_impl;
    };
    
} // namespace AtomDestiny
