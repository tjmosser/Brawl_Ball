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

	/* Launches the character upwards into the air only after the associated
	timer is finished */
	UFUNCTION()
		void HighJump();

	/* Launches the character upwards and forwards in the direction that they
	are moving in */
	UFUNCTION()
		void LongJump();

	/* Restores any lost charges until maximum allowed amount is reached */
	UFUNCTION()
		void RegenCharges();

	/* Current number of charges*/
	UPROPERTY(EditAnywhere)
		int charges;

	/* Number of maximum charges allowed at one time*/
	UPROPERTY(EditAnywhere)
		int maxCharges;

	/* Time required to regain an expended charge*/
	UPROPERTY(EditAnywhere)
		float rechargeTime;
	
	/* Length of time required for actionkey to be held down to preform HighJump*/
	UPROPERTY(EditAnywhere)
		float buttonHoldTime;

	/* Movement distance values*/
	UPROPERTY(EditAnywhere)
		float highJumpHeight;
	
	UPROPERTY(EditAnywhere)
		float longJumpHeight;

	UPROPERTY(EditAnywhere)
		float longJumpMaxDistance;

	/* Handle for timer that regenerates charges*/
	FTimerHandle MovementRegenHandle;

	/* Timer handle for calling HighJump after holding action key for duratoin of buttonHoldTime*/
	FTimerHandle HoldHandle;

protected:
	/* Called when the game starts or when spawned */
	virtual void BeginPlay() override;
	
};
