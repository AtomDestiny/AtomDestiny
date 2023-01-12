#pragma once

#include <Runtime/Core/Public/Containers/Map.h>
#include <Engine/Classes/Components/ActorComponent.h>

#include "ScrapConstruction.generated.h"

UCLASS(Blueprintable)
class UScrapConstruction : public UActorComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetActive(bool newActive, bool reset) override;
    
private:
    TMap<TWeakObjectPtr<UStaticMeshComponent>, FTransform> m_localTransforms;
};
