// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MovementAbilityComponent.generated.h"

// Forward Declarations
class APlayerCharacter;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAWL_BALL_API UMovementAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovementAbilityComponent();

	UFUNCTION()
		virtual void Use();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UCharacterMovementComponent *playerMovement;	// reference to player's movement component

	APlayerCharacter *player;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
	
};
