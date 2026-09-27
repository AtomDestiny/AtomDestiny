#include "HunterAnimation.h"

#include <Animation/AnimInstance.h>

void UHunterAnimation::BeginPlay()
{
    Super::BeginPlay();
    
    check(m_skeletalMeshComponent.IsValid());
    m_animation = m_skeletalMeshComponent->GetAnimInstance();

    check(m_animation.IsValid());
    m_isWalkingProperty = FindFieldChecked<FBoolProperty>(m_animation->GetClass(), TEXT("IsWalking"));

    check(m_isWalkingProperty);
}

void UHunterAnimation::Idle()
{
    check(m_isWalkingProperty);
    m_isWalkingProperty->SetPropertyValue_InContainer(m_animation.Get(), false);
}

void UHunterAnimation::Walk()
{
    check(m_isWalkingProperty);
    m_isWalkingProperty->SetPropertyValue_InContainer(m_animation.Get(), true);
}

void UHunterAnimation::Attack()
{
    check(m_isWalkingProperty);
    m_isWalkingProperty->SetPropertyValue_InContainer(m_animation.Get(), false);
}
