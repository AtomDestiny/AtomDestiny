// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankMoveComponent.h"
#include "Components/SceneComponent.h"
#include "TankWeaponComponent.generated.h"


UCLASS( ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent) )
class ATOMDESTINY_API UTankWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	UTankMoveComponent* tankMover = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* actor = nullptr;

	UPROPERTY(EditAnyWhere)
	int testField = 1;

};
