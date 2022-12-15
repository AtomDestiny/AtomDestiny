// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class ATOMDESTINY_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Box"))
    class UBoxComponent* m_box;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Mesh"))
    class UStaticMeshComponent* m_mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealthBar"))
    class UWidgetComponent* m_healthBar;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
