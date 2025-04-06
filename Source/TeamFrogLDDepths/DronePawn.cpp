
#include "DronePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ADronePawn::ADronePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Mesh);
	Arrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = DefaultArmLength;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsArrowMoving)
		RotateArrow(DeltaTime);

	if ( bHasLaunched )
	{
		DepthReached += SinkAmount;
		SetActorLocation( FVector( 0.0f, 0.0f, -SinkAmount ) );

		FuelAmount -= FuelConsumption;

		if( FuelAmount < 0.0f )
		{
			bHasLaunched = false;
		}
	}



}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(StartRotationAction, ETriggerEvent::Started, this, &ADronePawn::StartRotation);
	}
}

void ADronePawn::StartRotation(const FInputActionValue& Value)
{
	if (!bHasLaunched) // Only deal with rotating the arrow if the drone has not yet been launched
	{
		bIsArrowMoving = !bIsArrowMoving;
	}
	
	
	if (!bIsArrowMoving && !bHasLaunched)
	{
		float angle = Arrow->GetRelativeRotation().Roll;
		float Radians = FMath::DegreesToRadians(angle);

		FVector direction = FVector(0.0f, Radians, Radians);

		FVector LaunchVelocity = direction * LaunchSpeed;

		Mesh->AddImpulse(LaunchVelocity, NAME_None, true);
		Arrow->SetVisibility(false);

		bHasLaunched = true;
	}
	
}

void ADronePawn::RotateArrow(float DeltaTime)
{
	FRotator CurrentRotation = Arrow->GetRelativeRotation();


	if (bIsIncreasing)
	{
		CurrentRotation.Roll += ArrowSpeed * DeltaTime;
		if (CurrentRotation.Roll >= 90.0f)
			bIsIncreasing = false;
	}
	else
	{
		CurrentRotation.Roll -= ArrowSpeed * DeltaTime;
		if (CurrentRotation.Roll <= 0.0f)
			bIsIncreasing = true;
	}
	
	Arrow->SetRelativeRotation(CurrentRotation);
}

void ADronePawn::ResetDrone()
{
	bHasLaunched = false;
	FuelAmount = 50.0f;
	DepthReached = 0.0f;
}