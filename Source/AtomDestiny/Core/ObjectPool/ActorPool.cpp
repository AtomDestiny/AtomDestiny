// ReSharper disable CppMemberFunctionMayBeStatic
#include "ActorPool.h"
#include "Despawner.h"

#include "AtomDestiny/Core/Logger.h"
#include "AtomDestiny/Core/ActorComponentUtils.h"
#include "AtomDestiny/Core/ActorUtils.h"
#include "AtomDestiny/Core/ObjectPool/Details/ActorPoolMember.h"

#include <vector>

using namespace AtomDestiny;

ActorPool& ActorPool::Instance()
{
    static ActorPool actorPool;
    return actorPool;
}

void ActorPool::Initialize(const TWeakObjectPtr<AActor>& object)
{
    if (!m_pools.contains(object))
    {
        auto element = std::make_pair(object, std::make_shared<Pool>(object));
        m_pools.insert(std::move(element));
    }
}

TWeakObjectPtr<AActor> ActorPool::Spawn(TWeakObjectPtr<AActor> object, const FVector& position, const FQuat& rotation)
{
    if (object.Get() == nullptr)
    {
        LOG_WARNING(TEXT("The Actor is invalid, it can not be spawned"));
        return nullptr;
    }

    Initialize(object);
    return m_pools[std::move(object)]->Spawn(position, rotation);
}

TWeakObjectPtr<AActor> ActorPool::Spawn(const TSubclassOf<AActor>& object, const FVector& position, const FQuat& rotation)
{
    if (!IsValid(object))
    {
        LOG_WARNING(TEXT("The subclassof is invalid, can not spawn an Actor"));
        return nullptr;
    }
    
    return Spawn(MakeWeakObjectPtr(object.GetDefaultObject()), position, rotation);
}

TWeakObjectPtr<AActor> ActorPool::Spawn(TWeakObjectPtr<AActor> object)
{
    return Spawn(std::move(object), FVector::ZeroVector, FQuat::Identity);
}

TWeakObjectPtr<AActor> ActorPool::Spawn(const TSubclassOf<AActor>& object)
{
    if (!IsValid(object))
    {
        LOG_WARNING(TEXT("The subclassof is invalid, can not spawn an Actor"));
        return nullptr;
    }

    return Spawn(MakeWeakObjectPtr(object.GetDefaultObject()));
}

void ActorPool::Despawn(TWeakObjectPtr<AActor> object) const
{
    if (const auto poolMember = object->FindComponentByClass<UActorPoolMember>(); poolMember == nullptr)
    {
        LOG_WARNING(TEXT("The Actor %s wasn't spawned from a pool. Destroying it instead."), GetData(object->GetName()));
        
        Utils::SetActorActive(object, false);

        object->SetLifeSpan(0);
        object->Destroy();
    }
    else
    {
        poolMember->pool->Despawn(object);
    }
}

void ActorPool::Despawn(TWeakObjectPtr<AActor> object, double time) const
{
    auto despawner = object->FindComponentByClass<UDespawner>();

    if (despawner == nullptr)
    {
        despawner = Utils::AddNewComponentToActor<UDespawner>(object);
    }

    despawner->Despawn(time);
}

void ActorPool::DestroyAll()
{
    for (const auto& [actor, pool] : m_pools)
    {
        if (actor.IsValid())
        {
            actor->Destroy();
        }

        (void)pool;
    }
    
    m_pools.clear();
}

void ActorPool::Destroy(TWeakObjectPtr<AActor> object)
{
    m_pools.erase(std::move(object));
}

bool ActorPool::Contains(TWeakObjectPtr<AActor> object) const
{
    return m_pools.contains(std::move(object));
}

void ActorPool::Preload(const TSubclassOf<AActor>& object, uint32_t size)
{
    if (IsValid(object))
    {
        const TWeakObjectPtr<AActor> defaultObject = object.GetDefaultObject();
        Preload(defaultObject, size);
    }
}

void ActorPool::Preload(const TWeakObjectPtr<AActor>& object, uint32_t size)
{
    if (object.IsValid() && m_preloadingActive)
    {
        if (!m_pools.contains(object))
        {
            Initialize(object);

            // Make an array to grab the objects we're about to pre-spawn
            std::vector<TWeakObjectPtr<AActor>> objects;
            objects.reserve(size);

            for (uint32_t i = 0; i < size; ++i)
            {
                objects.push_back(Spawn(object));
            }

            // Now despawn them all
            for (const TWeakObjectPtr<AActor>& poolObject : objects)
            {
                Despawn(poolObject);
            }
        }
    }
}
