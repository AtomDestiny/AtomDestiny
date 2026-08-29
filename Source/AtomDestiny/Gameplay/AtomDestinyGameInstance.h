#pragma once

#include "Engine/GameInstance.h"
#include "AtomDestiny/Gameplay/TacticsLayoutElement.h"

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

    void SaveTacticsLayout(const TArray<FTacticsLayoutElement>& layout);

    const TArray<FTacticsLayoutElement>& GetTacticsLayout() const { return m_tacticsLayout; }

    bool HasSavedTacticsLayout() const { return m_tacticsLayout.Num() > 0; }

private:
    UPROPERTY()
    TArray<FTacticsLayoutElement> m_tacticsLayout;
};
