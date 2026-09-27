#include "UnitAnimationBase.h"

#include "AtomDestiny/Core/Logger.h"

UUnitAnimationBase::UUnitAnimationBase(const FObjectInitializer& objectInitializer):
    UActorComponent(objectInitializer)
{
    bWantsInitializeComponent = true;
}

void UUnitAnimationBase::InitializeComponent()
{
    Super::InitializeComponent();
    
    if (!m_skeletalMeshComponent.IsValid())
    {
        LOG_ERROR(TEXT("Skeletal animation mesh is not valid at Unit animation base"));
    }
}
