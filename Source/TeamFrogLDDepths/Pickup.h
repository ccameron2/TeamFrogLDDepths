// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"


UCLASS()
class TEAMFROGLDDEPTHS_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void OnCollected();

private:

	UPROPERTY(EditAnywhere)
	float SpinSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	bool bChooseRandomMesh = false;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> MeshArray;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;


};


