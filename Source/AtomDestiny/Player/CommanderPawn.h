// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "CommanderPawn.generated.h"

struct FInputActionValue;

///
/// Class for player controllable simple pawn looking down to the ground
///

UCLASS(Blueprintable)
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
    virtual void Tick(float deltaTime) override;

    void OnMoveAction(const FInputActionValue& actionValue);
    void OnRotateAction(const FInputActionValue& actionValue);
    void OnResetAction(const FInputActionValue& actionValue);

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

protected:
    
    UPROPERTY(EditAnywhere, meta = (DisplayName = "Sphere"))
    class USphereComponent* m_sphere;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Spring Arm"))
    class USpringArmComponent* m_springArm;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Camera"))
    class UCameraComponent* m_camera;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Movement"))
    class UFloatingPawnMovement* m_movement;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Move scale"))
    float m_moveScale;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotate scale"))
    float m_rotateScale;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Min height"))
    float m_minHeight;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Max height"))
    float m_maxHeight;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Max horizontal distance"))
    float m_absHorizDist;

private:
    FBox m_worldBox;
    FVector m_startPos;
    FRotator m_startRot;
};
