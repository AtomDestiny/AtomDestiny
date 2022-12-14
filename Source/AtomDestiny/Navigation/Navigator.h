#pragma once

#include <Engine/Classes/GameFramework/Actor.h>
#include <Runtime/AIModule/Classes/AIController.h>

#include "Navigator.generated.h"

//
// Base entity to use for navigation and AIControl
// for Atom Destiny unit
//
UCLASS(ClassGroup = AI, BlueprintType, Blueprintable)
class ATOMDESTINY_API ANavigator : public AAIController
{
    GENERATED_BODY()

public:
    explicit ANavigator(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    UFUNCTION()
    void Move(AActor* target);

    UFUNCTION()
    void Stop();

private:
    TWeakObjectPtr<AActor> m_target;
};
