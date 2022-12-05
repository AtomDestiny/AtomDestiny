#include "ActorPool.h"
#include "Despawner.h"

#include "AtomDestiny/Core/ActorComponentUtils.h"

using namespace AtomDestiny;

ActorPool& ActorPool::Instance()
{
    static ActorPool actorPool;
    return actorPool;
}

void ActorPool::Initialize(GameObject object)
{
    if (!m_pools.count(object))
    {
        auto element = std::make_pair(object, MakeShared<Pool>(object));
        m_pools.insert(std::move(element));
    }
}

GameObject ActorPool::Spawn(GameObject object, FVector position, FRotator rotation)
{
    if (!object)
    {
        return nullptr;
    }

    Initialize(object);
    return m_pools[std::move(object)]->Spawn(position, rotation);
}

GameObject ActorPool::Spawn(GameObject object)
{
    return Spawn(std::move(object), FVector::ZeroVector, FRotator::ZeroRotator);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ActorPool::Despawn(GameObject object)
{
    if (const auto poolMember = object->FindComponentByClass<ActorPoolMember>(); poolMember == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("The Actor %s wasn't spawned from a pool. Destroying it instead."), *object->GetName());
        object->SetLifeSpan(1);
    }
    else
    {
        poolMember->pool->Despawn(object);
    }
}

void ActorPool::Despawn(GameObject object, double time) const
{
    auto despawner = object->FindComponentByClass<Despawner>();

    if (despawner == nullptr)
    {
        despawner = Utils::AddNewComponentToActor<Despawner>(object);
    }

    despawner->Despawn(time);
}

void ActorPool::DestroyAll()
{
    m_pools.clear();
}

void ActorPool::Destroy(GameObject object)
{
    m_pools.erase(std::move(object));
}

bool ActorPool::Contains(GameObject object) const
{
    return static_cast<bool>(m_pools.count(std::move(object)));
}
