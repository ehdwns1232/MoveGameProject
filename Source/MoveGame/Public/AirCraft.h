#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AirCraft.generated.h"

class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EAirCraftState : uint8
{
	Ground,
	Air
};


UCLASS()
class MOVEGAME_API AAirCraft : public APawn
{
	GENERATED_BODY()

public:
	AAirCraft();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void ApplyGravity(float DeltaTime);

public:
	void ChangeIMC();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void AirMove(const FInputActionValue& Value);
	void AirLook(const FInputActionValue& Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs")
	TArray<UInputMappingContext*> IMCList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs")
	UInputAction* ChangeIMCAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs|Ground")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs|Ground")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs|Air")
	UInputAction* AirMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs|Air")
	UInputAction* AirLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EAirCraftState CurrentState;

	int32 CurrentIMCIndex;
	float MoveSpeed;
	float RotationSpeed;
	float Gravity;
	float GravityAcceleration;
};
