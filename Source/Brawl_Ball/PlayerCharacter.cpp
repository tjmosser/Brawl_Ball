// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	// Get FloatCurve
	/*static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Curves/C_WallRunRotation"));
	check(Curve.Succeeded());

	FloatCurve = Curve.Object;*/

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the First person camera
	FPSCameraComponent = CreateDefaultSubobject<UFPSCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCameraComponent->SetupAttachment(RootComponent);
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Setup WallRunDetector capsule collider
	WallRunDetector = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WallRunDetector"));
	WallRunDetector->SetupAttachment(RootComponent);
	WallRunDetector->SetCapsuleSize(GetCapsuleComponent()->GetScaledCapsuleRadius() + 7.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 4.0f, true);
	WallRunDetector->bGenerateOverlapEvents = true;
	WallRunDetector->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnRunnableWallOverlapBegin);
	WallRunDetector->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnRunnableWallOverlapEnd);

	// Setup RightWallDetector capsule collider
	/*RightWallDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWallDetector"));
	RightWallDetector->SetupAttachment(RootComponent);
	RightWallDetector->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.5f));
	RightWallDetector->SetRelativeLocation(FVector(0.0f, 40.0f, 0.0f));
	RightWallDetector->bGenerateOverlapEvents = true;
	RightWallDetector->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSideDetectorOverlapBegin);
	RightWallDetector->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnSideDetectorOverlapEnd);*/

	// Setup LeftWallDetector capsule collider
	/*LeftWallDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWallDetector"));
	LeftWallDetector->SetupAttachment(RootComponent);
	LeftWallDetector->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.5f));
	LeftWallDetector->SetRelativeLocation(FVector(0.0f, -40.0f, 0.0f));
	LeftWallDetector->bGenerateOverlapEvents = true;
	LeftWallDetector->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSideDetectorOverlapBegin);
	LeftWallDetector->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnSideDetectorOverlapEnd);*/

	// Initialize variable(s)
	defaultSpeed = 0.0f;

	sprintModifier = 2.0f;

	wallRunDuration = 5.0f;

	wallRunSpeed = 15000.0f;

	jumpForce = 420.0f;

	bIsWallRunning = false;
	bUseControllerRotationYaw = true;
	//bIsWallRunningRightSide = false;
	//bIsWallRunningLeftSide = false;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Intialize variables
	JumpMaxCount = 2;

	// Enable Crouching
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	// Allow for plane constraints
	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	
	// Setup WallRunTimeline with looping and callback
	FOnTimelineEvent onWallRunTimelineCallback;

	WallRunTimeline = NewObject<UTimelineComponent>(this, FName("WallRunTimeline"));
	WallRunTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	this->BlueprintCreatedComponents.Add(WallRunTimeline);
	WallRunTimeline->SetPropertySetObject(this);
	WallRunTimeline->SetLooping(true);
	onWallRunTimelineCallback.BindUFunction(this, FName{ TEXT("WallRunTimelineCallback") });
	WallRunTimeline->AddEvent(0.0f, onWallRunTimelineCallback);
	WallRunTimeline->RegisterComponent();

	// Setup WallRunRotationTimeline with curve
	/*FOnTimelineFloat onWallRunRotationTimelineCallback;

	WallRunRotationTimeline = NewObject<UTimelineComponent>(this, FName("WallRunRotationTimeline"));
	WallRunRotationTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	this->BlueprintCreatedComponents.Add(WallRunRotationTimeline);
	WallRunRotationTimeline->SetPropertySetObject(this);
	WallRunRotationTimeline->SetLooping(false);
	WallRunRotationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	WallRunRotationTimeline->SetPlaybackPosition(0.0f, false);
	onWallRunRotationTimelineCallback.BindUFunction(this, FName{ TEXT("WallRunRotationTimelineCallback") });
	WallRunRotationTimeline->AddInterpFloat(FloatCurve, onWallRunRotationTimelineCallback);
	WallRunRotationTimeline->RegisterComponent();*/

	// Find engine default speed
	defaultSpeed = GetCharacterMovement()->MaxWalkSpeed;

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Setup Timeline tick
	if (WallRunTimeline != nullptr)
	{
		WallRunTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

	/*if (WallRunRotationTimeline != nullptr)
	{
	WallRunRotationTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}*/

}

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

	// Bind Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::ToggleCrouch);

	// Bind Movement ability
	PlayerInputComponent->BindAction("UseMovementAbility", IE_Pressed, this, &APlayerCharacter::UseAbility);

}

/*void APlayerCharacter::WallRunRotationTimelineCallback(float interpolatedVal)
{
	if (WallRunRotationTimeline->IsReversing())
	{
		newRot.Roll = 0.0f;
	}
	else if (bIsWallRunningLeftSide)
	{
		newRot.Roll = 10.0f;
	}
	else if (bIsWallRunningRightSide)
	{
		newRot.Roll = -10.0f;
	}

	//GetController()->SetControlRotation(FMath::Lerp(playerRot, newRot, interpolatedVal));
	//FPSCameraComponent->SetWorldRotation(FMath::Lerp(playerRot, newRot, interpolatedVal));

}*/

