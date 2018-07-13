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
#include "FPSCameraComponent.h"
// TESTING
#include "LeapComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class BRAWL_BALL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/* Sets default values for this character's properties*/
	APlayerCharacter();

	/* Called every frame*/
	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Returns the default speed of the character*/
	UFUNCTION()
		const float GetDefaultMovementSpeed();

	/* Sets the movement speed of the character
	 * @param newSpeed - the new movement speed for the character*/
	UFUNCTION()
		void SetDefaultMovementSpeed(float newSpeed);

	/* Controls forward and backward movement*/
	UFUNCTION()
	void MoveForwardBackward(float value);

	/* Controls right and left movement*/
	UFUNCTION()
	void MoveRightLeft(float value);

	/* Increases movement speed while sprint key is held*/
	UFUNCTION()
		void StartSprint();

	/* Restores default movement speed when Sprint key is released*/
	UFUNCTION()
		void StopSprint();

	/*UFUNCTION()
	void LookUpDown(float value);*/

	/*UFUNCTION()
	void TurnRightLeft(float value);*/

	/* Starts a jumping action*/
	UFUNCTION()
	void StartJump();

	/* Ends a jumping action*/
	UFUNCTION()
	void StopJump();

	/* Uses the attached movement ability*/
	UFUNCTION()
	void UseAbility();

	/* Ends a wall run by stopping the WallRunTimeline and removing all movement 
	 * constraints set on the player.*/
	UFUNCTION()
	void EndWallRun();

	/* First person camera that the player will see through by default*/
	UPROPERTY(VisibleAnywhere)
		UFPSCameraComponent* FPSCameraComponent;
		//UCameraComponent* FPSCameraComponent;

protected:
	/* Called when the game starts or when spawned*/
	virtual void BeginPlay() override;

	/* Propels the player forward along the wall. Constrains the player's movement 
	 * and rotation while spacebar is held.*/
	UFUNCTION()
	void WallRunTimelineCallback();

	/*UFUNCTION()
	void WallRunRotationTimelineCallback(float val);*/

	/*UPROPERTY()
	UTimelineComponent* WallRunRotationTimeline;*/

	/* Timeline started when WallRunDetector detects a runnable wall and the player is 
	 * falling. Loops until stopped and has callback WallRunTimelineCallback(). Ends 
	 * when WallRunDetector is no longer colliding with a wall.*/
	UPROPERTY()
	UTimelineComponent* WallRunTimeline;

	/*UPROPERTY()
	UCurveFloat* FloatCurve;*/

private:
	/* OnComponentBeginOverlap delegate for WallRunDetector*/
	UFUNCTION()
	void OnRunnableWallOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* OnComponentEndOverlap delegate for WallRunDetector*/
	UFUNCTION()
	void OnRunnableWallOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*UFUNCTION()
	void OnSideDetectorOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
									class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									const FHitResult& SweepResult);*/

	/*UFUNCTION()
	void OnSideDetectorOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
								  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/

	/* Reference to player's movement ability*/
	UPROPERTY(EditAnywhere)
	UMovementAbilityComponent *ability;

	/* Reference to the collider for detecting wall running*/
	UPROPERTY(EditAnywhere)
	UCapsuleComponent *WallRunDetector;
	
	/*UPROPERTY(EditAnywhere)
	UBoxComponent *RightWallDetector;	// Reference for detecting wall running walls on right side*/

	/*UPROPERTY(EditAnywhere)
	UBoxComponent *LeftWallDetector;	// Reference for detecting wall running walls on left side*/

	/* Multiplier to apply to default speed when generating sprint speed*/
	UPROPERTY(EditAnywhere)
	float sprintModifier;

	/* Speed that the player will be propelled by while wall running.*/
	UPROPERTY(EditAnywhere)
	float wallRunSpeed;

	/* Strength of force that will launch the player along the Z axis when jumping*/
	UPROPERTY(EditAnywhere)
	float jumpForce;

	/* Movement speed that is set at creation*/
	float defaultSpeed;

	/* Time a wall run lasts in seconds*/
	float wallRunDuration;

	bool bIsWallRunning;

	/* Handle for timer that ends the wall run after wallRunDuration seconds*/
	FTimerHandle WallRunHandle;

	/*bool bIsWallRunningRightSide;

	bool bIsWallRunningLeftSide;

	FRotator playerRot;
	FRotator newRot;*/

};
