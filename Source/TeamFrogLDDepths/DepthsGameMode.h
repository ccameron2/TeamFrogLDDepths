
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

UENUM(BlueprintType)
enum class Upgrades : uint8 {
	MaxFuel = 0 UMETA(DisplayName = "MAX FUEL"),
	FuelConsumption = 1 UMETA(DisplayName = "FUEL CONSUMPTION")
};


UCLASS()
class TEAMFROGLDDEPTHS_API ADepthsGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ADepthsGameMode();
	
	UFUNCTION()
	void Dive();

	UFUNCTION(BlueprintCallable)
	void StopDive();

	UFUNCTION(BlueprintCallable)
	void Upgrade( Upgrades upgrades );

	UFUNCTION(BlueprintCallable)
	void ShowUpgradeUI();

	UPROPERTY(BlueprintReadOnly)
	int PickedUpFuel = 0;

	UPROPERTY(BlueprintReadOnly)
	int PickedUpFish = 0;

	UPROPERTY(BlueprintReadOnly)
	int PickedUpCargo = 0;

	UFUNCTION(BlueprintCallable)
	FString GetFishAmount();

	UFUNCTION(BlueprintCallable)
	FString GetFuelAmount();

	UFUNCTION(BlueprintCallable)
	FString GetCargoAmount();
	
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UpgradeWidget;

	UPROPERTY()
	UUserWidget* UpgradeWidgetRef;

	UPROPERTY(BlueprintReadOnly)
	ADronePawn* PlayerPawn;

	UPROPERTY(EditAnywhere)
	UAudioComponent* BackgroundMusicComp;

	UPROPERTY(EditAnywhere)
	USoundBase* BackgroundMusic;

private:
	
	FTimerHandle FuelSpawnTimer;
	FTimerHandle FishSpawnTimer;
	FTimerHandle CargoSpawnTimer;
};
