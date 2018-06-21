// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementAbilityComponent.h"
#include "EngineMinimal.h"
#include "LeapComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRAWL_BALL_API ULeapComponent : public UMovementAbilityComponent
{
	GENERATED_BODY()
	
public:
	ULeapComponent();

	/* Begins button held down timer for HighJump */
	UFUNCTION()
		void Use();

private:

	/* Restores any lost charges until maximum allowed amount is reached */
	UFUNCTION()
		void RegenCharges();

	/* Launches the character upwards into the air only after the associated
	   timer is finished */
	UFUNCTION()
		void HighJump();

	/* Launches the character upwards and forwards in the direction that they
	   are moving in */
	UFUNCTION()
		void LongJump();

	UPROPERTY(EditAnywhere)
		int charges;					// Current number of charges

	UPROPERTY(EditAnywhere)
		int maxCharges;					// Number of maximum charges allowed at one time

	UPROPERTY(EditAnywhere)
		float rechargeTime;				// Time required to regain a expended charge
	
	UPROPERTY(EditAnywhere)
		float buttonHoldTime;			// Length of time required for action key to be 
										// held down to preform HighJump

	UPROPERTY(EditAnywhere)
		float highJumpHeight;			// Movement distance values
	
	UPROPERTY(EditAnywhere)
		float longJumpHeight;

	UPROPERTY(EditAnywhere)
		float longJumpMaxDistance;

	UCameraComponent *playerCamera;		// Reference to the currently being used camera
	ACharacter *myCharacter;			// Reference to the ACharacter this is attached to

	FTimerHandle MovementRegenHandle;	// Handle for timer that regenerates charges
	FTimerHandle HoldHandle;			// Timer handle for calling HighJump after holding
										// action key for duration of buttonHoldTime

	

protected:
	/* Called when the game starts or when spawned */
	virtual void BeginPlay() override;
	
};
