// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Bomb.generated.h"

UCLASS()
class F51PROJECT_API ABomb : public AActor
{
	GENERATED_BODY()

public:
	ABomb();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void FireInDirection(const FVector& ShootDirection);

protected:
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere)
		USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* ProjectileMovementComp;
	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMeshComp;


	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
