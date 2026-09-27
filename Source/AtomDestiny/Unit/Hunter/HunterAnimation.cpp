#include "HunterAnimation.h"

#include <Animation/AnimSingleNodeInstance.h>

void UHunterAnimation::BeginPlay()
{
    Super::BeginPlay();
    
    if (m_skeletalMeshComponent.IsValid())
    {
        m_animation = m_skeletalMeshComponent->GetSingleNodeInstance();
    }
}

void UHunterAnimation::Idle()
{
    if (m_animation.IsValid())
    {
        m_animation->StopAnim();
    }
}

void UHunterAnimation::Walk()
{
    if (m_animation.IsValid())
    {
        m_animation->PlayAnim();
    }
}

void UHunterAnimation::Attack()
{
    if (m_animation.IsValid())
    {
        m_animation->StopAnim();
    }
}
