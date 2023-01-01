// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;

/**
 * A HealthBar which is auto-oriented in the sreen space
 */
UCLASS(Abstract)
class ATOMDESTINY_API UHealthBar : public UUserWidget
{
    GENERATED_BODY()

public:
	
    UFUNCTION(BlueprintCallable)
    void SetHealthPercent(float value);

	UFUNCTION(BlueprintCallable)
	void SetEnergyPercent(float value);
    
protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* Health;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Energy;
	
};
