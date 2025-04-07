// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "InputActionValue.h"
#include "DronePawn.generated.h"

class AShipPawn;
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

	UFUNCTION(BlueprintCallable)
	FString GetDepthReached();

	UFUNCTION(BlueprintCallable)
	float GetFuelPercent();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginDestroy() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DepthReached = 0.0f;

private:
	void StartRotation(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);

	void RotateArrow(float DeltaTime);

	void ResetDrone();

	void ExtendArm(float DeltaTime);

private:

	UPROPERTY()
	AShipPawn* ParentShipPawn;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Arrow;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* CameraArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float DefaultArmLength = 400.0f;

	UPROPERTY(EditAnywhere)
	float MaxArmLength = 800.0f;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* StartRotationAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY()
	bool bHasLaunched = false;

	UPROPERTY()
	bool bIsArrowMoving = false;

	UPROPERTY(EditAnywhere)
	float ArrowSpeed = 30.0f;

	UPROPERTY(EditAnywhere)
	bool bIsIncreasing = true;

	UPROPERTY(EditAnywhere)
	float LaunchSpeed = 1000.0f;

	UPROPERTY(EditAnywhere)
	float MovementSpeed = 60.0f;

	UPROPERTY(EditAnywhere)
	float DefaultLinearDamping = 0.1f;

	UPROPERTY(EditAnywhere)
	float LaunchedLinearDamping = 2.0f;

	UPROPERTY(EditAnywhere)
	float ArmGrowSpeed = 1.0f;

	FName FuelPickupName;
	FName FishPickupName;
	FName CargoPickupName;

	bool bImpluse = false;
	bool bHasReachedMaxArmLength = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameOverWidget;

};
