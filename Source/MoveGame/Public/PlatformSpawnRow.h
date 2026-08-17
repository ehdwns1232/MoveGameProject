#pragma once

#include "CoreMinimal.h"
#include "PlatformSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FPlatformSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PlatformName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PlatformClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
