#include "Pool.h"

#include "ActorPoolMember.h"

#include "AtomDestiny/Core/ActorUtils.h"
#include "AtomDestiny/Core/ActorComponentUtils.h"

using namespace AtomDestiny;

Pool::Pool(TWeakObjectPtr<AActor> object):
    m_gameObjectToSpawn(std::move(object))
{
}

TWeakObjectPtr<AActor> Pool::Spawn(const FVector& position, const FQuat& rotation)
{
    TWeakObjectPtr<AActor> object;

    if (m_inactive.empty())
    {
        const auto world = GEngine->GetCurrentPlayWorld();
        
        FActorSpawnParameters spawnParams;
        spawnParams.Template = m_gameObjectToSpawn.Get();
        spawnParams.Name = FName{ m_gameObjectToSpawn->GetName() + TEXT(" (") + FString::FromInt(m_nextId++) + TEXT(") ") };

        const FRotator rotator { rotation };
        
        const auto newObject = world->SpawnActor<AActor>(m_gameObjectToSpawn->GetClass(), position, rotator, spawnParams);
        TWeakObjectPtr<AActor> newObjectPtr{ newObject };
        
        // Adds a PoolMember component so we know what pool we belong to.
        Utils::AddNewComponentToActor<UActorPoolMember>(newObjectPtr)->pool = shared_from_this();

        object = std::move(newObjectPtr);
    }
    else
    {
        // Grab the last object in the inactive array
        object = m_inactive.top();
        m_inactive.pop();

        if (!object.IsValid())
        {
            return Spawn(position, rotation);
        }
    }

    object->SetActorLocation(position);
    object->SetActorRotation(rotation);

    Utils::SetActorActive(object, true);
    
    return object;
}

void Pool::Despawn(TWeakObjectPtr<AActor> object)
{
    Utils::SetActorActive(object, false);
    object->SetActorLocation(FVector::Zero());
    
    m_inactive.push(std::move(object));
}
