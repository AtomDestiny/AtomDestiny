// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/TrainingMainWidget.h"

#include <Unit/Unit.h>
#include <Gameplay/UnitStorage.h>
#include <UObject/ConstructorHelpers.h>

#include <vector>

UTrainingMainWidget::UTrainingMainWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUnitCardWidget> unitCardClassFinder(TEXT("/Game/Blueprint/GUI/Widgets/WBP_UnitCard.WBP_UnitCard_C"));
	//const TSubclassOf<UUnitCardWidget> unitCardClass = unitCardClassFinder.Class;
	unitCardClass = unitCardClassFinder.Class;
}

static const std::vector<ESlateVisibility> VisibilityBool = { ESlateVisibility::Hidden, ESlateVisibility::Visible };

void UTrainingMainWidget::SetupUnits(const TArray<EUnitType>& units)
{
	auto wdg = CreateWidget<UUnitCardWidget>(this, unitCardClass);
	wdg->SetTitle("Unit 1");
	UnitsList->AddItem(wdg);

	auto wdg2 = CreateWidget<UUnitCardWidget>(this, unitCardClass);
	wdg2->SetTitle("Unit 2");
	UnitsList->AddItem(wdg2);

	auto wdg3 = CreateWidget<UUnitCardWidget>(this, unitCardClass);
	wdg3->SetTitle("Unit 3");
	UnitsList->AddItem(wdg3);
	
	//UnitsList->AddItem(CreateWidget<UUnitCardWidget>(this));
	//UnitsList->AddItem(CreateWidget<UUnitCardWidget>(this));
	
	for (const auto& u : units)
	{
		//CreateWidget<UButton>(Scroll, );
		/*UButton *bn = CreateDefaultSubobject<UButton>(UEnum::GetValueAsName(u));
		Scroll->AddChild(bn);*/
	}
}

void UTrainingMainWidget::ChangeMode(bool setupArmy)
{
	BnSetupArmy->SetVisibility(VisibilityBool[!setupArmy]);
	BnEndSetupArmy->SetVisibility(VisibilityBool[setupArmy]);
	UnitsList->SetVisibility(VisibilityBool[setupArmy]);
}

bool UTrainingMainWidget::Initialize()
{
	if (!Super::Initialize())
		return false;

	SetupUnits(AtomDestiny::UnitStorage::Instance().GetUnits());
	
	return true;
}
