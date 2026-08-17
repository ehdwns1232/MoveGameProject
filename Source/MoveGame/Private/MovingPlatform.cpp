#include "MovingPlatform.h"
#include "Components/BoxComponent.h"

AMovingPlatform::AMovingPlatform()
{
 	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(BoxCollision);
	BoxCollision->SetCollisionProfileName(TEXT("BlockAll"));
	BoxCollision->SetNotifyRigidBodyCollision(true);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);

	StartLocation = FVector::ZeroVector;
	MoveVelocity = FVector::ZeroVector;
	MoveDistance = 800.0f;
	DelayTime = 10.0f;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	int32 RandIndex = FMath::RandRange(0, MaterialList.Num() - 1);
	StaticMesh->SetMaterial(0, MaterialList[RandIndex]);
	ChangeMoveVelocity();

	StartLocation = GetActorLocation();
	BoxCollision->OnComponentHit.AddDynamic(this, &AMovingPlatform::OnPlatformHit);
	GetWorldTimerManager().SetTimer(ChangeHandle, this, &AMovingPlatform::ChangeMoveVelocity, DelayTime, true);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	float CurDistance = FVector::Distance(StartLocation, GetActorLocation());
	if (CurDistance > MoveDistance)
	{
		MoveVelocity = -MoveVelocity;
		StartLocation = GetActorLocation();
	}

	AddActorLocalOffset(MoveVelocity * DeltaTime, true);
}

void AMovingPlatform::ChangeMoveVelocity()
{
	float RandSpeed = FMath::FRandRange(100.0f, 300.0f);
	float Value = FMath::RandRange(0, 1) ? -1.0f : 1.0f;
	FVector Direction = FVector::ZeroVector;
	int32 RandNum = FMath::RandRange(0, 2);

	switch (RandNum)
	{
	case 0:
	{
		Direction = FVector(Value, 0.0f, 0.0f);
		break;
	}
	case 1:
	{
		Direction = FVector(Value, 1.0f, 0.0f);
		break;
	}
	case 2:
	{
		Direction = FVector(0.0f, 0.0f, Value);
		break;
	}
	}
	
	MoveVelocity = Direction * RandSpeed;
	StartLocation = GetActorLocation();

	GetWorldTimerManager().ClearTimer(ChangeHandle);
}

void AMovingPlatform::OnPlatformHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ChangeMoveVelocity();
}
