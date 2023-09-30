// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category="Combat")
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditAnywhere, Category="Combat")
	float DamageAmount{0.f};

	UPROPERTY(VisibleAnywhere, Category="Movement")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category="Appearance")
	UParticleSystemComponent* SmokeTrailParticleComponent;

	UPROPERTY(EditAnywhere, Category="Appearance")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category="Appearance")
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

	UPROPERTY(EditAnywhere, Category="Appearance")
	TSubclassOf<UCameraShakeBase> LaunchCameraShakeClass;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);
};
