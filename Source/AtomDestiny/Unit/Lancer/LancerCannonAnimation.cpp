#include "LancerCannonAnimation.h"

#include "Animation/AnimSingleNodeInstance.h"

void ULancerCannonAnimation::BeginPlay()
{
    Super::BeginPlay();

    if (m_skeletalMeshComponent.IsValid())
    {
        m_animation = m_skeletalMeshComponent->GetSingleNodeInstance();
    }

    // Skeletal mesh may auto-play its default clip on spawn; keep cannon idle until a real shot.
    SetDefaultState();
}

bool ULancerCannonAnimation::IsReady() const
{
    if (m_animation.IsValid())
    {
        return !m_animation->IsPlaying();
    }

    return false;
}

void ULancerCannonAnimation::Animate()
{
    if (IsReady())
    {
        m_animation->PlayAnim();
    }
}

void ULancerCannonAnimation::SetDefaultState()
{
    if (m_animation.IsValid())
    {
        m_animation->StopAnim();
    }
}
