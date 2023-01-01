// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBar.h"

#include "Components/ProgressBar.h"

void UHealthBar::SetHealthPercent(float value)
{
    m_health->SetPercent(value);
}

void UHealthBar::SetEnergyPercent(float value)
{
    m_energy->SetPercent(value);
}
