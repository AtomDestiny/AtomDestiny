#pragma once

#include <Engine/Classes/Components/ActorComponent.h>

#include "Despawner.generated.h"

///
/// Actor despawner by timer.
/// Actor would be despawned to Actor's Pool
///
UCLASS(Blueprintable)
class ATOMDESTINY_API UDespawner final : public UActorComponent
{
    GENERATED_BODY()

public:
    void Despawn(double time);

private:
    FTimerHandle m_timerHandle;
};
