#pragma once

#include <Runtime/AIModule/Classes/DetourCrowdAIController.h>
#include "Navigator.generated.h"

//
// Base entity to use for navigation and AIControl
//
UCLASS(ClassGroup = AI, BlueprintType, Blueprintable)
class ANavigator : public ADetourCrowdAIController
{
    GENERATED_BODY()

public:
    using ADetourCrowdAIController::ADetourCrowdAIController;
};
