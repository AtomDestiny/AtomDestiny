#pragma once

#include <AtomDestiny/Projectile/ProjectileBase.h>
#include <AtomDestiny/Coroutines/Coroutines.h>

#include "InvisibleProjectile.generated.h"

///
/// Represents 'non-render' shot. It can be used to deal damage and spawns impact particle prefab.
///
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API AInvisibleProjectile : public AProjectileBase
{ 
    GENERATED_BODY()

public:
    explicit AInvisibleProjectile(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());
    
    // Base values initialization to projectile and generating impact prefab
    virtual void Launch() override;
    
private:

    // Generates impact damage and particle prefab
    FAsyncCoroutine GenerateImpact();
    
    // Time after spawn to add damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Add damage delay", AllowPrivateAccess = "true"))
    double m_damageDelay = 0.1;
};
