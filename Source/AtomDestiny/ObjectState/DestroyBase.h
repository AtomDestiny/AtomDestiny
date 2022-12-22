#pragma once

#include <Engine/Classes/Components/SceneComponent.h>

#include <AtomDestiny/ObjectState/Destroyable.h>

#include "DestroyBase.generated.h"

///
/// Represents base class for any destroyable game object
///
UCLASS(Abstract)
class ATOMDESTINY_API UDestroyBase : public USceneComponent, public IDestroyable
{
    GENERATED_BODY()

public:

    virtual bool IsDestroyed() const override;
    virtual void Destroy() override;
    
protected:
    virtual void BeginPlay() override;
    
    // Explosion particle prefab
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Explosion blueprint"))
    TWeakObjectPtr<AActor> m_explosionBlueprint;
    
    // Destroyed state
    bool m_destroyed = false;
    
    // Start blueprint preload count
    inline static uint32_t BlueprintExplosionPreloadCount = 5;
};
