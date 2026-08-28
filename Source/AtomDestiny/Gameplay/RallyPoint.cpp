#include "RallyPoint.h"

#include <Components/SceneComponent.h>
#include <EngineUtils.h>

ARallyPoint::ARallyPoint(const FObjectInitializer& objectInitializer):
    AActor(objectInitializer)
{
    m_root = objectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
    SetRootComponent(m_root);
}

ARallyPoint* ARallyPoint::FindForSide(const UObject* worldContextObject, const EGameSide side)
{
    if (worldContextObject == nullptr || side == EGameSide::None)
    {
        return nullptr;
    }

    const UWorld* world = worldContextObject->GetWorld();
    if (world == nullptr)
    {
        return nullptr;
    }

    for (TActorIterator<ARallyPoint> iterator(world); iterator; ++iterator)
    {
        ARallyPoint* rallyPoint = *iterator;
        if (rallyPoint != nullptr && rallyPoint->GetSide() == side)
        {
            return rallyPoint;
        }
    }

    return nullptr;
}
