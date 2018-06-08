// Fill out your copyright notice in the Description page of Project Settings.

#include "DashComponent.h"

// Sets default values for this component's properties
UDashComponent::UDashComponent()
{	
	charges = 2;
	maxCharges = 2;
	dashMod = 5000.0f;
	delay = 0.3f;
	rechargeTime = 2.0f;
	canDash = true;
}

// Called when the game starts
void UDashComponent::BeginPlay()
{
	Super::BeginPlay();

	// Reference to the camera being used by player
	playerCamera = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));
}

/**	
*	Called to regenerate charges when the ability has been used. Called by
*	a looping timer that is cleared when no more charges are left to be
*	regenerated.
*/
void UDashComponent::RegenCharges()
{
	if (charges < maxCharges)
	{
		charges = charges + 1;
	}

	if (charges == maxCharges)
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenChargeHandle);
	}
}

// Used to end dash movement smoothly and re-enable dash
void UDashComponent::DashDelay()
{
	playerMovement->StopMovementImmediately();		// Stops player so that the movement is in a straight line instead of an arch
	canDash = true;
}

// Move player and set associated timers
void UDashComponent::Use()
{
	if (charges > 0 && canDash)
	{
			canDash = false;
			
			// Move player in direction camera is facing by a distance modifier
			playerMovement->GetCharacterOwner()->LaunchCharacter(playerCamera->GetForwardVector() * dashMod, false, false);

			// Set delay for dash to be reusable
			GetOwner()->GetWorldTimerManager().SetTimer(DashDelayHandle, this, &UDashComponent::DashDelay, delay, false);
			
			charges--;

			// Start timer for charge regeneration
			GetOwner()->GetWorldTimerManager().SetTimer(RegenChargeHandle, this, &UDashComponent::RegenCharges, rechargeTime, true);
	}
}

