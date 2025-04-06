// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "ShipPawn.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class TEAMFROGLDDEPTHS_API AShipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShipPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnMouseClick();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* CameraArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USceneComponent* DroneSpawnPoint;

	UPROPERTY(EditAnywhere)
	float DefaultArmLength = 1000.0f;
	
	UPROPERTY(EditAnywhere)
	float CircleRadius = 2000.0f;
	
	UPROPERTY(EditAnywhere)
	float AngularSpeed = 30.0f;

	float CurrentAngle = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> DronePawnClass;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MouseClick;
};
