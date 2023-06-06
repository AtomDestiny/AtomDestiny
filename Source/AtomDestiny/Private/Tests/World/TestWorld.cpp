#include "TestWorld.h"

AtomDestiny::FTestWorld::FTestWorld():
    m_world(UWorld::CreateWorld(EWorldType::Game, false))
{
    check(IsInGameThread());

    auto& worldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
    worldContext.SetCurrentWorld(m_world.Get());

    m_world->InitializeActorsForPlay(FURL());
    
    AWorldSettings* settings = m_world->GetWorldSettings();
    settings->MinUndilatedFrameTime = 0.0001;
    settings->MaxUndilatedFrameTime = 10;
    
    m_world->BeginPlay();
}

AtomDestiny::FTestWorld::~FTestWorld()
{
    if (m_world.IsValid())
    {
        GEngine->DestroyWorldContext(m_world.Get());
        m_world->DestroyWorld(true);
    }

    CollectGarbage(RF_NoFlags);
}

TWeakObjectPtr<AActor> AtomDestiny::FTestWorld::CreateNewActor(const FString& name)
{
    FActorSpawnParameters spawnParams;
    spawnParams.Name = FName{name};
    
    AActor* actor = m_world->SpawnActor<AActor>(spawnParams);
    return MakeWeakObjectPtr(actor);
}

void AtomDestiny::FTestWorld::Tick(float deltaSeconds)
{
    check(IsInGameThread());
    
    StaticTick(deltaSeconds);
    m_world->Tick(LEVELTICK_All, deltaSeconds);
    
    ++m_frameCount;
}
