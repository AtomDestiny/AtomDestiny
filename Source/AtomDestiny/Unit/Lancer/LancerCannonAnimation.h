#pragma once

#include <Components/ActorComponent.h>

#include <AtomDestiny/Weapon/WeaponAnimationBase.h>

#include "LancerCannonAnimation.generated.h"

class UAnimSingleNodeInstance;

///
/// Represents phase cannon animation controller for Lancer
///
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API ULancerCannonAnimation final : public UWeaponAnimationBase
{
    GENERATED_BODY()
    
public:
    virtual void BeginPlay() override;
    
    virtual bool IsReady() const override;
    virtual void Animate() override;
    virtual void SetDefaultState() override;

private:
    TWeakObjectPtr<UAnimSingleNodeInstance> m_animation;
};
