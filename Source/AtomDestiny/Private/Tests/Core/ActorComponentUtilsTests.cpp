#include "Misc/AutomationTest.h"

#include <AtomDestiny/Core/ActorComponentUtils.h>
#include <AtomDestiny/Private/Tests/World/TestWorld.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FActorComponentUtilsTests, "AtomDestiny.Core.ActorComponentUtilsTests",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

// tests only correct addition component to actor
static void AddNewComponentToActorTest(FAutomationTestBase& test)
{
    AtomDestiny::FTestWorld world;
    
    const TWeakObjectPtr<AActor> actor = world.CreateNewActor(TEXT("TestActor"));
    const auto component = AtomDestiny::Utils::AddNewComponentToActor<USceneComponent>(actor);

    test.TestTrue(TEXT("Added component should not be nullptr"), component != nullptr);
}

bool FActorComponentUtilsTests::RunTest(const FString& parameters)
{
    AddNewComponentToActorTest(*this);
    return true;
}
