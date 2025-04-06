
#include "DronePawn.h"

#include "EngineUtils.h"
#include "CargoPickup.h"
#include "DepthsGameMode.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShipPawn.h"
#include "FishPickup.h"
#include "FuelPickup.h"
#include "Kismet/GameplayStatics.h"

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
	
	for ( TActorIterator<AShipPawn> iter = TActorIterator<AShipPawn>( GetWorld() ); iter; ++iter )
	{
		ParentShipPawn = *iter;
	}
}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsArrowMoving)
		RotateArrow(DeltaTime);

	if ( bHasLaunched && GetActorLocation().Z < 0.0f )
	{
		if ( ParentShipPawn->IsValidLowLevel() )
		{
			DepthReached = abs( ParentShipPawn->GetActorLocation().Z - GetActorLocation().Z ) / 100;
		}

		FuelAmount -= DeltaTime * FuelConsumption;

		if( FuelAmount <= 0.0f )
		{
			bHasLaunched = false;
			if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
			{
				PlayerController->Possess(ParentShipPawn);
				ParentShipPawn->SpawnDrone();
				Destroy();
			}
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADronePawn::Move);
	}
}

FString ADronePawn::GetDepthReached()
{
	if ( GetActorLocation().Z <= 0.0f )
	{
		return FString::Printf(TEXT("Depth Reached: %.0fm"), DepthReached);
	}
	return FString::Printf(TEXT(""));	
}

float ADronePawn::GetFuelPercent()
{
	return FuelAmount / MaxFuel;
}

void ADronePawn::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFuelPickup* FuelPickup = Cast<AFuelPickup>(OverlappedComp))
	{
		FuelAmount += 0.1 * MaxFuel;
		FuelPickup->Destroy();
	}
	if (AFishPickup* FishPickup = Cast<AFishPickup>(OverlappedComp))
	{
		
		FishPickup->Destroy();
	}
	if (ACargoPickup* CargoPickup = Cast<ACargoPickup>(OverlappedComp))
	{
		CargoPickup->Destroy();
	}
}

void ADronePawn::BeginDestroy()
{
	Super::BeginDestroy();

	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
	{
		if (ADepthsGameMode* DepthsGameMode = Cast<ADepthsGameMode>(GameMode))
		{
			DepthsGameMode->StopDive();
		}
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

		Mesh->SetSimulatePhysics(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->AddImpulse(LaunchVelocity, NAME_None, true);
		Arrow->SetVisibility(false);
		DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		//DetachRootComponentFromParent(true);
		bHasLaunched = true;
	}
	
}

void ADronePawn::Move(const FInputActionValue& Value)
{
	if (!bHasLaunched && GetActorLocation().Z >= 0.0f) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	MoveRight(MovementVector.X * 10);
	MoveUp_World(MovementVector.Y * 10);

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