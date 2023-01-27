#pragma once

#include "CoreMinimal.h"

#include "Projectile/ProjectileBase.h"

#include <AtomDestiny/Coroutines/Coroutines.h>

#include "LaserProjectile.generated.h"

class UNiagaraComponent;

///
/// Represents laser shot, that deals damage and spawns impact particle,
/// driven by raycasted weapon controller.
//
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API ULaserProjectile : public UProjectileBase
{
    GENERATED_BODY()

public:
    explicit ULaserProjectile(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());
    
    // Base values initialization to projectile and generating impact prefab
    virtual void Launch() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Niagara component"))
    TObjectPtr<UNiagaraComponent> m_niagaraComponent;
    
private:
    // Generates impact damage and particle
    FAsyncCoroutine GenerateImpact();
    
    // Time after spawn to add damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Add damage delay", AllowPrivateAccess = "true"))
    double m_damageDelay = 0.1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Lifetime (s)", AllowPrivateAccess = "true"))
    double m_lifeTime = 0.6;
};
