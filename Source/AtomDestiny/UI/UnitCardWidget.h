// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Blueprint/UserWidget.h>
#include <Components/Button.h>
#include <Components/Border.h>

#include "UnitCardWidget.generated.h"

/**
 * 
 */
UCLASS()
class ATOMDESTINY_API UUnitCardWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* Border;
};
