#pragma once

#include <AtomDestiny/Core/Macros.h>
#include <AtomDestiny/Projectile/Projectile.h>
#include <AtomDestiny/Coroutines/CoroutinesRunner.h>

#include <Engine/Classes/GameFramework/Actor.h>

#include "ProjectileBase.generated.h"

class AParticle;

///
/// Represents common abstract projectile.
/// It contains basic parameters and method that needed to weapon controller.
/// Projectile is Actor and should be spawned only by weapon controller
/// that implements IWeapon interface.
///
UCLASS(Abstract)
class AProjectileBase : public AActor, public AtomDestiny::CoroutinesRunner, public IProjectile
{
    GENERATED_BODY()

public:
    explicit AProjectileBase(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    // Sets projectile points from weapon or another projectile
    UFUNCTION()
    virtual void SetPoints(const FProjectilePoints& points) override final;
    
    // Sets weapon parameters to projectile
    UFUNCTION()
    virtual void SetParameters(const FWeaponParameters& parameters) override final;
    
    // Returns projectile parameters
    UFUNCTION()
    virtual const FWeaponParameters& GetParameters() const override final;
    
    // Returns projectile points
    UFUNCTION()
    virtual const FProjectilePoints& GetPoints() const override final;
    
    // Initializes base data and launches a projectile
    virtual void Launch() ABSTRACT_METHOD;

private:
    virtual void BeginPlay() override;
    
protected:
    FWeaponParameters m_parameters;
    FProjectilePoints m_points;
    
    // Impact prefab spawns by projectile when explodes target
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Impact prefab"))
    TSubclassOf<AParticle> m_impactPrefab;
};
