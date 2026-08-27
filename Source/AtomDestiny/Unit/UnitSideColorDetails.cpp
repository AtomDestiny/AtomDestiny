#include "UnitSideColorDetails.h"

#include <AtomDestiny/Core/ActorComponentUtils.h>
#include <AtomDestiny/Gameplay/SideStorage.h>
#include <AtomDestiny/Logic/Logic.h>

#include <Components/PrimitiveComponent.h>
#include <Materials/MaterialInstanceDynamic.h>

namespace
{
    bool NameContainsIgnoreCase(const FString& haystack, const FString& needle)
    {
        if (needle.IsEmpty())
        {
            return false;
        }

        return haystack.ToLower().Contains(needle.ToLower());
    }
}

void UUnitSideColorDetails::BeginPlay()
{
    Super::BeginPlay();

    if (const TScriptInterface<ILogic> logic = AtomDestiny::Utils::GetInterface<ILogic>(GetOwner()))
    {
        ApplyForSide(logic->GetSide());
    }
}

void UUnitSideColorDetails::ApplyForSide(const EGameSide side)
{
    if (side == EGameSide::None)
    {
        return;
    }

    TArray<UActorComponent*> detailComponents;
    CollectDetailComponents(detailComponents);
    if (detailComponents.IsEmpty())
    {
        return;
    }

    const FLinearColor teamColor = AtomDestiny::SideStorage::Instance().GetTeamColor(side);

    for (UActorComponent* component : detailComponents)
    {
        ApplyColorToComponent(component, teamColor);
    }
}

void UUnitSideColorDetails::CollectDetailComponents(TArray<UActorComponent*>& outComponents) const
{
    outComponents.Reset();

    TSet<UActorComponent*> uniqueComponents;

    if (AActor* owner = GetOwner())
    {
        for (const FComponentReference& reference : m_detailMeshes)
        {
            if (UActorComponent* component = reference.GetComponent(owner))
            {
                uniqueComponents.Add(component);
            }
        }

        if (m_autoDiscoverByName && !m_autoDiscoverNameContains.IsEmpty())
        {
            for (UActorComponent* component : owner->GetComponents())
            {
                if (component == nullptr || !component->IsA<UPrimitiveComponent>())
                {
                    continue;
                }

                const FString& componentName = component->GetName();
                const FString& readableName = component->GetReadableName();
                if (NameContainsIgnoreCase(componentName, m_autoDiscoverNameContains)
                    || NameContainsIgnoreCase(readableName, m_autoDiscoverNameContains))
                {
                    uniqueComponents.Add(component);
                }
            }
        }
    }

    outComponents.Reserve(uniqueComponents.Num());
    for (UActorComponent* component : uniqueComponents)
    {
        outComponents.Add(component);
    }
}

void UUnitSideColorDetails::ApplyColorToComponent(UActorComponent* component, const FLinearColor& color) const
{
    UPrimitiveComponent* primitive = Cast<UPrimitiveComponent>(component);
    if (primitive == nullptr)
    {
        return;
    }

    const int32 materialCount = primitive->GetNumMaterials();
    for (int32 materialIndex = 0; materialIndex < materialCount; ++materialIndex)
    {
        UMaterialInterface* sourceMaterial = primitive->GetMaterial(materialIndex);
        if (sourceMaterial == nullptr)
        {
            continue;
        }

        if (UMaterialInstanceDynamic* dynamicMaterial = primitive->CreateDynamicMaterialInstance(materialIndex, sourceMaterial))
        {
            dynamicMaterial->SetVectorParameterValue(m_colorParameterName, color);
        }
    }
}
