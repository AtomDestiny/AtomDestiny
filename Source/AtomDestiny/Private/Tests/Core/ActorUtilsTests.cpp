#include "Misc/AutomationTest.h"

#include <AtomDestiny/Private/Tests/World/TestWorld.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FActorUtilsTests, "AtomDestiny.Core.ActorUtils", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FActorUtilsTests::RunTest(const FString& parameters)
{
    return true;
}
