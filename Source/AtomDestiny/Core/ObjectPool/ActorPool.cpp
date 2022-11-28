#include "ActorPool.h"

using namespace AtomDestiny;

ActorPool& ActorPool::Instance()
{
    static ActorPool actorPool;
    return actorPool;
}
