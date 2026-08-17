#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

class UBoxComponent;

UCLASS()
class MOVEGAME_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void MovePlatform(float DeltaTime);
	UFUNCTION()
	void ChangeMoveVelocity();
	UFUNCTION()
	void OnPlatformHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TArray<UMaterial*> MaterialList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Location")
	FVector StartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FVector MoveVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float MoveDistance;

	FTimerHandle ChangeHandle;
	float DelayTime;
};
