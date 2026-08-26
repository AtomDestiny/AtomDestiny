#include "AtomDestinyStartMenuGameState.h"

#include <AtomDestiny/Gameplay/UnitStorage.h>

void AAtomDestinyStartMenuGameState::PreInitializeComponents()
{
    Super::PreInitializeComponents();
    
    std::call_once(Initialized, [this] {
        if (m_units.Num() > 0)
        {
            AtomDestiny::UnitStorage::Instance().Add(m_units);
        }
    });
}
