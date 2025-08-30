#include "AtomDestiny.h"

#include "Modules/ModuleManager.h"
#include "Misc/EnumRange.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, AtomDestiny, "AtomDestiny" );

namespace
{
    TArray<FName> GetNamesImpl()
    {
        TArray<FName> names;

        for (const EGameSide side : TEnumRange<EGameSide>())
        {
            names.Emplace(AtomDestiny::GameSide::ToName(side));
        }

        return names;
    }
}

const TArray<FName>& AtomDestiny::GameSide::GetNames()
{
    static const TArray names { GetNamesImpl() };
    return names;
}

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
        return TEXT("Unknown side");
    }
}

FName AtomDestiny::GameSide::ToName(EGameSide side)
{
    return FName{ ToString(side) };
}
