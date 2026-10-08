#include "HunterAnimation.h"

#include <Animation/AnimInstance.h>

#include <Components/SkeletalMeshComponent.h>

void UHunterAnimation::InitializeComponent()
{
    Super::InitializeComponent();

    // Unit logic can call Walk() from its BeginPlay before ours, so resolve the animator before any BeginPlay
    check(m_skeletalMeshComponent.IsValid());
    m_animation = m_skeletalMeshComponent->GetAnimInstance();

    check(m_animation.IsValid());
    m_isWalkingProperty = FindFieldChecked<FBoolProperty>(m_animation->GetClass(), TEXT("IsWalking"));
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
