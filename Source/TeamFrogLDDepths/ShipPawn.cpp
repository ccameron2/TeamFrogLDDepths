// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"

// Sets default values
AShipPawn::AShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(ShipMesh);
	
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
}

// Called when the game starts or when spawned
void AShipPawn::BeginPlay()
{
	Super::BeginPlay();
	CameraArm->TargetArmLength = DefaultArmLength;
}

// Called every frame
void AShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentAngle += AngularSpeed * DeltaTime;
	if (CurrentAngle >= 360.0f)
	{
		CurrentAngle -= 360.0f;
	}

	float Radians = FMath::DegreesToRadians(CurrentAngle);
	FVector NewLocation = FVector(FMath::Cos(Radians) * CircleRadius, FMath::Sin(Radians) * CircleRadius, GetActorLocation().Z);
	SetActorLocation(NewLocation);

	FRotator NewRotation = FRotator(0.0f, CurrentAngle, 0.0f);
	SetActorRotation(NewRotation);
}

// Called to bind functionality to input
void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

