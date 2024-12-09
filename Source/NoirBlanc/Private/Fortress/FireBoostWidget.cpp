// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/FireBoostWidget.h"


float UFireBoostWidget::ChargeVelocity()
{
	float percentage = FMath::Clamp(Velocity/MaxVelocity, 0.0f, 1.0f);
	return percentage;
}
