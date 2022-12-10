#pragma once

#include <Engine/Classes/Components/SceneComponent.h>

#include <AtomDestiny/Core/Macros.h>
#include <AtomDestiny/ObjectState/Destroyable.h>

#include "DestroyBase.generated.h"

///
/// Represents base class for any destroyable game object
///
UCLASS(Abstract, Blueprintable)
class ATOMDESTINY_API UDestroyBase : public USceneComponent, public IDestroyable
{
    GENERATED_BODY()

public:
    
    // Destroys attached actor
    virtual void Destroy() ABSTRACT_METHOD;
    
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
