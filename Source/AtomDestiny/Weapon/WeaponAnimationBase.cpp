#include "WeaponAnimationBase.h"

#include <AtomDestiny/Core/Logger.h>

UWeaponAnimationBase::UWeaponAnimationBase(const FObjectInitializer& objectInitializer):
    UActorComponent(objectInitializer)
{
    bWantsInitializeComponent = true;
}

void UWeaponAnimationBase::InitializeComponent()
{
    Super::InitializeComponent();

    if (!m_skeletalMeshComponent.IsValid())
    {
        LOG_ERROR(TEXT("Skeletal animation mesh is not valid at Weapon animation base"));
    }
}
