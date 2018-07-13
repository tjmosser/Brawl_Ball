// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapComponent.h"
#include "PlayerCharacter.h"

ULeapComponent::ULeapComponent()
{
	// Initialize movement distance variables
	highJumpHeight = 1000.0f;
	longJumpHeight = 500.0f;
	longJumpMaxDistance = 1000.0f;

	// Initialize charge values
	charges = 1;
	maxCharges = 1;

	// Initialize timer durations
	rechargeTime = 5.0f;
	buttonHoldTime = 3.0f;
}

/* Initialize starting values for references */
void ULeapComponent::BeginPlay()
{
	Super::BeginPlay();

	// Setup new action for LongJump call
	GetOwner()->InputComponent->BindAction("UseMovementAbility", IE_Released, this, &ULeapComponent::LongJump);

}

/* Starts timer for HighJump. Called when the player presses the action key. */
void ULeapComponent::Use()
{
	if (charges > 0)
	{
		// TODO: Stop the player from being able to jump normally

		// When the player presses and holds the action key for a specified duration, preform a HighJump
		GetOwner()->GetWorldTimerManager().SetTimer(HoldHandle, this, &ULeapComponent::HighJump, buttonHoldTime, false);
	}	
}

/* Preforms a jump that launches the character upward at a large height. Only is called
after the player holds down the action key for buttonHoldTime seconds.*/
void ULeapComponent::HighJump()
{
	charges--;

	// Launch character upwards at highJumpHeight height
	player->LaunchCharacter(FVector::FVector(0.0f, 0.0f, highJumpHeight), false, false);

	// Regen expended charge
	GetOwner()->GetWorldTimerManager().SetTimer(MovementRegenHandle, this, &ULeapComponent::RegenCharges, rechargeTime, true);
}

/* Launches the character in the direction they're moving with a small height. Called when the
player releases the action key. This will only preform the jump when the player releases the
action key *before* buttonHoldTime seconds have passed */
void ULeapComponent::LongJump()
{
	// HoldHandle is the timer that starts calls HighJump after the action
	// key is held for buttonHoldTime seconds.
	if (GetOwner()->GetWorldTimerManager().IsTimerActive(HoldHandle))
	{
		// Clear the timer so HighJump will not be called
		GetWorld()->GetTimerManager().ClearTimer(HoldHandle);

		// Get which movement buttons are being placed
		float axisValueX = GetOwner()->InputComponent->GetAxisValue("MoveForward/Backward");
		float axisValueY = GetOwner()->InputComponent->GetAxisValue("MoveRight/Left");

		if ((axisValueX != 0.0f || axisValueY != 0.0f) && charges > 0)
		{
			// Get X and Y directions the player is moving by getting corresponding
			// vectors and changing their direction on the axis based on the movement
			// key values
			FVector XVector = GetOwner()->GetActorForwardVector() * axisValueX;
			FVector YVector = GetOwner()->GetActorRightVector() * axisValueY;

			FVector leapDir = XVector + YVector;

			// Change Z value to the specified height
			leapDir.Set(leapDir.X * longJumpMaxDistance, leapDir.Y * longJumpMaxDistance, longJumpHeight);

			charges--;

			// Preform Leap
			player->LaunchCharacter(leapDir, false, false);

			// Regen expended charge
			GetOwner()->GetWorldTimerManager().SetTimer(MovementRegenHandle, this, &ULeapComponent::RegenCharges, rechargeTime, true);

		}

	}
}

/* Regains expended charges until maximum is reached. Called by MovementRegenHandle timer after
   rechargeTime seconds have passed */
void ULeapComponent::RegenCharges()
{
	// Regain expended charges until maximum is reached
	if (charges < maxCharges)
	{
		charges++;
	}

	if (charges == maxCharges)
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementRegenHandle);
	}
}

