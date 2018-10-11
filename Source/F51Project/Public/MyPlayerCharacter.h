// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/DestroypackTask/Bomb.h"
#include "MyPlayerCharacter.generated.h"

UCLASS()
class F51PROJECT_API AMyPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyPlayerCharacter();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Replicated)
		class ALightTask* SpawnedLightTask;

	UFUNCTION()
		void SpawnLight();
	UFUNCTION()
		void ToogleLight();
	UFUNCTION()
		void SpawnBomb();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

public:
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSpawnLight();
	void ServerSpawnLight_Implementation();
	bool ServerSpawnLight_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerToogleLight();
	void ServerToogleLight_Implementation();
	bool ServerToogleLight_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSpawnBomb();
	void ServerSpawnBomb_Implementation();
	bool ServerSpawnBomb_Validate();
};
