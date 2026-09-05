#pragma once

#include <Components/ActorComponent.h>

#include "Despawner.generated.h"

///
/// Actor despawner by timer.
/// Actor would be des-pawned to Actor's Pool
///
UCLASS(Blueprintable)
class ATOMDESTINY_API UDespawner final : public UActorComponent
{
    GENERATED_BODY()

public:
    void Despawn(double time);
    void ClearDespawnTimer();

protected:
    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
    FTimerHandle m_timerHandle;
};
