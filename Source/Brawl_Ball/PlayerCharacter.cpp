// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the First person camera
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Initialize variable(s)
	defaultSpeed = 0.0f;

	sprintModifier = 3.0f;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Find engine default speed
	defaultSpeed = GetCharacterMovement()->MaxWalkSpeed;

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Movement Axes
	PlayerInputComponent->BindAxis("MoveForward/Backward", this, &APlayerCharacter::MoveForwardBackward);
	PlayerInputComponent->BindAxis("MoveRight/Left", this, &APlayerCharacter::MoveRightLeft);

	// Bind Mouse Movement
	PlayerInputComponent->BindAxis("LookUp/Down", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRight/Left", this, &APlayerCharacter::AddControllerYawInput);

	// Bind Jump Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);

	// Bind Sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);

	// Bind Movement ability
	PlayerInputComponent->BindAction("UseMovementAbility", IE_Pressed, this, &APlayerCharacter::UseAbility);

}

/* Moves player in direction of parameter value along the X axis 
 * in relation to its rotation. 1.0 moves player forward, -1.0 
 * moves player backwards*/
void APlayerCharacter::MoveForwardBackward(float value)
{
	// Find and move in input direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

/* Moves player in direction of parameter value along the Y axis
 * in relation to its rotation. 1.0 moves player right, -1.0 
 * moves player left*/
void APlayerCharacter::MoveRightLeft(float value)
{
	// Find and move in input direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

/* Uses the ACharacter class features to perform a jump*/
void APlayerCharacter::StartJump()
{
	bPressedJump = true;
}

/* Uses the ACharacter class features to end a jump*/
void APlayerCharacter::StopJump()
{
	bPressedJump = false;
}

/* Sets the movement speed of the character to a modified sprinting speed*/
void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed * sprintModifier;
}

/* Sets the movement speed of the character back to its defaultspeed after a sprint*/
void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed;
}

/* Calls the Use() method from the character's movement ability*/
void APlayerCharacter::UseAbility()
{
	if (ability != nullptr)
	{
		ability->Use();
	}
}

/* Returns the character's default movement speed*/
const float APlayerCharacter::GetDefaultMovementSpeed()
{
	return defaultSpeed;
}

/* Sets defaultSpeed to parameter newSpeed and resets the walk speed of
 * the character's movement component to the new defaultSpeed*/
void APlayerCharacter::SetDefaultMovementSpeed(float newSpeed)
{
	defaultSpeed = newSpeed;
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed;
}
