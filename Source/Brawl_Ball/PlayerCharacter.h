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
// Remove after testing
#include "Engine.h"
#include "FPSCameraComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class BRAWL_BALL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	/* Called every frame*/
	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Getter for bIsWallRunning*/
	UFUNCTION()
	const bool IsWallRunning();

	/* Returns the default speed of the character*/
	UFUNCTION()
	const float GetDefaultMovementSpeed();

	/* Sets the movement speed of the character
	 * @param newSpeed - the new movement speed for the character
	 * the character's movement component to the new defaultSpeed
	 * @param newSpeed - the new movement speed for the character*/
	UFUNCTION()
	void SetDefaultMovementSpeed(float newSpeed);

	/* Moves player in direction of parameter value along the X axis
	* in relation to its rotation. 1.0 moves player forward, -1.0
	* moves player backwards*/
	UFUNCTION()
	void MoveForwardBackward(float value);

	/* Moves player in direction of parameter value along the Y axis
	* in relation to its rotation. 1.0 moves player right, -1.0
	* moves player left*/
	UFUNCTION()
	void MoveRightLeft(float value);

	/* Sets the movement speed of the character to a modified sprinting speed*/
	UFUNCTION()
	void StartSprint();

	/* Sets the movement speed of the character back to its defaultSpeed after a sprint*/
	UFUNCTION()
	void StopSprint();

	/*UFUNCTION()
	void LookUpDown(float value);*/

	/*UFUNCTION()
	void TurnRightLeft(float value);*/

	/* Uses the ACharacter class features to perform a jump*/
	UFUNCTION()
	void StartJump();

	/* Uses the ACharacter class features to end a jump*/
	UFUNCTION()
	void StopJump();

	/* Toggles player crouching and ends wall running*/
	UFUNCTION()
	void ToggleCrouch();

	/* Calls the Use() method from the character's movement ability*/
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
	 * and rotation while spacebar is held. Called every iteration of WallRunTimeline*/
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
	/* Bound Delegate for WallRunDetector that is triggered when the player is close 
	 * enough to an object that is a runnable wall and is currently in the air. Sets
	 * variables, starts wall run duration timer, and begins WallRunTimeline*/
	UFUNCTION()
	void OnRunnableWallOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Bound Delegate for WallRunDetector that is triggered when the player leaves 
	 * a runnable wall. Ends timelines and timers.*/
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
