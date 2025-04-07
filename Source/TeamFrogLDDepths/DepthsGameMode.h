
#pragma once
#include "DronePawn.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DepthsGameMode.generated.h"

USTRUCT(BlueprintType)
struct FSpawnRate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Min;

	UPROPERTY(EditAnywhere)
	int Max;
};


UCLASS()
class TEAMFROGLDDEPTHS_API ADepthsGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ADepthsGameMode();
	
	UFUNCTION()
	void Dive();

	UFUNCTION()
	void StopDive();
	
protected: 
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	AActor* SpawnFuel();

	UFUNCTION()
	AActor* SpawnFish();

	UFUNCTION()
	AActor* SpawnCargo();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> DronePawnClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> FuelSpawnClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> FishSpawnClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> CargoSpawnClass;

	UPROPERTY(EditAnywhere)
	float MinSpawnDepth = -10.0f;

	UPROPERTY(EditAnywhere)
	float MinSpawnRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float MaxSpawnRate = 5.0f;

	UPROPERTY(EditAnywhere)
	TMap<FString, FSpawnRate> SpawnRate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MenuWidget;

private:
	UPROPERTY()
	ADronePawn* PlayerPawn;
	
	FTimerHandle FuelSpawnTimer;
	FTimerHandle FishSpawnTimer;
	FTimerHandle CargoSpawnTimer;
};
