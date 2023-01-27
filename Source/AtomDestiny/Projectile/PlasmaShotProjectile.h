#pragma once

#include <AtomDestiny/Projectile/ProjectileBase.h>
#include <AtomDestiny/Coroutines/Coroutines.h>

#include "PlasmaShotProjectile.generated.h"

///
/// Represents tank shot, that deals damage and spawns impact particle prefab,
/// driven by raycasted weapon controller.
///
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API UPlasmaShotProjectile : public UProjectileBase
{
    GENERATED_BODY()

public:
    explicit UPlasmaShotProjectile(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    // Base values initialization to projectile and generating impact prefab
    virtual void Launch() override;
    
private:

    // Generates impact damage and particle prefab
    FAsyncCoroutine GenerateImpact();
    
    // Time after spawn to add damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Add damage delay", AllowPrivateAccess = "true"))
    double m_damageDelay = 0.1;
};
