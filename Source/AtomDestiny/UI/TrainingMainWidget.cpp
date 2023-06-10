// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/TrainingMainWidget.h"

#include <Unit/Unit.h>
#include <Gameplay/UnitStorage.h>

#include <vector>

UTrainingMainWidget::UTrainingMainWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	//ChangeMode(false);
	//m_canvas = CreateDefaultSubobject<UCanvasPanel>(TEXT("Canvas"));
	//SetRootComponent(m_canvas);
}

static const std::vector<ESlateVisibility> VisibilityBool = { ESlateVisibility::Hidden, ESlateVisibility::Visible };

void UTrainingMainWidget::SetupUnits(const TArray<EUnitType>& units)
{
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

void UTrainingMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetupUnits(AtomDestiny::UnitStorage::Instance().GetUnits());
}
