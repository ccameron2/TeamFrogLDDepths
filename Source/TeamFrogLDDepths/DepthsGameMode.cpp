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
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (APawn* Pawn = PlayerController->GetPawn())
			{
				if (ADronePawn* DronePawn = Cast<ADronePawn>(Pawn))
				{
					PlayerPawn = DronePawn;
				}
			}
		}
		// Start fuel timer
		FTimerDelegate FuelTimerDelegate;
		FuelTimerDelegate.BindLambda([this]()
		{
			 SpawnFuel();
			 UE_LOG(LogTemp, Warning, TEXT("SpawnCalled"));
		});

		if(SpawnRate.Contains("fuel"))
			World->GetTimerManager().SetTimer(FuelSpawnTimer, FuelTimerDelegate,  FMath::RandRange(SpawnRate["fuel"].Min, SpawnRate["fuel"].Max), true, 5.0f);
		


		// Start fish timer
		FTimerDelegate FishTimerDelegate;
		FishTimerDelegate.BindLambda([this]()
		{
			 SpawnFish();
			 UE_LOG(LogTemp, Warning, TEXT("SpawnCalled"));
		});

		if (SpawnRate.Contains("fish"))
			World->GetTimerManager().SetTimer(FishSpawnTimer, FishTimerDelegate,  FMath::RandRange(SpawnRate["fish"].Min, SpawnRate["fish"].Max), true, 5.0f);

		// Start cargo timer
		FTimerDelegate CargoTimerDelegate;
		CargoTimerDelegate.BindLambda([this]()
		{
			 SpawnCargo();
			 UE_LOG(LogTemp, Warning, TEXT("SpawnCalled"));
		});
		if (SpawnRate.Contains("cargo"))
			World->GetTimerManager().SetTimer(CargoSpawnTimer, CargoTimerDelegate, FMath::RandRange(SpawnRate["cargo"].Min, SpawnRate["cargo"].Max), true, 5.0f);
	}
}

void ADepthsGameMode::StopDive()
{
	if (UWorld* World = GetWorld())
	{
		if (FTimerManager* TimerManager = &World->GetTimerManager())
		{
			TimerManager->ClearTimer(FuelSpawnTimer);
			TimerManager->ClearTimer(FishSpawnTimer);
			TimerManager->ClearTimer(CargoSpawnTimer);
		}
	}
}

AActor* ADepthsGameMode::SpawnFuel()
{
	if (UWorld* World = GetWorld())
	{
		if (AActor* FuelActor = World->SpawnActor(FuelSpawnClass))
		{
			FuelActor->SetLifeSpan(30.f);


			if (PlayerPawn->IsValidLowLevel() && PlayerPawn->GetActorLocation().Z <= MinSpawnDepth)
			{
				FuelActor->SetActorLocation(PlayerPawn->GetActorLocation() - FVector{0, static_cast<double>(FMath::RandRange(-2000,2000)), static_cast<double>(FMath::RandRange(1000,10000))});
			}
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
			FishActor->SetLifeSpan(30.f);
			if (PlayerPawn->IsValidLowLevel() && PlayerPawn->GetActorLocation().Z <= MinSpawnDepth)
			{
				FishActor->SetActorLocation(PlayerPawn->GetActorLocation() - FVector{0, static_cast<double>(FMath::RandRange(-2000,2000)), static_cast<double>(FMath::RandRange(1000,10000))});
			}
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
			CargoActor->SetLifeSpan(30.f);
			if (PlayerPawn->IsValidLowLevel() && PlayerPawn->GetActorLocation().Z <= MinSpawnDepth)
			{
				CargoActor->SetActorLocation(PlayerPawn->GetActorLocation() - FVector{0, static_cast<double>(FMath::RandRange(-1000,1000)), static_cast<double>(FMath::RandRange(200,1000))});
			}
			return CargoActor;
		}
	}
	return nullptr;
}
