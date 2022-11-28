#include "Pool.h"

using namespace AtomDestiny;

Pool::Pool(GameObject object):
    m_gameObjectToSpawn(std::move(object))
{
}

GameObject Pool::Spawn(FVector position, FRotator rotation)
{
    GameObject object = nullptr;

    if (m_inactive.empty())
    {
        const auto world = m_gameObjectToSpawn->GetWorld();
        
        FActorSpawnParameters spawnParams;
        spawnParams.Template = m_gameObjectToSpawn.Get();
        
        const auto newObject = world->SpawnActor<AActor>(m_gameObjectToSpawn->GetClass(), position, rotation, spawnParams);
        GameObject sharedNewObject = MakeShareable(newObject);

        const FString str = m_gameObjectToSpawn->GetName() + TEXT(" (") + FString::FromInt(m_nextId++) + TEXT(") ");
        sharedNewObject->Rename(GetData(str));
        
        // Add a PoolMember component so we know what pool
        // we belong to.
        obj.AddComponent<ObjectPoolMember>().pool = this;
    }
    else
    {
        // Grab the last object in the inactive array
        obj = inactive.Pop();

        if (obj == null)
            return Spawn(pos, rot);
    }

    obj.transform.position = pos;
    obj.transform.rotation = rot;
    obj.SetActive(true);
    
    return obj;
}

void Pool::Despawn(GameObject object)
{
    
}
