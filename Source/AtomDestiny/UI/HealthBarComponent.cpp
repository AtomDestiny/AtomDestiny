#include "UI/HealthBarComponent.h"

#include "Core/Logger.h"
#include "UI/HealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

UHealthBarComponent::UHealthBarComponent(const FObjectInitializer& objectInitializer):
    UWidgetComponent(objectInitializer)
{
    const ConstructorHelpers::FClassFinder<UHealthBar> heathBarClassFinder(TEXT("/Game/Blueprint/GUI/Widgets/WBP_HealthBar.WBP_HealthBar_C"));
    const TSubclassOf<UHealthBar> heathBarClass = heathBarClassFinder.Class;

    SetWidgetSpace(EWidgetSpace::Screen);
    SetWidgetClass(heathBarClass);
    SetDrawAtDesiredSize(true);
    SetUsingAbsoluteLocation(true);
}

void UHealthBarComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    const auto world = GetWorld();
    
    if (!world)
    {
        return;
    }

    const auto cameraManager = UGameplayStatics::GetPlayerCameraManager(world, 0);
    
    if (!cameraManager)
    {
        return;
    }
    
    const auto& playerTransform = cameraManager->GetTransform();
    
    if (!playerTransform.IsValid())
    {
        LOG_WARNING(TEXT("Invalid player transform!"));
        return;
    }
    
    const FVector up = playerTransform.TransformVector({0,0,1});
    SetWorldLocation(GetOwner()->GetActorLocation() + up * 420);
}
