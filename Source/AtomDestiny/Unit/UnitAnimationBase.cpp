#include "UnitAnimationBase.h"

#include "AtomDestiny/Core/Logger.h"

#include <GameFramework/Actor.h>
#include <Components/SkeletalMeshComponent.h>

UUnitAnimationBase::UUnitAnimationBase(const FObjectInitializer& objectInitializer):
    UActorComponent(objectInitializer)
{
    bWantsInitializeComponent = true;
}

void UUnitAnimationBase::InitializeComponent()
{
    Super::InitializeComponent();

    m_skeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

    if (!m_skeletalMeshComponent.IsValid())
    {
        LOG_ERROR(TEXT("Skeletal animation mesh is not valid at Unit animation base"));
    }
}
