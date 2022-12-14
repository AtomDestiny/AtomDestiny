#pragma once

#include <memory>

#include <AtomDestiny/Unit/Unit.h>

#include <Runtime/Engine/Classes/GameFramework/GameModeBase.h>
#include <Core/Public/Templates/UnrealTemplate.h>

#include "AtomDestinyGameModeBase.generated.h"

///
/// Represents super base core class for AtomDestiny game.
/// You should use this class as base for any game mode.
/// To has a better compile time and scaling it stores data as pointer implementation.
///
UCLASS()
class ATOMDESTINY_API AAtomDestinyGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AAtomDestinyGameModeBase();
    virtual ~AAtomDestinyGameModeBase() override;
    
    void AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side);
    void RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side);

protected:
    
    ///
    /// Event callbacks
    ///
    
    UFUNCTION()
    void OnUnitCreated(AActor* actor, EGameSide side, EUnitType unitType);
    
    UFUNCTION()
    void OnUnitDestroyed(AActor* actor, EGameSide side, EUnitType unitType);

private:
    struct GameModeBasePrivateData;
    GameModeBasePrivateData* m_impl = nullptr; // not std::unique_ptr because of virtual destructor
};
