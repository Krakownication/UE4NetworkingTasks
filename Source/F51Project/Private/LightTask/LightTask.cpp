// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/LightTask/LightTask.h"
#include "F51Project.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"


ALightTask::ALightTask()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	if (Role == ROLE_Authority)
	{
		PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightTaskPointLight"));
		PointLight->Intensity = 3100;
		PointLight->bVisible = true;
		PointLight->SetLightColor(FColor((uint8)255, (uint8)0, (uint8)0, (uint8)1).ReinterpretAsLinear());
		RootComponent = PointLight;

		VisualBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LightTaskVisualBoxComponent"));
		VisualBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		VisualBoxComponent->bVisible = true;
		VisualBoxComponent->bHiddenInGame = false;
		VisualBoxComponent->SetupAttachment(RootComponent);
	}
}

void ALightTask::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALightTask, PointLight);
	DOREPLIFETIME(ALightTask, VisualBoxComponent);

	DOREPLIFETIME(ALightTask, IsEnabled);
	DOREPLIFETIME(ALightTask, LightColor);
}

void ALightTask::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(RandomizeColorTimer, this, &ALightTask::RandomizeColor, 2, true);
	}
}

void ALightTask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ALightTask::ToogleLight()
{
	if (Role == ROLE_Authority)
	{
		IsEnabled = !IsEnabled;
		OnRep_IsEnabled();
	}
}

void ALightTask::RandomizeColor()
{
	if (Role == ROLE_Authority)
	{
		uint8 newRedValue = (uint8)(rand() % 255);
		uint8 newGreenValue = (uint8)(rand() % 255);
		uint8 newBlueValue = (uint8)(rand() % 255);

		LightColor = FColor(newRedValue, newGreenValue, newBlueValue, (uint8)1);
		OnRep_LightColor();
	}
}


void ALightTask::OnRep_LightColor()
{
	PointLight->SetLightColor(LightColor.ReinterpretAsLinear());
}

void ALightTask::OnRep_IsEnabled()
{
	PointLight->ToggleVisibility();
}
