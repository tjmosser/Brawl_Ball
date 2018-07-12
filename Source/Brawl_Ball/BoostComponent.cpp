// Fill out your copyright notice in the Description page of Project Settings.

#include "BoostComponent.h"
#include "PlayerCharacter.h"

UBoostComponent::UBoostComponent()
{
	// Initialize all variables
	defaultSpeed = 0.0f;
	boostSpeed = 0.0f;

	speedModifier = 10.0f;
	
	regenTime = 10.0f;
	boostDuration = 5.0f;
	
	bCanUse = true;
}

void UBoostComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize speeds
	defaultSpeed = playerMovement->MaxWalkSpeed;
	boostSpeed = defaultSpeed * speedModifier;

}

void UBoostComponent::Use()
{
	if (bCanUse)
	{
		// Set player's new movement speed
		playerMovement->MaxWalkSpeed = boostSpeed;
		bCanUse = false;

		// Start timer with length boostDuration to end the boost effect
		player->GetWorldTimerManager().SetTimer(BoostDurationHandle, this, &UBoostComponent::CancelBoost, boostDuration, false);
	}
}

void UBoostComponent::CancelBoost()
{
	// Reset player's movement speed
	playerMovement->MaxWalkSpeed = defaultSpeed;

	// Start timer with length regenTime to restore use of ability
	player->GetWorldTimerManager().SetTimer(RegenUsageHandle, this, &UBoostComponent::ResetBoost, regenTime, false);
}

void UBoostComponent::ResetBoost()
{
	bCanUse = true;
}

