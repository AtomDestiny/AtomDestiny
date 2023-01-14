#pragma once

#include <Runtime/Core/Public/Containers/Map.h>
#include <Engine/Classes/Components/ActorComponent.h>

#include "ScrapConstruction.generated.h"

///
/// Represents starter scrap component to save local
/// static meshes components transforms.
/// It constructs every static mesh after explosion on every spawn.
///
UCLASS(Blueprintable)
class UScrapConstruction : public UActorComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void Activate(bool reset) override;
    
private:
    TMap<TWeakObjectPtr<UStaticMeshComponent>, FTransform> m_localTransforms;
};
