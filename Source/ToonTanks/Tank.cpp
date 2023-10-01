// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"


#include "ToonTanksGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

APlayerController* ATank::GetTankPlayerController() const
{
	return TankPlayerController;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
	EngineSound = FindComponentByClass<UAudioComponent>();

	if (EngineSound)
	{
		EngineSound->SetVolumeMultiplier(1.5f);
	}
}

void ATank::Move(float Value)
{
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	// if any input
	if (Value != 0.f)
	{
		// if player does contrary motion (Value and MoveSpeed signs are different) enforce contrary acceleration
		const float LocalAcceleration = Value * MoveSpeed > 0 ? Acceleration : Acceleration * ContraryAccelerationMultiplier;
		MoveSpeed += LocalAcceleration * Value * DeltaTime;
		MoveSpeed = MoveSpeed > 0 ? std::min(MoveSpeed, MoveSpeedLimit) : std::max(MoveSpeed, -MoveSpeedLimit);
	}
	else
	{
		// decrease speed when no input from player
		MoveSpeed /= 1.f + DeltaTime * IdleSpeedDecreaseMultiplier;
	}

	FVector DeltaLocation{FVector::ZeroVector};
	DeltaLocation.X = MoveSpeed * DeltaTime;

	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation{FRotator::ZeroRotator};
	DeltaRotation.Yaw = Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);

	if (InputComponent->GetAxisValue(TEXT("MoveForward")) < 0)
	{
		DeltaRotation *= -1;
	}

	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::SetPaused()
{
	Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this))->Pause();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ATank::Fire);
	PlayerInputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, &ATank::SetPaused).bExecuteWhenPaused = true;
}

// Called every frames
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!InputEnabled())
	{
		return;
	}

	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		RotateTurret(HitResult.ImpactPoint);
	}

	if (EngineSound)
	{
		EngineSound->SetFloatParameter(TEXT("RPM"), std::abs(MoveSpeed));
	}
}

void ATank::MuteTankEngine()
{
	if (EngineSound)
	{
		EngineSound->SetVolumeMultiplier(0.f);
	}
}
