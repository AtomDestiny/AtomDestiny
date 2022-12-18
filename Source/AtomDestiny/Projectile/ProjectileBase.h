#pragma once

#include <AtomDestiny/Core/Macros.h>
#include <AtomDestiny/Projectile/Projectile.h>

#include <Engine/Classes/Components/SceneComponent.h>

#include "ProjectileBase.generated.h"

///
/// Represents common abstract projectile
///
UCLASS()
class UProjectileBase : public USceneComponent, public IProjectile
{
    GENERATED_BODY()

public:

    // Sets projectile points from weapon or another projectile
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetPoints(const FProjectilePoints& points) override;
    
    // Sets weapon parameters to projectile
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetParameters(const FWeaponParameters& parameters) override;
    
    // Returns projectile parameters
    UFUNCTION(Meta = (AllowOverride = true))
    virtual const FWeaponParameters& GetParameters() const override;
    
    // Returns projectile points
    UFUNCTION(Meta = (AllowOverride = true))
    virtual const FProjectilePoints& GetPoints() const override;
    
    // Initializes base data and launches a projectile
    virtual void Launch() ABSTRACT_METHOD;

private:

    virtual void InitializeComponent() override;
    
protected:

    FWeaponParameters m_parameters;
    FProjectilePoints m_points;
    
    // Impact prefab spawns by projectile when explodes target
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Impact blueprint"))
    TWeakObjectPtr<AActor> m_impactBlueprint;
    
    // Preload impact blueprint count
    inline static int ImpactPreloadCount = 5;
};
