// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TankMoveComponent.generated.h"


UCLASS( ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent) )
class ATOMDESTINY_API UTankMoveComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	float speed = 1.0f;

private:
	FVector startPos;
	FVector direction;
	FVector offset;
};
