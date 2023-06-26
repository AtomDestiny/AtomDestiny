// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitCardWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"

/*UUnitCardWidget::UUnitCardWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Border = CreateWidget<UBorder>(this, UBorder::StaticClass(), "Border");
	Button = CreateWidget<UButton>(Border, UButton::StaticClass(), "Button");
	Text = CreateWidget<UTextBlock>(Button, UTextBlock::StaticClass(), "Text");
}*/

void UUnitCardWidget::SetTitle(const FString& title)
{
//	m_title = title;
	Text->SetText(FText::FromString(title));
}

/*bool UUnitCardWidget::Initialize()
{
	if (!Super::Initialize())
		return false;

	//Text->SetText(FText::FromString(m_title));
	
	return true;
}*/

/*void UUnitCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WidgetTree)
	{
		UCanvasPanel* RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Canvas"));
		// If GetRootWidget() is still null
		WidgetTree->RootWidget = RootWidget;

		Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Border"));

		if (RootWidget)
			RootWidget->AddChild(Border);
	
		Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("Button"));
		Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Text"));
	}
}*/
