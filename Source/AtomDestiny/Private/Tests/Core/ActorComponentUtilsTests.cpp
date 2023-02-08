#include "Misc/AutomationTest.h"

#include <AtomDestiny/Core/ActorComponentUtils.h>

#include <AtomDestiny/Unit/UnitParameters.h>
#include <AtomDestiny/Unit/UnitScrapDestroy.h>

#include <AtomDestiny/Private/Tests/World/TestWorld.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FActorComponentUtilsTests, "AtomDestiny.Core.ActorComponentUtilsTests",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

// tests only correct addition component to actor
static void AddNewComponentToActorTest(AtomDestiny::FTestWorld& world, FAutomationTestBase& test)
{
    const TWeakObjectPtr<AActor> actor = world.CreateNewActor(TEXT("TestActor1"));
    const auto component = AtomDestiny::Utils::AddNewComponentToActor<USceneComponent>(actor);

    test.TestTrue(TEXT("Added component should not be nullptr"), component != nullptr);
}

static void FindComponentByNameTest(AtomDestiny::FTestWorld& world, FAutomationTestBase& test)
{
    const TWeakObjectPtr<AActor> actor = world.CreateNewActor(TEXT("TestActor2"));
    const auto component = AtomDestiny::Utils::AddNewComponentToActor<USceneComponent>(actor);

    FString name;
    component->AppendName(name);

    test.TestFalse(TEXT("Not empty name"), name.IsEmpty());

    const std::string stdName = std::string(TCHAR_TO_UTF8(*name));
    const auto expectedComponent = AtomDestiny::Utils::FindComponentByName(actor.Get(), stdName.c_str());

    test.TestTrue(TEXT("Searched component is not nullptr"), expectedComponent != nullptr);
}

static void GetActorInterfacesTests(AtomDestiny::FTestWorld& world, FAutomationTestBase& test)
{
    const TWeakObjectPtr<AActor> actor = world.CreateNewActor(TEXT("TestActor3"));
    
    AtomDestiny::Utils::AddNewComponentToActor<UUnitScrapDestroy>(actor);
    AtomDestiny::Utils::AddNewComponentToActor<UUnitScrapDestroy>(actor);

    const auto destroyInterface = GET_ACTOR_INTERFACE(Destroyable, actor.Get());

    test.TestTrue(TEXT("Destroyable interface is valid"), destroyInterface != nullptr);

    const auto interfaces = GET_ACTOR_INTERFACES(Destroyable, actor.Get());

    test.TestEqual(TEXT("Get interfaces size is two"), interfaces.Num(), 2);

    for (const auto& interface : interfaces)
    {
        test.TestTrue(TEXT("Destroyable interface is valid"), interface != nullptr);
    }
}

bool FActorComponentUtilsTests::RunTest(const FString& parameters)
{
    AtomDestiny::FTestWorld world;
    
    AddNewComponentToActorTest(world, *this);
    FindComponentByNameTest(world, *this);
    GetActorInterfacesTests(world, *this);
    
    return true;
}
