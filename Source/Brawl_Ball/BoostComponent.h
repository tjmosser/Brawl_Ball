// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementAbilityComponent.h"
#include "BoostComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRAWL_BALL_API UBoostComponent : public UMovementAbilityComponent
{
	GENERATED_BODY()
	
public:
	UBoostComponent();

	/* Activates the ability based on value of bCanUse*/
	UFUNCTION()
	void Use();
	
private:
	/* Stops the movement speed increase*/
	UFUNCTION()
	void CancelBoost();

	/* Resets bCanUse so ability can be activated*/
	UFUNCTION()
	void ResetBoost();

	float defaultSpeed;					// Base movement speed of character before modifications
	float boostSpeed;					// New movement speed for character when boost is active

	float speedModifier;				// Modifier to multiply speed by to create boostSpeed
	float regenTime;					// Time required to regain ability usage
	float boostDuration;				// Length of time the speed boost is active
	
	bool bCanUse;

	FTimerHandle BoostDurationHandle;	// Handle for timer that runs for length of boost
	FTimerHandle RegenUsageHandle;		// Handle for timer that runs to regenerate boost usage

protected:
	/* Called when the game starts or when spawned*/
	virtual void BeginPlay() override;
	
};
