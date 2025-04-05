// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "DronePawn.generated.h"

class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;


UCLASS()
class TEAMFROGLDDEPTHS_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADronePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void StartRotation(const FInputActionValue& Value);

	void RotateArrow(float DeltaTime);

private:

	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* ArrowHandle;

	UPROPERTY(EditAnywhere)
	UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* CameraArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float DefaultArmLength = 400.0f;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* StartRotationAction;

	UPROPERTY()
	bool bHasLaunched = false;

	UPROPERTY()
	bool bIsArrowMoving = false;

	UPROPERTY(EditAnywhere, Category = tweak)
	float ArrowSpeed = 30.0f;

	UPROPERTY(EditAnywhere)
	bool bIsIncreasing = true;

	UPROPERTY(EditAnywhere, Category = tweak)
	float LaunchSpeed = 1000.0f;
};
