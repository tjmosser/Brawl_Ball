// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MovementAbilityComponent.h"
#include "Camera/CameraComponent.h"
#include "Classes/Components/SceneComponent.h"
#include "Classes/Components/CapsuleComponent.h"
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

	UFUNCTION()
		void MoveForwardBackward(float value);

	UFUNCTION()
		void MoveRightLeft(float value);

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	UFUNCTION()
		void StartSprint();

	UFUNCTION()
		void StopSprint();

	UFUNCTION()
		void UseAbility();

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UMovementAbilityComponent *ability;

	UPROPERTY(EditAnywhere)
	float defaultSpeed;

	UPROPERTY(EditAnywhere)
	float sprintModifier;

};
