#include "ScrapConstruction.h"

#include <AtomDestiny/Core/ActorComponentUtils.h>

void UScrapConstruction::BeginPlay()
{
    Super::BeginPlay();
    
    for (const UStaticMeshComponent* component : AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(GetOwner()))
    {
        m_localTransforms.Add(component->GetRelativeTransform());
    }
}

void UScrapConstruction::Deactivate()
{
    Super::Deactivate();

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
