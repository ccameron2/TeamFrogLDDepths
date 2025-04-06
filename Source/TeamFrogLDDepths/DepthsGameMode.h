
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DepthsGameMode.generated.h"

/**
 * 
 */
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

private:
	FTimerHandle FuelSpawnTimer;
	FTimerHandle FishSpawnTimer;
	FTimerHandle CargoSpawnTimer;
};