void APlayerCharacter::OnRunnableWallOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName{ TEXT("RunnableWall") }) && GetCharacterMovement()->IsFalling())
	{
		bIsWallRunning = true;

		// This will work when side detection is active.
		//SetActorRotation(FRotationMatrix::MakeFromZY(SweepResult.Normal, GetActorRightVector()).Rotator());

		JumpCurrentCount = 0;

		// Lock actor's rotation while and allow player to look around while wall running
		bUseControllerRotationYaw = false;

		// Start duration timer
		GetWorldTimerManager().SetTimer(WallRunHandle, this, &APlayerCharacter::EndWallRun, wallRunDuration, false);

		if (WallRunTimeline != nullptr)
		{
			WallRunTimeline->PlayFromStart();
		}
	}

}

void APlayerCharacter::OnRunnableWallOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName{ TEXT("RunnableWall") }))
	{
		if (bIsWallRunning)
			EndWallRun();
	}
}

void APlayerCharacter::WallRunTimelineCallback()
{
	FVector playerDir = GetActorForwardVector();

	// TODO: This needs to be tested in a multiplayer setting
	if (Controller != nullptr)
	{

		APlayerController* myController = CastChecked<APlayerController>(Controller);

		if (myController->GetInputKeyTimeDown(EKeys::SpaceBar) > 0.0f && bIsWallRunning)
		{
			// Removed gravity and remove abiltiy to move on Z axis
			GetCharacterMovement()->GravityScale = 0.0f;
			GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));
			
			// propel the player forward
			GetCharacterMovement()->AddForce(playerDir * wallRunSpeed);
		}
		else
		{
			EndWallRun();
		}
	}
}

void APlayerCharacter::EndWallRun()
{
	if (GetWorldTimerManager().IsTimerActive(WallRunHandle))
	{
		GetWorldTimerManager().ClearTimer(WallRunHandle);
	}

	if (WallRunTimeline != nullptr)
	{
		WallRunTimeline->Stop();
	}

	// Reset the player's rotation to where they are looking
	bUseControllerRotationYaw = true;
	
	// Reset gravity and remove constraints
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector::ZeroVector);
	bIsWallRunning = false;
	//bIsWallRunningRightSide = false;
	//bIsWallRunningLeftSide = false;
}

const bool APlayerCharacter::IsWallRunning()
{
	return bIsWallRunning;
}

const float APlayerCharacter::GetDefaultMovementSpeed()
{
	return defaultSpeed;
}

void APlayerCharacter::SetDefaultMovementSpeed(float newSpeed)
{
	defaultSpeed = newSpeed;
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed;
}

/*void APlayerCharacter::OnSideDetectorOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (OtherComp->ComponentHasTag(FName{ TEXT("RunnableWall") }) && GetCharacterMovement()->IsFalling())
	{
		if (OverlappedComp->GetName().Equals(RightWallDetector->GetName()))
		{
			bIsWallRunningRightSide = true;
		}
		else if (OverlappedComp->GetName().Equals(LeftWallDetector->GetName()))
		{
			bIsWallRunningLeftSide = true;
		}

		playerRot = GetActorRotation();
		newRot = playerRot;

		if (WallRunRotationTimeline != nullptr)
		{
			WallRunRotationTimeline->PlayFromStart();
		}

	}

}*/

/*void APlayerCharacter::OnSideDetectorOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex)
{

	if (OtherComp->ComponentHasTag(FName{ TEXT("RunnableWall") }))
	{

		playerRot = GetActorRotation();
		newRot = playerRot;

		if (WallRunRotationTimeline != nullptr)
		{
			WallRunRotationTimeline->ReverseFromEnd();
		}

		bIsWallRunningLeftSide = false;
		bIsWallRunningRightSide = false;

	}

}*/

void APlayerCharacter::MoveForwardBackward(float value)
{
	// Find and move in input direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

void APlayerCharacter::MoveRightLeft(float value)
{
	// Find and move in input direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed * sprintModifier;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed;
}

/*void APlayerCharacter::LookUpDown(float value)
{

}*/

/*void APlayerCharacter::TurnRightLeft(float value)
{

}*/

void APlayerCharacter::StartJump()
{
	JumpCurrentCount++;

	switch (JumpCurrentCount)
	{
		case 1:
			LaunchCharacter(FVector(0.0f, 0.0f, jumpForce), false, true);
			break;
		case 2:
			LaunchCharacter(FVector(0.0f, 0.0f, jumpForce), false, true);
			break;
		default:
			break;
	}
}

void APlayerCharacter::StopJump()
{
	StopJumping();
}

void APlayerCharacter::ToggleCrouch()
{
	if (!bIsWallRunning)
	{
		if (bIsCrouched)
		{
			UnCrouch();
		}
		else
		{
			Crouch();
		}
	}
}

void APlayerCharacter::UseAbility()
{
	if (ability != nullptr)
	{
		ability->Use();
	}
}

