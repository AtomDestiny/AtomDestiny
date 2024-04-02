#pragma once

#include "ProjectileBase.h"

#include "RocketBase.generated.h"

class UBoxComponent;

///
/// Represents basic rocket.
/// It contains all common parameters to rocket and missile.
///
UCLASS(Abstract)
class ATOMDESTINY_API ARocketBase : public AProjectileBase
{
    GENERATED_BODY()

public:
    explicit ARocketBase(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

protected:
    virtual void BeginPlay() override;
    virtual void OnDisabled() override;
    virtual void NotifyActorBeginOverlap(AActor* other) override;

    FAsyncCoroutine LockOn();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Rocket max speed"))
    double m_speed = 6.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Time after launch to lock target"))
    double m_lockedTime = 0.3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Rocket life time after launch"))
    double m_lifeTime = 10.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Rocket angular speed after lock"))
    double m_angularSpeed = 1.05;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Time to set rocket in ready state after launch"))
    double m_activationTime = 0.25;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reference to rocket engine"))
    TWeakObjectPtr<USceneComponent> m_rocketEngine;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Trace prefab"))
    TSubclassOf<AParticle> m_tracePrefab;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Box collider"))
    TObjectPtr<UBoxComponent> m_boxCollider;
    
    bool m_locked = false;
    bool m_launched = false;
    
    FVector m_rocketForward;
    double m_startLifeTime = 0;
};
