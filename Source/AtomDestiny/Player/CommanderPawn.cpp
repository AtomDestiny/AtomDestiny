#include "CommanderPawn.h"

#include "CommanderController.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ACommanderPawn::ACommanderPawn()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    //PrimaryActorTick.bCanEverTick = true;

    m_sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    SetRootComponent(m_sphere);

    m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    m_springArm->SetupAttachment(m_sphere);

    m_springArm->TargetArmLength = 100;

    FRotator rotator;
    rotator.Pitch = -90;
    m_springArm->SetRelativeRotation(rotator);

    m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    m_camera->SetupAttachment(m_springArm, USpringArmComponent::SocketName);
    
    m_movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

    m_moveScale = 1.0f;
}

// Called when the game starts or when spawned
void ACommanderPawn::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ACommanderPawn::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

void ACommanderPawn::Move(const FInputActionValue& actionValue)
{
    FVector2D input = actionValue.Get<FInputActionValue::Axis2D>();

    //AddMovementInput(GetActorRotation().RotateVector(input), moveScale);
    AddMovementInput(FVector(input.X, input.Y, 0), m_moveScale);
}

// Called to bind functionality to input
void ACommanderPawn::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
    Super::SetupPlayerInputComponent(playerInputComponent);

    UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(playerInputComponent);
    const ACommanderController* commanderController = Cast<ACommanderController>(Controller);

    check(enhancedInputComponent && commanderController)

    enhancedInputComponent->BindAction(commanderController->GetActionMove(), ETriggerEvent::Triggered, this, &ACommanderPawn::Move);

    const ULocalPlayer* localPlayer = commanderController->GetLocalPlayer();
    check(localPlayer)

    UEnhancedInputLocalPlayerSubsystem* subSys = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(subSys)

    subSys->ClearAllMappings();
    subSys->AddMappingContext(commanderController->GetPawnMappingContext(), 0);
}

