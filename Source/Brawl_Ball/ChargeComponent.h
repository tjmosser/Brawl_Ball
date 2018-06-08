// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementAbilityComponent.h"
#include "ChargeComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRAWL_BALL_API UChargeComponent : public UMovementAbilityComponent
{
	GENERATED_BODY()
	
public:
	UChargeComponent();

	UFUNCTION()
		void Use();

private:

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
};
