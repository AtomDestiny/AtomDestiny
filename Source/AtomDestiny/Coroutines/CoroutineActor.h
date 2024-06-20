#pragma once

#include "AtomDestiny/Coroutines/CoroutinesRunner.h"

#include <GameFramework/Actor.h>

#include "CoroutineActor.generated.h"

///
/// Represents adapter to CoroutinesRunner as UE Actor.
/// 
UCLASS(ClassGroup=(AtomDestiny), Blueprintable)
class ATOMDESTINY_API ACoroutineActor: public AActor
{
    GENERATED_BODY()
    
public:
    explicit ACoroutineActor(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    [[maybe_unused]] TOptional<uint64_t> RunCoroutine(FAsyncCoroutine&& coroutine);
    [[maybe_unused]] bool StopCoroutine(const uint64_t id);
    void StopAllCoroutines();

protected:
    virtual void SetActorHiddenInGame(const bool isHidden) override;
    virtual void OnEnabled() {}
    virtual void OnDisabled() {}
    
private:
    AtomDestiny::CoroutinesRunner m_runner;
};

inline ACoroutineActor::ACoroutineActor(const FObjectInitializer& objectInitializer):
    AActor(objectInitializer)
{
}

inline TOptional<uint64_t> ACoroutineActor::RunCoroutine(FAsyncCoroutine&& coroutine)
{
    return m_runner.RunCoroutine(std::move(coroutine), this);
}

inline bool ACoroutineActor::StopCoroutine(const uint64_t id)
{
    return m_runner.StopCoroutine(id);
}

inline void ACoroutineActor::StopAllCoroutines()
{
    m_runner.StopAllCoroutines();
}

inline void ACoroutineActor::SetActorHiddenInGame(const bool isHidden)
{
    Super::SetActorHiddenInGame(isHidden);
    
    if (isHidden)
    {
        StopAllCoroutines();
        OnDisabled();
    }
    else
    {
        OnEnabled();
    }
}
