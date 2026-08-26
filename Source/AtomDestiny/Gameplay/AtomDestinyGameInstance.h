#pragma once

#include "Engine/GameInstance.h"

#include "AtomDestinyGameInstance.generated.h"

///
/// Startup module: copies the Unit Catalog into UnitStorage
/// so any map (including Training) can build unit cards.
///
UCLASS()
class ATOMDESTINY_API UAtomDestinyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
};
