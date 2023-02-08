#include "Misc/AutomationTest.h"

#include <AtomDestiny/Private/Tests/World/TestWorld.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FActorComponentUtilsTests, "AtomDestiny.Core.ActorComponentUtilsTests",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FActorComponentUtilsTests::RunTest(const FString& parameters)
{
    AtomDestiny::FTestWorld world;
    
    return true;
}
