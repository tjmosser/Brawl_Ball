// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "FPSCameraComponent.generated.h"

/**
 * 
 */

// Forward Declarations
class APlayerCharacter;

UCLASS()
class BRAWL_BALL_API UFPSCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UFPSCameraComponent();

	UFUNCTION()
	void AddCameraPitch(float val);

	UFUNCTION()
	void AddCameraYaw(float val);

	UFUNCTION()
	void AddCameraRoll(float val);

	UFUNCTION()
	void SetXAxisSensitivity(float newVal);

	UFUNCTION()
	float GetXAxisSensitivity();

	UFUNCTION()
	void SetYAxisSensitivity(float newVal);

	UFUNCTION()
	float GetYAxisSensitivity();

private:
	APlayerCharacter* player;

	UPROPERTY()
	float sensitivityX;

	UPROPERTY()
	float sensitivityY;

protected:
	virtual void BeginPlay() override;
	
	
};
