// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"


UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const;

	virtual void Tick(float DeltaTime) override;

	void MuteTankEngine();

	bool bAlive{true};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category="Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeed{0.f};
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeedLimit{1.f};
	UPROPERTY(EditAnywhere, Category="Movement")
	float Acceleration{1.f};
	UPROPERTY(EditAnywhere, Category="Movement")
	float RotationSpeed{1.f};

	float ContraryAccelerationMultiplier{4.f};
	float IdleSpeedDecreaseMultiplier{5.f};
	
	void Move(float Value);

	void Turn(float Value);

	void SetPaused();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerController* TankPlayerController;
	class UAudioComponent* EngineSound;

};
