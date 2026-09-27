#pragma once

#include <Components/ActorComponent.h>

#include "Despawner.generated.h"

///
/// Actor despawner by timer.
/// Actor would be des-pawned to Actor's Pool.
/// Timer is cleared when the actor is deactivated (returned to pool) or ends play.
///
UCLASS(Blueprintable)
class ATOMDESTINY_API UDespawner final : public UActorComponent
{
    GENERATED_BODY()

public:
    explicit UDespawner(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    void Despawn(double time);
    void Reset();

    virtual void Deactivate() override;

protected:
    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
    void ClearDespawnTimer();

    FTimerHandle m_timerHandle;
};
