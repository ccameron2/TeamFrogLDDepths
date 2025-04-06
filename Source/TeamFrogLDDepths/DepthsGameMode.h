
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

protected: 
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void Dive();

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
	
	bool bDiving = false;
private:
	FTimerHandle FuelSpawnTimer;
	FTimerHandle FishSpawnTimer;
	FTimerHandle CargoSpawnTimer;

	UPROPERTY()
	TArray<AActor*> FuelActors;
	
	UPROPERTY()
	TArray<AActor*> FishActors;
	
	UPROPERTY()
	TArray<AActor*> CargoActors;
};
