#include "CommanderPawn.h"

#include "CommanderController.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Core/Logger.h"


// Sets default values
ACommanderPawn::ACommanderPawn()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    //PrimaryActorTick.bCanEverTick = true;
    m_mouseEnabled = true;

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

    m_moveScale = 0.7f;
    m_rotateScale = 50.f;
    m_minHeight = 500;
    m_maxHeight = 10000;
    m_absHorizDist = 10000;

    m_worldBox = FBox(
        FVector(-m_absHorizDist, -m_absHorizDist, m_minHeight),
        FVector(m_absHorizDist, m_absHorizDist, m_maxHeight)
    );

    m_startPos = FVector(3000, 0, 10000);
    m_startRot = FRotator(-90, 0, 0);
}

// Called when the game starts or when spawned
void ACommanderPawn::BeginPlay()
{
    Super::BeginPlay();

    if (const UWorld* world = GetWorld(); world != nullptr && world->GetMapName().Contains(TEXT("Training")))
    {
        ApplyTopCameraView();
    }
}

void ACommanderPawn::OnMoveAction(const FInputActionValue& actionValue)
{
    FVector&& input = actionValue.Get<FInputActionValue::Axis3D>();
    FVector vec(input);

    /// ***  TODO: Limit pawn moving ***
    vec = GetActorRotation().RotateVector(vec);
    AddMovementInput(vec, m_moveScale);
}

void ACommanderPawn::OnLookAction(const FInputActionValue& actionValue)
{
    FRotator rot = FRotator(
        m_mouseEnabled ? actionValue[0] : 0,
        m_mouseEnabled ? actionValue[1] : 0,
        0);

    rot *= GetWorld()->GetDeltaSeconds() * m_rotateScale;

    AddActorLocalRotation(rot);
}

void ACommanderPawn::OnRollAction(const FInputActionValue& actionValue)
{
    AddActorLocalRotation(FRotator(0,0, actionValue[0]));
}

void ACommanderPawn::ApplyTopCameraView()
{
    SetActorLocation(m_startPos);
    SetActorRotation(m_startRot);
}

void ACommanderPawn::OnResetAction(const FInputActionValue&)
{
    ApplyTopCameraView();
}

void ACommanderPawn::OnLeftClickAction(const FInputActionValue&)
{
    if (ACommanderController* controller = Cast<ACommanderController>(Controller))
    {
        controller->TryPlaceUnitAtCursor();
    }
}

void ACommanderPawn::OnEndSetupArmyAction(const FInputActionValue&)
{
    if (ACommanderController* controller = Cast<ACommanderController>(Controller))
    {
        controller->TryFinishArmySetup();
    }
}

// Called to bind functionality to input
void ACommanderPawn::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
    Super::SetupPlayerInputComponent(playerInputComponent);

    UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(playerInputComponent);
    const ACommanderController* cmdCtrl = Cast<ACommanderController>(Controller);

    check(inputComp && cmdCtrl);

    inputComp->BindAction(cmdCtrl->GetActionMove(), ETriggerEvent::Triggered, this, &ACommanderPawn::OnMoveAction);
    inputComp->BindAction(cmdCtrl->GetActionLook(), ETriggerEvent::Triggered, this, &ACommanderPawn::OnLookAction);
    inputComp->BindAction(cmdCtrl->GetActionRoll(), ETriggerEvent::Triggered, this, &ACommanderPawn::OnRollAction);
    inputComp->BindAction(cmdCtrl->GetActionReset(), ETriggerEvent::Triggered, this, &ACommanderPawn::OnResetAction);
    inputComp->BindAction(cmdCtrl->GetActionLClick(), ETriggerEvent::Started, this, &ACommanderPawn::OnLeftClickAction);
    inputComp->BindAction(cmdCtrl->GetActionEndSetupArmy(), ETriggerEvent::Started, this, &ACommanderPawn::OnEndSetupArmyAction);

    const ULocalPlayer* localPlayer = cmdCtrl->GetLocalPlayer();
    check(localPlayer);

    UEnhancedInputLocalPlayerSubsystem* subSys = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(subSys);

    subSys->ClearAllMappings();
    subSys->AddMappingContext(cmdCtrl->GetPawnMappingContext(), 0);
}
