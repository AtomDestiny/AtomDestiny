// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Blueprint/UserWidget.h>
#include <Gameplay/UnitStorage.h>
#include <UI/UnitCardWidget.h>

#include <Components/Button.h>
#include <Components/ListView.h>
//#include <Components/ScrollBox.h>

#include "TrainingMainWidget.generated.h"

/**
 * Class of main widget used in Training map
 */
UCLASS(Abstract, Blueprintable)
class ATOMDESTINY_API UTrainingMainWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	TSubclassOf<UUnitCardWidget> unitCardClass;
	
public:
	UTrainingMainWidget(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, meta=(DisplayName = "SetupArmyFlag"))
	bool m_flSetupArmy = false;

	UFUNCTION(BlueprintCallable)
	void SetupUnits(const TArray<EUnitType>& units);

	UFUNCTION(BlueprintCallable)
	void ChangeMode(bool setupArmy);
	
protected:
	virtual bool Initialize() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BnSetupArmy;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BnEndSetupArmy;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UListView* UnitsList;
	
	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* UnitsList;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* Scroll;*/
};
