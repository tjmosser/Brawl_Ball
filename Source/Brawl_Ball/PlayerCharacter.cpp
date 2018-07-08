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

	// Setup WallRunDectection capsule collider
	WallRunDetector = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WallRunDector"));
	WallRunDetector->SetupAttachment(RootComponent);
	WallRunDetector->SetCapsuleSize(GetCapsuleComponent()->GetScaledCapsuleRadius() + 7.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 4.0f, true);
	WallRunDetector->bGenerateOverlapEvents = true;
	WallRunDetector->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnRunnableWallOverlapBegin);
	WallRunDetector->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnRunnableWallOverlapEnd);

	// Initialize variable(s)
	defaultSpeed = 0.0f;

	sprintModifier = 3.0f;

	bIsWallRunning = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	JumpMaxCount = 2;

	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	
	FOnTimelineEvent onTimelineCallback;

	// Setup WallRunTimeline with looping and callback
	WallRunTimeline = NewObject<UTimelineComponent>(this, FName("WallRunTimeline"));
	WallRunTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	this->BlueprintCreatedComponents.Add(WallRunTimeline);
	WallRunTimeline->SetPropertySetObject(this);
	WallRunTimeline->SetLooping(true);
	onTimelineCallback.BindUFunction(this, FName{ TEXT("WallRunTimelineCallback") });
	WallRunTimeline->AddEvent(0.0f, onTimelineCallback);
	WallRunTimeline->RegisterComponent();

	// Find engine default speed
	defaultSpeed = GetCharacterMovement()->MaxWalkSpeed;

}

void APlayerCharacter::WallRunTimelineCallback()
{
	FVector playerDir = FPSCameraComponent->GetForwardVector();

	APlayerController* myController = GetWorld()->GetFirstPlayerController();

	if (myController->GetInputKeyTimeDown(EKeys::SpaceBar) > 0.0f && bIsWallRunning)
	{
		GetCharacterMovement()->GravityScale = 0.0f;
		GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));
		GetCharacterMovement()->AddForce(playerDir * 20000.0f);
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1.0f;
		GetCharacterMovement()->SetPlaneConstraintNormal(FVector::ZeroVector);
		bIsWallRunning = false;
	}
}

/**/
void APlayerCharacter::OnRunnableWallOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherComp->ComponentHasTag(FName{ TEXT("RunnableWall") }) && GetCharacterMovement()->IsFalling())
	{
		bIsWallRunning = true;

		JumpCurrentCount = 0;

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("%d"), bFromSweep));

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *SweepResult.ImpactPoint.ToString()));

		if (WallRunTimeline != nullptr)
		{
			WallRunTimeline->PlayFromStart();
		}
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1.0f;
		GetCharacterMovement()->SetPlaneConstraintNormal(FVector::ZeroVector);
		bIsWallRunning = false;
	}

}

/**/
void APlayerCharacter::OnRunnableWallOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex)
{
	if (WallRunTimeline != nullptr)
	{
		WallRunTimeline->Stop();
	}

	if (OtherComp->ComponentHasTag(FName{ TEXT("RunnableWall") }))
	{
		GetCharacterMovement()->GravityScale = 1.0f;
		GetCharacterMovement()->SetPlaneConstraintNormal(FVector::ZeroVector);
		bIsWallRunning = false;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WallRunTimeline != nullptr)
	{
		WallRunTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

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

/* Called upon landing when falling, to perform actions based on the Hit result
 * and reset number of jumps able to be performed. Calls ACharacter::Landed(Hit)*/
void APlayerCharacter::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);

	JumpCurrentCount = 0;
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
	JumpCurrentCount++;

	switch (JumpCurrentCount)
	{
		case 1:
			LaunchCharacter(FVector(0.0f, 0.0f, 420.0f), false, true);
			break;
		case 2:
			LaunchCharacter(FVector(0.0f, 0.0f, 420.0f), false, true);
			break;
		default:
			break;
	}
}

/* Uses the ACharacter class features to end a jump*/
void APlayerCharacter::StopJump()
{
	StopJumping();
}

/* Sets the movement speed of the character to a modified sprinting speed*/
void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed * sprintModifier;
}

/* Sets the movement speed of the character back to its defaultSpeed after a sprint*/
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
