#pragma once

#include <AtomDestiny/AtomDestiny.h>

#include <AtomDestiny/Unit/Unit.h>
#include <AtomDestiny/Gameplay/GameDestination.h>

#include <AtomDestiny/Weapon/Projectile.h>
#include <AtomDestiny/ObjectState/ObjectState.h>

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
    FSharedEnemiesList GetEnemies(EGameSide side) const;
    
    // Adds damage from projectile to object with explosion point parameters
    UFUNCTION()
    static void AddDamage(const TScriptInterface<IProjectile>& projectile, EProjectileDamageOptions options = EProjectileDamageOptions::ProjectilePoint);

    // Adds damage to ObjectState directly
    UFUNCTION()
    static void AddDamageToState(const TScriptInterface<IObjectState>& objectState, const FWeaponParameters& parameters);

protected:
    
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
    
    struct GameStateBasePrivateData;
    GameStateBasePrivateData* m_impl = nullptr; // not std::unique_ptr because of virtual destructor
};

namespace AtomDestiny
{
    inline AAtomDestinyGameStateBase* GetGameState(const AActor* actor)
    {
        return CastChecked<AAtomDestinyGameStateBase>(actor->GetWorld()->GetGameState());
    }
    
} // namespace AtomDestiny
