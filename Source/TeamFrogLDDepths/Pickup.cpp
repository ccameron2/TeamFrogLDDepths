// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Blueprint/UserWidget.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshC"));
	SetRootComponent(StaticMesh);

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
	if(bShouldSetRotate)
		StaticMesh->SetWorldRotation({ 0.0f, 90.0f, 0.0f });

	if (bChooseRandomMesh && !MeshArray.IsEmpty())
	{
		int numMeshes = MeshArray.Num();
		int i = FMath::RandRange(0, numMeshes - 1);

		if (UStaticMesh* mesh = MeshArray[i])
		{
			StaticMesh->SetStaticMesh(MeshArray[i]);
		}
	}
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(GetActorRotation() + FRotator(0, 0, DeltaTime * SpinSpeed));
}

void APickup::OnCollected()
{
	if (WidgetClass)
	{
		UUserWidget* widg = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		widg->AddToViewport();
	}
}
