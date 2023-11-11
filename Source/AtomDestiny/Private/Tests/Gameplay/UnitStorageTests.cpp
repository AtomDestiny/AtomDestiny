#include "Misc/AutomationTest.h"

#include <AtomDestiny/Core/Utils.h>
#include <AtomDestiny/Core/Logger.h>

#include <AtomDestiny/Gameplay/UnitStorage.h>

#include <unordered_map>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnitStorageTests, "AtomDestiny.Gameplay.UnitStorage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

static constexpr auto WrongValue = EADUnitType::None;

static size_t UnitEnumCount()
{
    return static_cast<size_t>(EADUnitType::None);
}

static std::unordered_map<EADUnitType, FUnitInfo> CreateUnitInfo()
{
    std::unordered_map<EADUnitType, FUnitInfo> result;

    for (auto unit = EADUnitType::Shooter; unit != EADUnitType::None; unit = AtomDestiny::Utils::IncrementEnum(unit))
    {
        result.emplace(unit, FUnitInfo{.prefab = {}, .color = FColor::MakeRandomColor()});
    }
    
    return result;
}

bool FUnitStorageTests::RunTest(const FString& parameters)
{
    const std::unordered_map<EADUnitType, FUnitInfo> unitInfos = CreateUnitInfo();
    
    // Add
    for (const auto& [type, info] : unitInfos)
    {
        AtomDestiny::UnitStorage::Instance().Add(type, info);
    }
    
    TestEqual(TEXT("Unit storage size is equal to Enums count"), UnitEnumCount(), unitInfos.size());

    // Contains
    for ([[maybe_unused]] const auto& [type, info] : unitInfos)
    {
        TestTrue(TEXT("Unit storage contains type"), AtomDestiny::UnitStorage::Instance().Contains(type));
    }

    TestFalse(TEXT("Unit storage does not contain type"), AtomDestiny::UnitStorage::Instance().Contains(WrongValue));

    // Get info
    for ([[maybe_unused]] const auto& [type, info] : unitInfos)
    {
        TestTrue(TEXT("Unit storage contains info"), AtomDestiny::UnitStorage::Instance().GetInfo(type).IsSet());
    }

    TestFalse(TEXT("Unit storage does not contain info"), AtomDestiny::UnitStorage::Instance().GetInfo(WrongValue).IsSet());
    
    return true;
}
