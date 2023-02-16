#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <AtomDestiny/Particle/Particle.h>

#include "TestExploder.generated.h"

UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API UTestExploder : public UActorComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    
    UFUNCTION(CallInEditor)
    void Explode();

    UFUNCTION(CallInEditor)
    void ResetLocation();

private:
    
    // Rigidbody min explosion power. Random value between min and max values
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Min explosion power", AllowPrivateAccess = "true"))
    double m_minExplosionPower = 100.0;
    
    // Rigidbody max explosion power. Random value between min and max values
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max explosion power", AllowPrivateAccess = "true"))
    double m_maxExplosionPower = 500.0;
    
    // Rigidbody explosion radius
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Explosion radius", AllowPrivateAccess = "true"))
    double m_explosionRadius = 25.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Simulate physics", AllowPrivateAccess = "true"))
    bool m_simulatePhysics = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Destroy after explosioon", AllowPrivateAccess = "true"))
    bool m_destroyAfterExplosion = false;

    // Destroy time for parts after explosion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( DisplayName = "Explode parts destroy time", editcondition = "m_destroyAfterExplosion", AllowPrivateAccess = "true"))
    double m_partsDestroyTime = 1.5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Explosion prefab", AllowPrivateAccess = "true"))
    TSubclassOf<AParticle> m_explosionPrefab;
    
    TArray<FTransform> m_localTransforms;
};
