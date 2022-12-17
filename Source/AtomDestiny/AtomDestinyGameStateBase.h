#pragma once

#include <AtomDestiny/Unit/Unit.h>
#include <AtomDestiny/Gameplay/GameDestination.h>

#include <Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include <Core/Public/Templates/UnrealTemplate.h>

#include "AtomDestinyGameStateBase.generated.h"

///
/// Represents super base core class for AtomDestiny game.
/// You should use this class as base for any game mode.
/// To has a better compile time and scaling it stores data as pointer implementation.
///
UCLASS()
class ATOMDESTINY_API AAtomDestinyGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    AAtomDestinyGameStateBase();
    virtual ~AAtomDestinyGameStateBase() override;
    
    void AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side);
    void RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side);

    TWeakObjectPtr<AActor> GetDestination(EGameSide side) const;

protected:
    
    ///
    /// Event callbacks
    ///
    
    UFUNCTION()
    void OnUnitCreated(AActor* actor, EGameSide side, EUnitType unitType);
    
    UFUNCTION()
    void OnUnitDestroyed(AActor* actor, EGameSide side, EUnitType unitType);

private:
    void InitializeSides();
    void InitializeEnemies();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Game destination"))
    FGameDestination m_destination;
    
    struct GameStateBasePrivateData;
    GameStateBasePrivateData* m_impl = nullptr; // not std::unique_ptr because of virtual destructor
};
