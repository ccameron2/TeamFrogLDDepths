// Fill out your copyright notice in the Description page of Project Settings.

#include "DepthsGameMode.h"

ADepthsGameMode::ADepthsGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADepthsGameMode::BeginPlay()
{
	
}

void ADepthsGameMode::Tick(float DeltaSeconds)
{
	
}

void ADepthsGameMode::Dive()
{
	if (UWorld* World = GetWorld())
	{
		// Start fuel timer
		FTimerDelegate FuelTimerDelegate;
		FuelTimerDelegate.BindLambda([this]()
		{
			 FuelActors.Push(ADepthsGameMode::SpawnFuel());
		});
		World->GetTimerManager().SetTimer(FuelSpawnTimer, FuelTimerDelegate,  FMath::RandRange(5.0f, 10.0f), true, 5.0f);
		
		// Start fish timer
		FTimerDelegate FishTimerDelegate;
		FishTimerDelegate.BindLambda([this]()
		{
			 FishActors.Push(ADepthsGameMode::SpawnFish());
		});
		World->GetTimerManager().SetTimer(FishSpawnTimer, FishTimerDelegate,  FMath::RandRange(5.0f, 10.0f), true, 5.0f);

		// Start cargo timer
		FTimerDelegate CargoTimerDelegate;
		CargoTimerDelegate.BindLambda([this]()
		{
			 CargoActors.Push(ADepthsGameMode::SpawnCargo());
		});
		World->GetTimerManager().SetTimer(CargoSpawnTimer, CargoTimerDelegate, FMath::RandRange(5.0f, 10.0f), true, 5.0f);
	}
}

AActor* ADepthsGameMode::SpawnFuel()
{
	if (UWorld* World = GetWorld())
	{
		if (AActor* FuelActor = World->SpawnActor(FuelSpawnClass))
		{
			return FuelActor;
		}
	}
	return nullptr;
}

AActor* ADepthsGameMode::SpawnFish()
{
	if (UWorld* World = GetWorld())
	{
		if (AActor* FishActor = World->SpawnActor(FishSpawnClass))
		{
			return FishActor;
		}
	}
	return nullptr;
}

AActor* ADepthsGameMode::SpawnCargo()
{
	if (UWorld* World = GetWorld())
	{
		if (AActor* CargoActor = World->SpawnActor(CargoSpawnClass))
		{
			return CargoActor;
		}
	}
	return nullptr;
}
