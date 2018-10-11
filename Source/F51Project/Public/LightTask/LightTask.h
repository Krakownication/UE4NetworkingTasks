// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightTask.generated.h"

UCLASS()
class F51PROJECT_API ALightTask : public AActor
{
	GENERATED_BODY()

public:
	ALightTask();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	UPROPERTY(Replicated)
		class UPointLightComponent* PointLight;

	UPROPERTY(Replicated)
		class UBoxComponent* VisualBoxComponent;


	void ToogleLight();

	void RandomizeColor();


	UPROPERTY(ReplicatedUsing = OnRep_IsEnabled)
		bool IsEnabled;
	UFUNCTION()
		virtual void OnRep_IsEnabled();


	UPROPERTY(ReplicatedUsing = OnRep_LightColor)
		FColor LightColor;
	UFUNCTION()
		virtual void OnRep_LightColor();


protected:
	virtual void BeginPlay() override;

	FTimerHandle RandomizeColorTimer;

public:
	virtual void Tick(float DeltaTime) override;

};
