#include "Misc/AutomationTest.h"

#include <AtomDestiny/Core/Utils.h>
#include <AtomDestiny/Gameplay/UnitStorage.h>

#include <vector>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnitStorageTests, "AtomDestiny.Gameplay.UnitStorage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

static size_t UnitEnumCount()
{
    return static_cast<size_t>(EUnitType::None) + 1;
}

static std::vector<FUnitInfo> CreateUnitInfo()
{
    std::vector<FUnitInfo> result;
    result.reserve(UnitEnumCount());

    for (auto unit = EUnitType::Shooter; unit != EUnitType::None; unit = AtomDestiny::Utils::IncrementEnum(unit))
    {
        result.push_back(FUnitInfo{.prefab = {}, .color = FColor::MakeRandomColor()});
    }
    
    return result;
}

bool FUnitStorageTests::RunTest(const FString& parameters)
{
    // Add
    const std::vector<FUnitInfo> unitInfos = CreateUnitInfo();
    
    return true;
}
