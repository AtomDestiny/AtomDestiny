#include "AtomDestinyStartMenuGameState.h"

#include <AtomDestiny/Gameplay/UnitStorage.h>

void AAtomDestinyStartMenuGameState::PreInitializeComponents()
{
    Super::PreInitializeComponents();
    
    std::call_once(Initialized, [this] {
        AtomDestiny::UnitStorage::Instance().Add(m_units);
    });
}
