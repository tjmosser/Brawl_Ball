// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementAbilityComponent.h"
#include "Engine.h"
#include "DashComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRAWL_BALL_API UDashComponent : public UMovementAbilityComponent
{
	GENERATED_BODY()
	
	// UFUNCTIONs
public:

	UFUNCTION()
		void Use();

private:

	UFUNCTION()
		void RegenCharges();

	UFUNCTION()
		void DashDelay();

public:
	UDashComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	int		charges;							// Number of uses of the ability. Regained over time and never exceeds maxCharges
	int		maxCharges;							// Max number of charges to be held at one time
	float	dashMod;						// Modifier for dash distance
	float	delay;							// Duration of the dash. Used to keep the player from arching after launch
	float	rechargeTime;					// Time for a charge to be added back
	bool	canDash;
	UCameraComponent			*playerCamera;
	FTimerHandle				RegenChargeHandle;	// Handle of timer that controls charge regeneration
	FTimerHandle				DashDelayHandle;
	
};
