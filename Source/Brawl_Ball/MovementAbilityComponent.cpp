// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementAbilityComponent.h"


// Sets default values for this component's properties
UMovementAbilityComponent::UMovementAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UMovementAbilityComponent::Use()
{

}

// Called when the game starts
void UMovementAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// Reference to the player's movement component
	playerMovement = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	
}


// Called every frame
void UMovementAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

