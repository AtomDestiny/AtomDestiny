#pragma once

#include "RocketBase.h"

#include "AimRocket.generated.h"

///
/// Represents "Aimed" rocket.
/// Current rocket will lock the target after launch.
/// 
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API AAimRocket : public ARocketBase
{
    GENERATED_BODY()

public:
    explicit AAimRocket(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

protected:
    virtual void Tick(float deltaSeconds) override;
    virtual void Launch() override;
};