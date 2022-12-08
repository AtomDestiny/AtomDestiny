// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CommanderController.generated.h"

/**
 * Class which contains actions & Movement setup for CommanderPawn
 */

UCLASS()
class ATOMDESTINY_API ACommanderController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

	UPROPERTY()
	class UInputMappingContext* pawnMappingContext;

	UPROPERTY()
	class UInputAction* actionMove;
};
