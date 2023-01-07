#pragma once

#include <Engine/Classes/Components/ActorComponent.h>

#include <AtomDestiny/Behaviour/Destroyable.h>
#include <AtomDestiny/Particle/Particle.h>

#include "DestroyBase.generated.h"

///
/// Represents base class for any destroyable game object
///
UCLASS(Abstract)
class ATOMDESTINY_API UDestroyBase : public UActorComponent, public IDestroyable
{
    GENERATED_BODY()

public:
    explicit UDestroyBase(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    virtual bool IsDestroyed() const override;
    virtual void Destroy() override;
    
protected:
    virtual void BeginPlay() override;

    void SpawnExplosion(const FVector& location, const FQuat& rotation);
    
    // Explosion particle prefab
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Explosion blueprint"))
    TSubclassOf<AParticle> m_explosionBlueprint;
    
    // Destroyed state
    bool m_destroyed = false;
    
    // Start blueprint preload count
    inline static uint32_t BlueprintExplosionPreloadCount = 5;
};
