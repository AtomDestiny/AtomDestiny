#pragma once

#include <memory>

#include <Templates/SharedPointer.h>
#include <Components/ActorComponent.h>

#include "ActorPoolMember.generated.h"

namespace AtomDestiny
{
    class Pool;
    
} // namespace AtomDestiny

///
/// Implements a member of AActor to control actor's pool>
/// Added to freshly instantiated objects, so we can link back
/// to the correct pool to UnSpawn.
///
UCLASS(Blueprintable)
class ATOMDESTINY_API UActorPoolMember final : public UActorComponent
{
    GENERATED_BODY()
    
public:
    std::shared_ptr<AtomDestiny::Pool> pool;
};
