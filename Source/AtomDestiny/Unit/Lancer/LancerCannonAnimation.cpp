#include "LancerCannonAnimation.h"

#include "Animation/AnimSingleNodeInstance.h"

void ULancerCannonAnimation::BeginPlay()
{
    Super::BeginPlay();

    if (m_skeletalMeshComponent.IsValid())
    {
        m_animation = m_skeletalMeshComponent->GetSingleNodeInstance();
    }
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
    if (!m_animation.IsValid())
    {
        return;
    }

    m_animation->StopAnim();
}
