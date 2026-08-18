#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;
struct FPlatformSpawnRow;

UCLASS()
class MOVEGAME_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void SpawnRandomPlatform();
	FPlatformSpawnRow* GetRandomPlatform() const;
	void SpawnPlatform(TSubclassOf<AActor> PlatformClass);
	FVector GetRandomPointInVolume() const;

	void SpawnPlatformByCount(int32 Count);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* PlatformDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 SpawnCount;
};
