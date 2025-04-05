// Fill out your copyright notice in the Description page of Project Settings.


#include "DepthsGameMode.h"

ADepthsGameMode::ADepthsGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DronePawnBPClass(TEXT("/Game/Blueprints/Characters/BP_DronePawn"));
	if (DronePawnBPClass.Class != NULL)
		DefaultPawnClass = DronePawnBPClass.Class;
	else
		UE_LOG(LogTemp, Warning, TEXT("Class is null"));

	PrimaryActorTick.bCanEverTick = false;
}

void ADepthsGameMode::BeginPlay()
{
}

void ADepthsGameMode::Tick(float DeltaSeconds)
{
}
