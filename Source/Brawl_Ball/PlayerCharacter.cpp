// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

// Sets default values
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

	sprintModifier = 3.0f;

	bIsWallRunning = false;
	bUseControllerRotationYaw = true;
	//bIsWallRunningRightSide = false;
	//bIsWallRunningLeftSide = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	JumpMaxCount = 2;

	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	
	FOnTimelineEvent onWallRunTimelineCallback;
	//FOnTimelineFloat onWallRunRotationTimelineCallback;

	// Setup WallRunTimeline with looping and callback
	WallRunTimeline = NewObject<UTimelineComponent>(this, FName("WallRunTimeline"));
	WallRunTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	this->BlueprintCreatedComponents.Add(WallRunTimeline);
	WallRunTimeline->SetPropertySetObject(this);
	WallRunTimeline->SetLooping(true);
	onWallRunTimelineCallback.BindUFunction(this, FName{ TEXT("WallRunTimelineCallback") });
	WallRunTimeline->AddEvent(0.0f, onWallRunTimelineCallback);
	WallRunTimeline->RegisterComponent();

	// Setup WallRunRotationTimeline with curve
	/*WallRunRotationTimeline = NewObject<UTimelineComponent>(this, FName("WallRunRotationTimeline"));
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

/**/
void APlayerCharacter::WallRunTimelineCallback()
{
	// TODO: Fix to where the player can freely look around without detacting themselves from the wall

	//FVector playerDir = FPSCameraComponent->GetForwardVector();
	FVector playerDir = GetActorForwardVector();

	// TODO: This probably won't work in a multiplayer setting. Especially not local multiplayer
	APlayerController* myController = GetWorld()->GetFirstPlayerController();

	if (myController->GetInputKeyTimeDown(EKeys::SpaceBar) > 0.0f && bIsWallRunning)
	{
		GetCharacterMovement()->GravityScale = 0.0f;
		GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));
		GetCharacterMovement()->AddForce(playerDir * 20000.0f);
	}
	else
	{
		if (GetWorldTimerManager().IsTimerActive(WallRunHandle))
		{
			GetWorldTimerManager().ClearTimer(WallRunHandle);
		}

		EndWallRun();

		/*GetCharacterMovement()->GravityScale = 1.0f;
		GetCharacterMovement()->SetPlaneConstraintNormal(FVector::ZeroVector);
		bIsWallRunning = false;*/
		//bIsWallRunningRightSide = false;
		//bIsWallRunningLeftSide = false;
	}
}

/**/
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

/**/
void APlayerCharacter::OnRunnableWallOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName{ TEXT("RunnableWall") }) && GetCharacterMovement()->IsFalling())
	{
		bIsWallRunning = true;

		JumpCurrentCount = 0;

		bUseControllerRotationYaw = false;

		GetWorldTimerManager().SetTimer(WallRunHandle, this, &APlayerCharacter::EndWallRun, 5.0f, false);

		if (WallRunTimeline != nullptr)
		{
			WallRunTimeline->PlayFromStart();
		}
	}

}

/**/
void APlayerCharacter::OnRunnableWallOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName{ TEXT("RunnableWall") }))
	{
		if (GetWorldTimerManager().IsTimerActive(WallRunHandle))
		{
			GetWorldTimerManager().ClearTimer(WallRunHandle);
		}
		if (bIsWallRunning)
			EndWallRun();
	}
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

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WallRunTimeline != nullptr)
	{
		WallRunTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

	/*if (WallRunRotationTimeline != nullptr)
	{
		WallRunRotationTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}*/

	if (bIsWallRunning)
	{

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

/*void APlayerCharacter::LookUpDown(float value)
{

}*/

/*void APlayerCharacter::TurnRightLeft(float value)
{

}*/

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

/**/
void APlayerCharacter::EndWallRun()
{
	// DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Wall Run Ended"));

	if (WallRunTimeline != nullptr)
	{
		WallRunTimeline->Stop();
	}

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector::ZeroVector);
	bIsWallRunning = false;
	//bIsWallRunningRightSide = false;
	//bIsWallRunningLeftSide = false;

}
