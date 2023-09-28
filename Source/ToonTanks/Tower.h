// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"


UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category="Combat")
	double FireRange{0.f};
	UPROPERTY(EditAnywhere, Category="Combat")
	float FireRate{0.f};

	class ATank* Target;
	FTimerHandle FireRateTimerHandle;

	void CheckFireCondition();
	bool InFireRange() const;
};
