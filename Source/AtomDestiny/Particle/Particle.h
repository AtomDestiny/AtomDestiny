#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include "Particle.generated.h"

class USceneComponent;
class UNiagaraComponent;

///
/// Particle base represents any Atom Destiny particle at world.
/// Create Blueprints or any derived classes from this entity.
///
UCLASS(BlueprintType, Blueprintable, hideCategories=(Actor), meta=(ShortTooltip="Default particle base class, use it for any Atom Destiny particle"))
class AParticle : public AActor
{
    GENERATED_BODY()
    
public:
    explicit AParticle(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    // plays particle on activation
    void SetActorHiddenInGame(bool hidden) override;
    void Tick(float deltaTime) override;
    
protected:
    void UpdateNiagaraTransform();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Root scene component"))
    TObjectPtr<USceneComponent> m_sceneComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Niagara component"))
    TObjectPtr<UNiagaraComponent> m_niagaraComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Enable despawn delay on activation"))
    bool m_isDespawnDelayEnabled = true;
    
    // delay to be despawn at seconds, makes no sense if 'Enable despawn delay on activation' is disabled
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Despawn delay"))
    double m_despawnDelay = 1.0f;
};
