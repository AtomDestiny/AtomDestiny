#pragma once

#include <AtomDestiny/AtomDestiny.h>

#include <AtomDestiny/Unit/Unit.h>
#include <AtomDestiny/Gameplay/GameDestination.h>

#include <AtomDestiny/Projectile/Projectile.h>
#include <AtomDestiny/Parameters/Parameters.h>

#include <Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include <Core/Public/Templates/UnrealTemplate.h>

#include "AtomDestinyGameStateBase.generated.h"

///
/// Represents super base core class for AtomDestiny game.
/// You should use this class as base for any game mode.
/// To has a better compile time and scaling it stores data as pointer implementation.
///
UCLASS(Blueprintable)
class ATOMDESTINY_API AAtomDestinyGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    
    AAtomDestinyGameStateBase();
    virtual ~AAtomDestinyGameStateBase() override = default;
    
    void AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side);
    void RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side);

    TWeakObjectPtr<AActor> GetDestination(EGameSide side) const;

    // use this method to prevent crushes for GetEnemies reference
    bool IsEnemiesExist(EGameSide side) const;
    const FEnemiesList& GetEnemies(EGameSide side) const;
    
    // Adds damage from projectile to object with explosion point parameters
    UFUNCTION()
    static void AddDamage(const TScriptInterface<IProjectile>& projectile, EProjectileDamageOptions options = EProjectileDamageOptions::ProjectilePoint);

    // Adds damage to ObjectState directly
    UFUNCTION()
    static void AddDamageToState(const TScriptInterface<IParameters>& objectState, const FWeaponParameters& parameters);

protected:

    // called by GameMode directly
    virtual void HandleBeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    ///
    /// Event callbacks
    ///
    
    UFUNCTION()
    void OnUnitCreated(AActor* actor, EGameSide side, EUnitType unitType);
    
    UFUNCTION()
    void OnUnitDestroyed(AActor* actor, EGameSide side, EUnitType unitType);
    
    void InitializeSides();
    void InitializeEnemies();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Game destination"))
    FGameDestination m_destination;

    // represents active units at overall battle
    TMap<EGameSide, FSharedGameStateUnitList> m_activeUnits;

    // represents references to Active units, that could be threaded as enemies
    TMap<EGameSide, FEnemiesList> m_enemies;
};

namespace AtomDestiny
{
    inline TWeakObjectPtr<AAtomDestinyGameStateBase> GetGameState(const AActor* actor)
    {
        return MakeWeakObjectPtr(Cast<AAtomDestinyGameStateBase>(actor->GetWorld()->GetGameState()));
    }
    
} // namespace AtomDestiny
