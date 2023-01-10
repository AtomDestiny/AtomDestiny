#pragma once

#include <Engine/Classes/Components/ActorComponent.h>

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

    virtual bool IsReady() const ABSTRACT_RETURN_METHOD;
    virtual void Animate() ABSTRACT_METHOD;
    virtual void SetDefaultState() ABSTRACT_METHOD;

protected:
    virtual void InitializeComponent() override;

    // Skeletal mesh that contains an animator
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Skeletal mesh with animation"))
    TWeakObjectPtr<USkeletalMeshComponent> m_skeletalMeshComponent;
};
