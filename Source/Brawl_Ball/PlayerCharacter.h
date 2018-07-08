// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MovementAbilityComponent.h"
#include "Camera/CameraComponent.h"
#include "Classes/Components/SceneComponent.h"
#include "Classes/Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/TimelineComponent.h"
#include "Engine.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class BRAWL_BALL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

	/* Controls forward and backward movement*/
	UFUNCTION()
		void MoveForwardBackward(float value);

	/* Controls right and left movement*/
	UFUNCTION()
		void MoveRightLeft(float value);

	/* Starts a jumping action*/
	UFUNCTION()
		void StartJump();

	/* Ends a jumping action*/
	UFUNCTION()
		void StopJump();

	/* Increases movement speed while sprint key is held*/
	UFUNCTION()
		void StartSprint();

	/* Restores default movement speed when Sprint key is released*/
	UFUNCTION()
		void StopSprint();

	/* Uses the attached movement ability*/
	UFUNCTION()
		void UseAbility();

	/* Returns the default speed of the character*/
	UFUNCTION()
		const float GetDefaultMovementSpeed();

	/* Sets the movement speed of the character
	 * @param newSpeed - the new movement speed for the character*/
	UFUNCTION()
		void SetDefaultMovementSpeed(float newSpeed);

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent;	// First person camera player will see through by default

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void WallRunTimelineCallback();

	UPROPERTY()
	UTimelineComponent* WallRunTimeline;

private:
	/* OnComponentBeginOverlap delegate for WallRunDetector*/
	UFUNCTION()
	void OnRunnableWallOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
									class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
									const FHitResult& SweepResult);

	/* OnComponentEndOverlap delegate for WallRunDetector*/
	UFUNCTION()
	void OnRunnableWallOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
								  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	UMovementAbilityComponent *ability;		// Reference to player's movement ability

	UPROPERTY(EditAnywhere)
	UCapsuleComponent *WallRunDetector;		// Reference to the collider for detecting wall running

	UPROPERTY(EditAnywhere)
	float sprintModifier;					// Multiplier to apply to default speed when generating sprint speed

	float defaultSpeed;						// Movement speed that is set at creation

	bool bIsWallRunning;

};
