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
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Root scene component"))
    TObjectPtr<USceneComponent> m_sceneComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Niagara component"))
    TObjectPtr<UNiagaraComponent> m_niagaraComponent;
};
