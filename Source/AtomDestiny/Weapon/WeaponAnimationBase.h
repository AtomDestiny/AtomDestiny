#pragma once

#include <Components/ActorComponent.h>
#include <Components/SkeletalMeshComponent.h>

#include <AtomDestiny/Core/Macros.h>
#include <AtomDestiny/Weapon/WeaponAnimation.h>

#include "WeaponAnimationBase.generated.h"

///
/// Represents base weapon animation for any unit.
///
UCLASS(Abstract, Blueprintable)
class UWeaponAnimationBase : public UActorComponent, public IWeaponAnimation
{
    GENERATED_BODY()

public:
    explicit UWeaponAnimationBase(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    bool IsReady() const ABSTRACT_RETURN_METHOD;
    void Animate() ABSTRACT_METHOD;
    void SetDefaultState() ABSTRACT_METHOD;

protected:
    void InitializeComponent() override;

    // Skeletal mesh that contains an animator
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Skeletal mesh with animation"))
    TWeakObjectPtr<USkeletalMeshComponent> m_skeletalMeshComponent;
};
