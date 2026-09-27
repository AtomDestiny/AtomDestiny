#pragma once

#include <AtomDestiny/Weapon/WeaponBase.h>

#include "UniversalWeaponController.generated.h"

class AParticle;

///
/// Represent common controller for base projectiles
/// (laser projectiles should used only by laser controller, 
/// because of local/world matrix transforms)
///
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API UUniversalWeaponController final : public UWeaponBase
{
    GENERATED_BODY()

public:
    explicit UUniversalWeaponController(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());
    
    // Returns fire rate
    double GetFireRate() const override;
    
    // Returns true if weapon see target
    bool IsSeeTarget() const override;
    
    // Fires from weapon, called by Logic
    void Fire(float deltaTime) override;
    
protected:
    
    void BeginPlay() override;
    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
    
private:

    FVoidCoroutine MakeShot();
    
    // Time between every shot
    // These shots would use full ammunition count (Shot count)
    // If ammunition count is equal to 1, then this parameter is ignored
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Ammunition shot delay", AllowPrivateAccess = "true"))
    double m_shotDelay = 1.5;
    
    // Ammunition count before weapon reloading
    // Use 'Ammunition shot delay' parameter to configure delay between all ammunition shots
    // If these parameter is equal to 1, then 'Ammunition shot delay' param does not make any sense
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Ammunition count", AllowPrivateAccess = "true"))
    int32 m_ammunitionCount = 1.0;

    ///
    /// Particle prefab that spawns by weapon at firing.
    /// Current controller do not attack target with damage,
    /// only projectile prefabs should make real damage to target.
    /// 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shot particle prefab", AllowPrivateAccess = "true"))
    TSubclassOf<AParticle> m_shotParticlePrefab;

    ///
    /// Shot points where projectiles will be spawned.
    /// Shots between shot position controls by 'Delay between shots' parameter.
    /// Weapon would not make firing without shooting position.
    /// 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shooting position", AllowPrivateAccess = "true"))
    TArray<TObjectPtr<USceneComponent>> m_shootingPositions;

    ///
    /// Weapon scan point, it can be nullptr.
    /// Then controller would use shot positions to scan from.
    ///
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Scan position", AllowPrivateAccess = "true"))
    TWeakObjectPtr<USceneComponent> m_scanPosition;
    
    // Time between making shots at shot positions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Delay between position shots", AllowPrivateAccess = "true"))
    double m_delayBetweenShots = 0;
};
