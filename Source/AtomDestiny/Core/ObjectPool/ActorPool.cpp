#include "ActorPool.h"
#include "Despawner.h"

#include <AtomDestiny/Core/ActorComponentUtils.h>
#include <AtomDestiny/Core/ObjectPool/Detail/ActorPoolMember.h>

using namespace AtomDestiny;

ActorPool& ActorPool::Instance()
{
    static ActorPool actorPool;
    return actorPool;
}

void ActorPool::Initialize(const TStrongObjectPtr<AActor>& object)
{
    if (!m_pools.count(object))
    {
        auto element = std::make_pair(object, MakeShared<Pool>(object));
        m_pools.insert(std::move(element));
    }
}

TStrongObjectPtr<AActor> ActorPool::Spawn(TStrongObjectPtr<AActor> object, FVector position, FRotator rotation)
{
    if (!object)
        return nullptr;

    Initialize(object);
    return m_pools[std::move(object)]->Spawn(position, rotation);
}

TStrongObjectPtr<AActor> ActorPool::Spawn(TStrongObjectPtr<AActor> object)
{
    return Spawn(std::move(object), FVector::ZeroVector, FRotator::ZeroRotator);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ActorPool::Despawn(TStrongObjectPtr<AActor> object) const
{
    if (const auto poolMember = object->FindComponentByClass<ActorPoolMember>(); poolMember == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("The Actor %s wasn't spawned from a pool. Destroying it instead."), GetData(object->GetName()));
        object->SetLifeSpan(1);
    }
    else
        poolMember->pool->Despawn(object);
}

void ActorPool::Despawn(TStrongObjectPtr<AActor> object, double time) const
{
    auto despawner = object->FindComponentByClass<UDespawner>();

    if (despawner == nullptr)
        despawner = Utils::AddNewComponentToActor<UDespawner>(object);

    despawner->Despawn(time);
}

void ActorPool::DestroyAll()
{
    m_pools.clear();
}

void ActorPool::Destroy(TStrongObjectPtr<AActor> object)
{
    m_pools.erase(std::move(object));
}

bool ActorPool::Contains(TStrongObjectPtr<AActor> object) const
{
    return static_cast<bool>(m_pools.count(std::move(object)));
}

void ActorPool::Preload(const TStrongObjectPtr<AActor>& object, uint32_t size)
{
    if (object.IsValid() && m_preloadingActive)
    {
        if (!m_pools.count(object))
        {
            Initialize(object);

            // Make an array to grab the objects we're about to pre-spawn
            std::vector<TStrongObjectPtr<AActor>> objects;
            objects.reserve(size);

            for (uint32_t i = 0; i < size; ++i)
                objects.push_back(Spawn(object));

            // Now despawn them all
            for (const TStrongObjectPtr<AActor>& poolObject : objects)
                Despawn(poolObject);
        }
    }
}
