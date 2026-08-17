#include "AirCraft.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAirCraft::AAirCraft()
	: MoveAction(nullptr)
	, LookAction(nullptr)
	, AirMoveAction(nullptr)
	, AirLookAction(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(BoxCollision);
	BoxCollision->SetSimulatePhysics(false);
	BoxCollision->SetCollisionProfileName("BlockAll");

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);
	StaticMesh->SetSimulatePhysics(false);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	CurrentIMCIndex = -1;
	MoveSpeed = 600.0f;
	RotationSpeed = 90.0f;
	Gravity = -500.0f;
	GravityAcceleration = 0.0f;
	CurrentState = EAirCraftState::Ground;
}

void AAirCraft::BeginPlay()
{
	Super::BeginPlay();
}

void AAirCraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentState == EAirCraftState::Ground)
	{
		ApplyGravity(DeltaTime);
	}
}

void AAirCraft::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	ChangeIMC();
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ChangeIMCAction)
		{
			EnhancedInput->BindAction(ChangeIMCAction, ETriggerEvent::Started, this, &AAirCraft::ChangeIMC);
		}
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAirCraft::Move);
		}
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAirCraft::Look);
		}
		
		if (AirMoveAction)
		{
			EnhancedInput->BindAction(AirMoveAction, ETriggerEvent::Triggered, this, &AAirCraft::AirMove);
		}
		if (AirLookAction)
		{
			EnhancedInput->BindAction(AirLookAction, ETriggerEvent::Triggered, this, &AAirCraft::AirLook);
		}
	}
}

void AAirCraft::ApplyGravity(float DeltaTime)
{
	FHitResult Hit;
	GravityAcceleration += Gravity * DeltaTime;
	AddActorWorldOffset(FVector(0.0f, 0.0f, GravityAcceleration * DeltaTime), true, &Hit);

	if (Hit.bBlockingHit)
	{
		GravityAcceleration = 0.0f;

		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = FRotator(0.0f, CurrentRotation.Yaw, 0.0f);

		SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 20.0f));
	}
}

void AAirCraft::ChangeIMC()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) return;

	if (IMCList.IsValidIndex(CurrentIMCIndex))
	{
		Subsystem->RemoveMappingContext(IMCList[CurrentIMCIndex]);
	}

	CurrentIMCIndex = (CurrentIMCIndex + 1) % IMCList.Num();
	Subsystem->AddMappingContext(IMCList[CurrentIMCIndex], 0);
	if (CurrentIMCIndex == 0)
	{
		CurrentState = EAirCraftState::Ground;
		MoveSpeed = 600.0f;
	}
	else
	{
		CurrentState = EAirCraftState::Air;
		MoveSpeed /= 2;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("IMCIndex : %d"), CurrentIMCIndex));
}

void AAirCraft::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D MoveInput = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddActorLocalOffset(FVector(MoveInput.X * (MoveSpeed * DeltaTime), 0.0f, 0.0f), true);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddActorLocalOffset(FVector(0.0f, MoveInput.Y * (MoveSpeed * DeltaTime), 0.0f), true);
	}
}

void AAirCraft::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (!FMath::IsNearlyZero(LookInput.X))
	{
		AddActorLocalRotation(FRotator(0.0f, LookInput.X * (RotationSpeed * DeltaTime), 0.0f), true);
	}

	// 자연스러운 비행체를 위해 Y축 제거했음
	/*if (!FMath::IsNearlyZero(LookInput.Y))
	{
		AddActorLocalRotation(FRotator(LookInput.Y * (RotationSpeed * DeltaTime), 0.0f, 0.0f), true);
	}*/
}

void AAirCraft::AirMove(const FInputActionValue& Value)
{
	const FVector MoveInput = Value.Get<FVector>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddActorLocalOffset(FVector(MoveInput.X * (MoveSpeed * DeltaTime), 0.0f, 0.0f), true);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddActorLocalOffset(FVector(0.0f, MoveInput.Y * (MoveSpeed * DeltaTime), 0.0f), true);
	}
	if (!FMath::IsNearlyZero(MoveInput.Z))
	{
		AddActorLocalOffset(FVector(0.0f, 0.0f, MoveInput.Z * (MoveSpeed * DeltaTime)), true);
	}
}

void AAirCraft::AirLook(const FInputActionValue& Value)
{
	const FVector LookInput = Value.Get<FVector>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (!FMath::IsNearlyZero(LookInput.X))
	{
		AddActorLocalRotation(FRotator(0.0f, LookInput.X * (RotationSpeed * DeltaTime), 0.0f), true);
	}
	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		AddActorLocalRotation(FRotator(LookInput.Y * (RotationSpeed * DeltaTime), 0.0f, 0.0f), true);
	}
	if (!FMath::IsNearlyZero(LookInput.Z))
	{
		AddActorLocalRotation(FRotator(0.0f, 0.0f, LookInput.Z * (RotationSpeed * DeltaTime)), true);
	}
}

