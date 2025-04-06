// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"

#include "DepthsGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DronePawn.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AShipPawn::AShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(ShipMesh);
	ShipMesh->SetSimulatePhysics(false);
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);

	DroneSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DroneSpawnPoint"));
	DroneSpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AShipPawn::BeginPlay()
{
	Super::BeginPlay();
	CameraArm->TargetArmLength = DefaultArmLength;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Actor location %s"), *GetActorLocation().ToString());

	// Spawn Drone
	SpawnDrone();
}

// Called every frame
void AShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*CurrentAngle += AngularSpeed * DeltaTime;
	if (CurrentAngle >= 360.0f)
	{
		CurrentAngle -= 360.0f;
	}
	
	float Radians = FMath::DegreesToRadians(CurrentAngle);
	FVector NewLocation = FVector(FMath::Cos(Radians) * CircleRadius, FMath::Sin(Radians) * CircleRadius, GetActorLocation().Z);
	SetActorLocation(NewLocation);

	FRotator NewRotation = FRotator(0.0f, CurrentAngle, 0.0f);
	SetActorRotation(NewRotation);*/
}

// Called to bind functionality to input
void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MouseClick, ETriggerEvent::Started, this, &AShipPawn::OnMouseClick);
	}
}

void AShipPawn::OnMouseClick()
{
	FHitResult Hit;
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, Hit);

		if (Hit.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());
			if (auto DronePawn = Cast<ADronePawn>(Hit.GetActor()))
			{
				PlayerController->Possess(DronePawn);
				
				if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
				{
					if (ADepthsGameMode* DepthsGameMode = Cast<ADepthsGameMode>(GameMode))
					{
						DepthsGameMode->Dive();
					}
				}
			}
			
		}
	}
}

void AShipPawn::SpawnDrone()
{
	if (UWorld* World = GetWorld())
	{
		if (DronePawnClass->IsValidLowLevel())
		{

			if (AActor* a = World->SpawnActor<AActor>(DronePawnClass, GetActorTransform() + DroneSpawnPoint->GetRelativeTransform()))
			{
				FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
				a->AttachToActor(this, rules);

				FString s = (GetActorTransform() + DroneSpawnPoint->GetRelativeTransform()).ToString();
				UE_LOG(LogTemp, Warning, TEXT(" spawn point %s"), *s);

				UE_LOG(LogTemp, Warning, TEXT("Actuual spawn point %s"), *a->GetActorLocation().ToString());
			}
		}
	}
}


