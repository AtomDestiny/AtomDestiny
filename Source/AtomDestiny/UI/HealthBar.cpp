// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBar.h"

#include "Components/ProgressBar.h"

void UHealthBar::SetHealthPercent(float value)
{
	Health->SetPercent(value);
}

void UHealthBar::SetEnergyPercent(float value)
{
	Energy->SetPercent(value);
}
