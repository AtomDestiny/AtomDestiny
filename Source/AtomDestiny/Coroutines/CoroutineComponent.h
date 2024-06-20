#pragma once

#include <Runtime/Core/Public/Misc/Optional.h>
#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include <AtomDestiny/Coroutines/CoroutinesRunner.h>

#include "CoroutineComponent.generated.h"

///
/// Represents adapter to CoroutinesRunner as UE Component.
/// CoroutineComponent mainly derived by ADObject.
/// 
UCLASS(ClassGroup=(AtomDestiny), Blueprintable)
class ATOMDESTINY_API UCoroutineComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    explicit UCoroutineComponent(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());
    
    [[maybe_unused]] TOptional<uint64_t> RunCoroutine(FAsyncCoroutine&& coroutine);
    [[maybe_unused]] bool StopCoroutine(const uint64_t id);
    void StopAllCoroutines();

protected:
    virtual void Deactivate() override;
    
private:
    AtomDestiny::CoroutinesRunner m_runner;
};

inline UCoroutineComponent::UCoroutineComponent(const FObjectInitializer& objectInitializer):
    UActorComponent(objectInitializer)
{
}

inline TOptional<uint64_t> UCoroutineComponent::RunCoroutine(FAsyncCoroutine&& coroutine)
{
    return m_runner.RunCoroutine(std::move(coroutine), this);
}

inline bool UCoroutineComponent::StopCoroutine(const uint64_t id)
{
    return m_runner.StopCoroutine(id);
}

inline void UCoroutineComponent::StopAllCoroutines()
{
    m_runner.StopAllCoroutines();
}

inline void UCoroutineComponent::Deactivate()
{
    Super::Deactivate();
    StopAllCoroutines();
}
