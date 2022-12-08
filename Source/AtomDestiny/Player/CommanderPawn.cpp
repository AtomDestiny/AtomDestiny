// Fill out your copyright notice in the Description page of Project Settings.

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

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(sphere);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(sphere);

	springArm->TargetArmLength = 100;

	FRotator rotator;
	rotator.Pitch = -90;
	springArm->SetRelativeRotation(rotator);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	
	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	moveScale = 1.0f;
}

// Called when the game starts or when spawned
void ACommanderPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommanderPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACommanderPawn::Move(const FInputActionValue& actionValue)
{
	FVector2D input = actionValue.Get<FInputActionValue::Axis2D>();

	//AddMovementInput(GetActorRotation().RotateVector(input), moveScale);
	AddMovementInput(FVector(input.X, input.Y, 0), moveScale);
}

// Called to bind functionality to input
void ACommanderPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ACommanderController* CmdC = Cast<ACommanderController>(Controller);

	check(EIC && CmdC)

	EIC->BindAction(CmdC->actionMove, ETriggerEvent::Triggered, this, &ACommanderPawn::Move);

	ULocalPlayer* localPlayer = CmdC->GetLocalPlayer();
	check(localPlayer)

	UEnhancedInputLocalPlayerSubsystem* subSys = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subSys)

	subSys->ClearAllMappings();
	subSys->AddMappingContext(CmdC->pawnMappingContext, 0);
}

