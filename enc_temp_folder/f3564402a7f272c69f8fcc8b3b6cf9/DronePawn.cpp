// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePawn.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ADronePawn::ADronePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));



	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	//Mesh->SetupAttachment(DefaultRoot);
	Mesh->SetSimulatePhysics(true);

	ArrowHandle = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowHandle"));
	ArrowHandle->SetupAttachment(Mesh);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(ArrowHandle);
	Arrow->SetRelativeLocation(FVector(-78.0f, 0.0f, 0.0f));
	Arrow->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Arrow->SetHiddenInGame(false);
	Arrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = DefaultArmLength;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);


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
		bIsArrowMoving = !bIsArrowMoving;
	
	
	if (!bIsArrowMoving && !bHasLaunched)
	{
		float angle = ArrowHandle->GetRelativeRotation().Roll;
		float Radians = FMath::DegreesToRadians(angle);

		FVector direction = FVector(0.0f, Radians, Radians);

		FVector LaunchVelocity = direction * LaunchSpeed;

		Mesh->AddImpulse(LaunchVelocity, NAME_None, true);


		bHasLaunched = true;

	}



}

void ADronePawn::RotateArrow(float DeltaTime)
{
	FRotator CurrentRotation = ArrowHandle->GetRelativeRotation();


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


	ArrowHandle->SetRelativeRotation(CurrentRotation);

	CurrentRotation = ArrowHandle->GetRelativeRotation();


}