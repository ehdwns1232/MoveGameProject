#include "RotatingPlatform.h"
#include "Components/BoxComponent.h"

ARotatingPlatform::ARotatingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(BoxCollision);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);

	RotateVelocity = FRotator(0.0f, 0.0f, 0.0f);
}

void ARotatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	int32 RandIndex = FMath::RandRange(0, MaterialList.Num() - 1);
	StaticMesh->SetMaterial(0, MaterialList[RandIndex]);

	ChangeRotateVelocity();
	GetWorldTimerManager().SetTimer(ChangeHandle, this, &ARotatingPlatform::ChangeRotateVelocity, DelayTime, true);
}

void ARotatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotatePlatform(DeltaTime);
}

void ARotatingPlatform::RotatePlatform(float DeltaTime)
{
	AddActorLocalRotation(RotateVelocity * DeltaTime, true);
}

void ARotatingPlatform::ChangeRotateVelocity()
{
	float RandVelocity = FMath::RandRange(30.0f, 90.0f);
	int32 RandomNum = FMath::RandRange(0, 2);
	int32 RandomNum2 = FMath::RandRange(0, 1);
	FRotator NewRotator = FRotator::ZeroRotator;
	switch (RandomNum)
	{
	case 0:
		NewRotator = FRotator(RandVelocity, 0.0f, 0.0f);
		if (RandomNum2)
		{
			NewRotator *= -1;
		}
		break;
	case 1:
		NewRotator = FRotator(0.0f, RandVelocity, 0.0f);
		if (RandomNum2)
		{
			NewRotator *= -1;
		}
		break;
	case 2:
		NewRotator = FRotator(0.0f, 0.0f, RandVelocity);
		if (RandomNum2)
		{
			NewRotator *= -1;
		}
		break;
	}

	RotateVelocity = NewRotator;
	GetWorldTimerManager().ClearTimer(ChangeHandle);
}

