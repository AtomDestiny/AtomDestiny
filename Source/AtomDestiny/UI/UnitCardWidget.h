// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Blueprint/UserWidget.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/Border.h>

#include "UnitCardWidget.generated.h"

/**
 * This Widget helps to place unit on the battleground
 */
UCLASS(Blueprintable)
class ATOMDESTINY_API UUnitCardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//UUnitCardWidget(const FObjectInitializer& ObjectInitializer);
	
	void SetTitle(const FString& title);
	
protected:
	//virtual bool Initialize() override;
	
	UPROPERTY(EditAnywhere)
	FString m_title;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UBorder* Border;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* Button;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* Text;
	
};
