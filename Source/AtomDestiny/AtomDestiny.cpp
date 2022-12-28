#include "AtomDestiny.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, AtomDestiny, "AtomDestiny" );

FString AtomDestiny::GameSide::ToString(EGameSide side)
{
    switch (side)
    {
    case EGameSide::Rebels:
        return TEXT("Rebels");
    case EGameSide::Federation:
        return TEXT("Federation");
    case EGameSide::Neutral:
        return TEXT("Neutral");
    default:
        return TEXT("");
    }
}
