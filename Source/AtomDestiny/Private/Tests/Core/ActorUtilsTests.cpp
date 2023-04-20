#include "Misc/AutomationTest.h"

#include <AtomDestiny/Core/ActorUtils.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

#include <AtomDestiny/Private/Tests/World/TestWorld.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FActorUtilsTests, "AtomDestiny.Core.ActorUtils",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FActorUtilsTests::RunTest(const FString& parameters)
{
    AtomDestiny::FTestWorld world;

    const TWeakObjectPtr<AActor> actor = world.CreateNewActor(TEXT("TestActor"));
    actor->PrimaryActorTick.bCanEverTick = true;
    
    AtomDestiny::Utils::AddNewComponentToActor<UStaticMeshComponent>(actor);
    AtomDestiny::Utils::AddNewComponentToActor<UStaticMeshComponent>(actor);

    const auto components = actor->GetComponents();

    AtomDestiny::Utils::SetActorActive(actor, false);

    // actor is disabled
    TestTrue(TEXT("Actor is hidden"), actor->IsHidden());
    TestFalse(TEXT("Actor collision is disabled"), actor->GetActorEnableCollision());
    TestFalse(TEXT("Actor tick is disabled"), actor->IsActorTickEnabled());

    for (const auto component : components)
    {
        TestFalse(TEXT("Component is inactive"), component->IsActive());
        TestFalse(TEXT("Component simulate physics option is off"), Cast<UPrimitiveComponent>(component)->IsSimulatingPhysics());
    }

    AtomDestiny::Utils::SetActorActive(actor, true);

    // actor is enabled
    TestFalse(TEXT("Actor is not hidden"), actor->IsHidden());
    TestTrue(TEXT("Actor collision is enabled"), actor->GetActorEnableCollision());
    TestTrue(TEXT("Actor tick is enabled"), actor->IsActorTickEnabled());

    for (const auto component : components)
    {
        TestTrue(TEXT("Component is active"), component->IsActive());
        TestFalse(TEXT("Component simulate physics option is off"), Cast<UPrimitiveComponent>(component)->IsSimulatingPhysics());
        // physics simulation should be off even after set actor active true
    }
    
    return true;
}
