// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "CommanderPawn.generated.h"

/*
Class for player controllable simple pawn looking down to the ground
*/

UCLASS()
class ATOMDESTINY_API ACommanderPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACommanderPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Move(const struct FInputActionValue& actionValue);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* sphere;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* movement;

	UPROPERTY(EditAnywhere)
	float moveScale;
};
