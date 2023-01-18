#include "TestExploder.h"

#include <AtomDestiny/Core/ActorComponentUtils.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

void UTestExploder::BeginPlay()
{
    Super::BeginPlay();

    for (const UStaticMeshComponent* component : AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(GetOwner()))
    {
        m_localTransforms.Add(component->GetRelativeTransform());
    }
}

void UTestExploder::Explode()
{
    if (IsValid(m_explosionBlueprint))
    {
        AtomDestiny::ObjectPool::Instance().Spawn(m_explosionBlueprint.GetDefaultObject(), GetOwner()->GetActorLocation(), GetOwner()->GetActorQuat());
    }
    
    for (const auto component : AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(GetOwner()))
    {
        const double power = FMath::FRandRange(m_minExplosionPower, m_maxExplosionPower);

        component->SetSimulatePhysics(m_simulatePhysics);
        component->AddRadialImpulse(GetOwner()->GetActorLocation(), m_explosionRadius, static_cast<float>(power),
                                    ERadialImpulseFalloff::RIF_Constant, true);
    }

    if (m_destroyAfterExplosion)
    {
        GetOwner()->SetLifeSpan(m_partsDestroyTime + 0.5f);
    }
}

void UTestExploder::ResetLocation()
{
    const auto rootComponent = GetOwner()->GetRootComponent();
    const TArray<UStaticMeshComponent*> components = AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(GetOwner());

    for (uint64_t index = 0; index < components.Num(); ++index)
    {
        check(index < m_localTransforms.Num());

        UStaticMeshComponent* component = components[index];
        const FTransform& localTransform = m_localTransforms[index];

        component->SetSimulatePhysics(false);
        component->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        component->SetRelativeTransform(localTransform);
    }
}
