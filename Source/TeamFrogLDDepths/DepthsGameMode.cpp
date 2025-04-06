// Fill out your copyright notice in the Description page of Project Settings.

#include "DepthsGameMode.h"

ADepthsGameMode::ADepthsGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADepthsGameMode::BeginPlay()
{
	if (UWorld* World = GetWorld())
	{
		
		/*if (DronePawnClass->IsValidLowLevel())
		{
			if (AActor* DroneActor = World->SpawnActor(DronePawnClass->GetClass()))
			{
				if (APawn* DronePawn = Cast<APawn>(DroneActor))
				{
					if (APlayerController* PlayerController = World->GetFirstPlayerController())
					{
						PlayerController->Possess(DronePawn);
					}
				}
			}
		}*/
	}
}

void ADepthsGameMode::Tick(float DeltaSeconds)
{
	
}
