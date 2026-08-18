#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "PlatformSpawnRow.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Scene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Scene);

	SpawnCount = 100;
	PlatformDataTable = nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = BoxCollision->GetScaledBoxExtent();
	FVector BoxOrigin = BoxCollision->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

void ASpawnVolume::SpawnPlatformByCount(int32 Count)
{
	for (int32 i = 0; i < Count; ++i)
	{
		SpawnRandomPlatform();
	}
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	SpawnPlatformByCount(SpawnCount);
}

void ASpawnVolume::SpawnRandomPlatform()
{
	if (FPlatformSpawnRow* SelectedRow = GetRandomPlatform())
	{
		if (UClass* ActualClass = SelectedRow->PlatformClass.Get())
		{
			SpawnPlatform(ActualClass);
		}
	}
}

FPlatformSpawnRow* ASpawnVolume::GetRandomPlatform() const
{
	if (!PlatformDataTable) return nullptr;

	TArray<FPlatformSpawnRow*> AllRows;
	static const FString ContextString(TEXT("PlatformSpawnContext"));
	PlatformDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FPlatformSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(1.0f, TotalChance);
	float AccumulatedChance = 0.0f;

	for (FPlatformSpawnRow* Row : AllRows)
	{
		AccumulatedChance += Row->SpawnChance;
		if (RandValue <= AccumulatedChance)
		{
			return Row;
		}
	}

	return nullptr;
}

void ASpawnVolume::SpawnPlatform(TSubclassOf<AActor> PlatformClass)
{
	if (!PlatformClass) return;

	GetWorld()->SpawnActor<AActor>(
		PlatformClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
}