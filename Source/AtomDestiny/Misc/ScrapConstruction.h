#pragma once

#include <Runtime/Core/Public/Containers/Map.h>
#include <Engine/Classes/Components/ActorComponent.h>

#include "ScrapConstruction.generated.h"

///
/// Represents starter scrap component to save local
/// static meshes components transforms.
/// It constructs every static mesh after explosion on every spawn.
///
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class UScrapConstruction : public UActorComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void Deactivate() override;
    
private:
    TMap<TStrongObjectPtr<UStaticMeshComponent>, FTransform> m_localTransforms;
};
