// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroypackTask.generated.h"

UCLASS()
class F51PROJECT_API ADestroypackTask : public AActor
{
	GENERATED_BODY()

public:
	ADestroypackTask();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health)
		float Health;

	UFUNCTION()
		virtual void OnRep_Health();


protected:
	virtual void BeginPlay() override;

	void UpdateCharText();

	void InitHealth();

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(Replicated)
		class UStaticMeshComponent* CubeMesh;

	UPROPERTY(Replicated)
		class UTextRenderComponent* CharText;


public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	void ServerTakeDamage_Implementation(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	bool ServerTakeDamage_Validate(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

};
