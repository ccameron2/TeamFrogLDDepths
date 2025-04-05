
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
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> DronePawnClass;

private:
	bool bDiving = false;
};
