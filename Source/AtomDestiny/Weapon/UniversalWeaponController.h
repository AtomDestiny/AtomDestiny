#pragma once

#include <AtomDestiny/Weapon/WeaponBase.h>

#include "UniversalWeaponController.generated.h"

///
/// Represent common controller for base projectiles
/// (laser projectiles should used only by laser controller, 
/// because of local/world matrix transforms)
///
UCLASS(ClassGroup=(AtomDestiny), Blueprintable)
class ATOMDESTINY_API UUniversalWeaponController final : public UWeaponBase
{
    GENERATED_BODY()

public:
    explicit UUniversalWeaponController(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());
    
    // Returns fire rate
    virtual double GetFireRate() const override;
    
    // Returns true if weapon see target
    virtual bool IsSeeTarget() const override;
    
    // Fires from weapon, called by Logic
    virtual void Fire(float deltaTime) override;
    
protected:
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
    
private:

    FAsyncCoroutine MakeShot();
    
    // Time between every shot
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shot delay", AllowPrivateAccess = "true"))
    double m_shotDelay = 1.5;
    
    // Ammunition count before reloading
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shot count", AllowPrivateAccess = "true"))
    int32 m_shotCount = 1.0;

    ///
    /// Particle blueprint that spawns by weapon at firing.
    /// Current controller do not attack target with damage,
    /// projectile blueprints should make a real damage to target.
    /// 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shot particle blueprint", AllowPrivateAccess = "true"))
    TSubclassOf<AActor> m_shotParticleBlueprint;

    ///
    /// Shot points where projectiles will be spawned.
    /// Shots between shot position controls by 'Delay between shots' parameter.
    /// Weapon would not make firing without shooting position.
    /// 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Shooting position", AllowPrivateAccess = "true"))
    TArray<USceneComponent*> m_shootingPositions;

    ///
    /// Weapon scan point, it can be nullptr.
    /// Then controller would use shot positions to scan from.
    ///
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Scan position", AllowPrivateAccess = "true"))
    TWeakObjectPtr<USceneComponent> m_scanPosition;
    
    // Time between shots at shot positions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Delay between shots", AllowPrivateAccess = "true"))
    double m_delayBetweenShots = 0;
};
