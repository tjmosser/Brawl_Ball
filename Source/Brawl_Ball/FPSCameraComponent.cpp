// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCameraComponent.h"
#include "PlayerCharacter.h"

UFPSCameraComponent::UFPSCameraComponent()
{
	sensitivityX = 1.0f;
	sensitivityY = 1.0f;
}

void UFPSCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	player = CastChecked<APlayerCharacter>(GetAttachmentRootActor());
}

void UFPSCameraComponent::AddCameraPitch(float val)
{
	RelativeRotation.Pitch += val * sensitivityY;
}

void UFPSCameraComponent::AddCameraYaw(float val)
{
	RelativeRotation.Yaw += val * sensitivityX;
}

void UFPSCameraComponent::AddCameraRoll(float val)
{
	RelativeRotation.Roll += val;
}

void UFPSCameraComponent::SetXAxisSensitivity(float newVal)
{
	if (newVal > 0.f)
	{
		sensitivityX = newVal;
	}
}

float UFPSCameraComponent::GetXAxisSensitivity()
{
	return sensitivityX;
}

void UFPSCameraComponent::SetYAxisSensitivity(float newVal)
{
	if (newVal > 0.f)
	{
		sensitivityY = newVal;
	}
}

float UFPSCameraComponent::GetYAxisSensitivity()
{
	return sensitivityY;
}

