#pragma once

#include <AtomDestiny/Projectile/ProjectileBase.h>
#include <AtomDestiny/Coroutines/Coroutines.h>

#include "InvisibleProjectile.generated.h"

///
/// Represents 'non-render' shot. It can be used to deal damage and spawns impact particle prefab.
///
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class AInvisibleProjectile : public AProjectileBase
{ 
    GENERATED_BODY()
};
