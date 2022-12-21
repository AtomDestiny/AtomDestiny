#pragma once

#include <AtomDestiny/Weapon/Weapon.h>
#include <AtomDestiny/Weapon/WeaponAnimation.h>

#include <AtomDestiny/Core/ADObject/ADObject.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

#include <UE5Coro/Public/UE5Coro.h>

#include "WeaponBase.generated.h"

///
/// Represents base abstract weapon, any weapon realizations should use it as base class
///
UCLASS(Abstract)
class ATOMDESTINY_API UWeaponBase : public UADObject, public IWeapon
{
    GENERATED_BODY()

protected:
    
    // Setups weapon
    virtual void InitializeComponent() override;
    
    // Before first frame
    virtual void BeginPlay() override;
    
    // Destroy message
    virtual void EndPlay(const EEndPlayReason::Type type) override;

    // Active to false
    virtual void Activate(bool state) override;

    // Removes actors layer from raycast mask
    void ExcludeSameLayer();

public:
    
    // Returns fire rate, should be realized at concrete weapon
    virtual double GetFireRate() const ABSTRACT_RETURN_METHOD;
    
    // Main shot for weapon, called by Logic
    virtual void Fire(float deltaTime) ABSTRACT_METHOD;

    // Returns true if weapon can see target, called by Logic
    virtual bool IsSeeTarget() const ABSTRACT_RETURN_METHOD;
    
    // Returns weapon firing state
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsFiring() const override;
    
    // Returns aoe flag
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsAoe() const override;
    
    // Returns current damage
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetDamage() const override;
    
    // Returns critical flag
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsCritical() const override;
    
    // Returns stop when attack flag
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsShouldStop() const override;
    
    // Returns use raycast flag
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsRaycasted() const override;
    
    // Returns use friendly fire flag
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsFriendlyFireAvailable() const override;
    
    // Returns reload time
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetReloadTime() const override;
    
    // Returns started reload time
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetBaseReloadTime() const override;
    
    // Sets target to weapon, called by Logic
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetTarget(AActor* target) override;
    
    // Returns current attack range
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetAttackRange() const override;
    
    // Returns current minimal attack range
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetMinAttackRange() const override;
    
    // Returns rotated flag
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsRotated() const override;
    
    // Returns weapon type
    UFUNCTION(Meta = (AllowOverride = true))
    virtual EWeaponType GetWeaponType() const override;
    
    // Returns base weapon
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetBaseDamage() const override;
    
    // Returns base attack range
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetBaseAttackRange() const override;
    
    // Returns critical chance
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetCriticalChance() const override;
    
    // Returns base critical chance
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetBaseCriticalChance() const override;
    
    // Returns critical rate
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetCriticalRate() const override;
    
    // Returns started critical rate
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetBaseCriticalRate() const override;
    
    // Returns current weapon parameters
    UFUNCTION(Meta = (AllowOverride = true))
    virtual FWeaponParameters GetParameters() const override;

protected:
    
    // Rotates weapon on target
    void RotateToTarget(float deltaTime);
    
    // Rotates weapon on root
    void RotateToRoot(float deltaTime) const;
    
    // Throws raycast to target by origin along direction and checks hit transform
    bool CheckRaycastToTarget(const FVector& origin, const FVector& direction, const TWeakObjectPtr<AActor>& target, FHitResult* hitResult = nullptr) const;
    
    // Reload coroutine for reset fire
    FAsyncCoroutine FiringDelay();
    
    // Recalculates weapon parameter
    virtual void RecalculateParameter(EObjectParameters parameter) override;
    
    // Sets parameter to zero value
    virtual void ZeroizeParameter(EObjectParameters parameter) override;

    ///
    /// Properties
    /// 
    
    // Time to reload weapon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reload time"))
    double m_reloadTime = 2.0;
    
    // Max shot azimuth angle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Attack angle"))
    double m_attackAngle = 2.0;
    
    // Weapon rotation speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Rotation speed"))
    double m_rotateSpeed = 1.5;
    
    // Base weapon damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Damage"))
    double m_damage = 2.0;
    
    // Base weapon critical rate, multiply on base damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Critical rate"))
    double m_criticalRate = 2.0;
    
    // Base weapon critical chance in percent
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Critical chance"))
    double m_criticalChance = 15.0;
    
    // Base weapon attack range
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Attack range"))
    double m_attackRange = 5.0;
    
    // Base projectile explosion radius if weapon is aoe
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Explosion radius"))
    double m_explosionRadius = 1.0;
    
    // Weapon type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Weapon type"))
    EWeaponType m_weaponType;
    
    // Weapon rotation ability
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Rotated weapon"))
    bool m_rotatedWeapon = false;
    
    // Weapon gives command to stop if it is firing
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Stop when attack"))
    bool m_stopWhenAttack = true;
    
    // Weapon animator reference
    TScriptInterface<IWeaponAnimation> m_weaponAnimation;
    
    // Use weapon projectiles as a child
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Use projectile as child"))
    bool m_useProjectileAsChild = false;
    
    // Use raycast when check fire ability
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Use raycast"))
    bool m_useRaycast = true;
    
    // Fire ability thought friendly units
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Use friendly fire"))
    bool m_useFriendlyFire = true;
    
    // Projectile blueprint with projectile interface that spawns by weapon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Projectile blueprint"))
    TWeakObjectPtr<AActor> m_projectileBlueprint;

    // Minimal distance to shot
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Minimal distance to shot"))
    double m_minShotDistance = 0;

    // Layer mask
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Layer mask to ignore raycast hits"))
    TEnumAsByte<ECollisionChannel> m_layerMask;
    
    // Main weapon target, driven by ILogic
    TWeakObjectPtr<AActor> m_target = nullptr;
    
    /// Firing state
    bool m_firing = false;
    
    // Rotated on target state
    bool m_isRotatedOnTarget = false;

    ///
    /// It's a pointer to scene component to rotate this weapon and aim from this.
    /// So be aware to use Static mesh or Skeletal mesh, that represents a weapon to rotate in real world.
    /// Hashed weapon transform (Scene component)
    /// 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Weapon component"))
    TWeakObjectPtr<USceneComponent> m_weaponComponent = nullptr;
    
    // Preload spawn blueprints count at pool
    inline static uint32_t BlueprintPreloadCount = 5;
    
    /// Current real damage
    double m_currentDamage = 0;
    
    /// Current real attack range
    double m_currentAttackRange = 0;
    
    // Current weapon explosion radius
    double m_currentExplosionRadius = 0;
    
    // Current weapon critical chance
    double m_currentCriticalChance = 0;
    
    // Current weapon critical rate
    double m_currentCriticalRate = 0;
    
    // Current weapon reload time
    double m_currentReloadTime = 0;
    
    // Current sqrt min shot distance
    double m_minShotSqrtDistance = 0;
};
