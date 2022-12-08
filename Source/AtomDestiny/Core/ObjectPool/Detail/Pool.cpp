#include "Pool.h"

#include "ActorPoolMember.h"

#include "AtomDestiny/Core/ActorUtils.h"
#include "AtomDestiny/Core/ActorComponentUtils.h"

using namespace AtomDestiny;

Pool::Pool(TStrongObjectPtr<AActor> object):
    m_gameObjectToSpawn(std::move(object))
{
}

TStrongObjectPtr<AActor> Pool::Spawn(FVector position, FRotator rotation)
{
    TStrongObjectPtr<AActor> object;

    if (m_inactive.empty())
    {
        const auto world = m_gameObjectToSpawn->GetWorld();
        
        FActorSpawnParameters spawnParams;
        spawnParams.Template = m_gameObjectToSpawn.Get();
        
        const auto newObject = world->SpawnActor<AActor>(m_gameObjectToSpawn->GetClass(), position, rotation, spawnParams);
        TStrongObjectPtr sharedNewObject{ newObject };

        const FString str = m_gameObjectToSpawn->GetName() + TEXT(" (") + FString::FromInt(m_nextId++) + TEXT(") ");
        sharedNewObject->Rename(GetData(str));
        
        // Adds a PoolMember component so we know what pool we belong to.
        Utils::AddNewComponentToActor<ActorPoolMember>(sharedNewObject)->pool = TSharedPtr<Pool>(this);

        object = std::move(sharedNewObject);
    }
    else
    {
        // Grab the last object in the inactive array
        object = m_inactive.top();
        m_inactive.pop();

        if (!object.IsValid())
            return Spawn(position, rotation);
    }

    object->SetActorLocation(position);
    object->SetActorRotation(rotation);

    Utils::SetActorActive(object, true);
    
    return object;
}

void Pool::Despawn(TStrongObjectPtr<AActor> object)
{
    Utils::SetActorActive(object, false);
    m_inactive.push(std::move(object));
}
