// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * A HealthBar which is auto-oriented in the sreen space
 */
UCLASS(Abstract)
class ATOMDESTINY_API UHealthBar : public UUserWidget
{
    GENERATED_BODY()

public:
   // UHealthBar(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable)
    void SetPercent(float value);
    
protected:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ProgressBar;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
