
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
	Mesh->SetSimulatePhysics(false);

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

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ADronePawn::OnOverlap);

	ResetDrone();

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
		if (bImpluse)
		{
			Mesh->SetLinearDamping(LaunchedLinearDamping);
			bImpluse = false;
		}


		if ( ParentShipPawn->IsValidLowLevel() )
		{
			DepthReached = abs( ParentShipPawn->GetActorLocation().Z - GetActorLocation().Z ) / 100;

			ParentShipPawn->DroneFuelAmount -= DeltaTime * ParentShipPawn->DroneFuelConsumption;

			if( ParentShipPawn->DroneFuelAmount <= 0.0f )
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

	ExtendArm(DeltaTime);
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
	if (ParentShipPawn->IsValidLowLevel())
	{
		return ParentShipPawn->DroneFuelAmount / ParentShipPawn->DroneMaxFuel;
	}
	return 0.0f;
}

void ADronePawn::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADepthsGameMode* DepthsGameMode = nullptr;
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
	{
		DepthsGameMode = Cast<ADepthsGameMode>(GameMode);
	}
	if (AFuelPickup* FuelPickup = Cast<AFuelPickup>(Other))
	{
		if (ParentShipPawn->IsValidLowLevel())
		{
			ParentShipPawn->DroneFuelAmount += 0.1 * ParentShipPawn->DroneMaxFuel;
		}
		if (DepthsGameMode->IsValidLowLevel())
		{
			++DepthsGameMode->PickedUpFuel;
		}
		FuelPickup->OnCollected();
		FuelPickup->Destroy();
	}
	if (AFishPickup* FishPickup = Cast<AFishPickup>(Other))
	{
		if (DepthsGameMode->IsValidLowLevel())
		{
			++DepthsGameMode->PickedUpFish;
		}
		FishPickup->OnCollected();
		FishPickup->Destroy();
	}
	if (ACargoPickup* CargoPickup = Cast<ACargoPickup>(Other))
	{
		if (DepthsGameMode->IsValidLowLevel())
		{
			++DepthsGameMode->PickedUpCargo;
		}
		CargoPickup->OnCollected();
		CargoPickup->Destroy();
	}
}

void ADronePawn::BeginDestroy()
{
	// Begin Destroy is called by garbadge collector not destroyed
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("Begin Destroy called"));

	bHasLaunched = false;

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
		bHasLaunched = true;
		bImpluse = true;

		if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
		{
			if (ADepthsGameMode* DepthsGameMode = Cast<ADepthsGameMode>(GameMode))
			{
				DepthsGameMode->Dive();
			}
		}
	}
	
}

void ADronePawn::Move(const FInputActionValue& Value)
{
	if (bHasLaunched && GetActorLocation().Z <= 0.0f)
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		// TODO Move left & right
		FVector Loc = GetActorLocation();
		Loc.Y += MovementSpeed * MovementVector.X;
		//Loc.Z += MovementSpeed * MovementVector.Y;


		SetActorLocation(Loc);

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
	Mesh->SetSimulatePhysics(false);
	Mesh->SetLinearDamping(DefaultLinearDamping);
	CameraArm->TargetArmLength = DefaultArmLength;
	bHasReachedMaxArmLength = false;

	bHasLaunched = false;
	if (ParentShipPawn->IsValidLowLevel())
	{
		ParentShipPawn->DroneFuelAmount = 50.0f;
	}
	DepthReached = 0.0f;
}

void ADronePawn::ExtendArm(float DeltaTime)
{

	if (bHasLaunched && !bHasReachedMaxArmLength)
	{
		CameraArm->TargetArmLength = FMath::FInterpTo(CameraArm->TargetArmLength, MaxArmLength, DeltaTime, ArmGrowSpeed);

		if (CameraArm->TargetArmLength >= MaxArmLength - 1)
		{
			bHasReachedMaxArmLength = true;
		}
	}
}