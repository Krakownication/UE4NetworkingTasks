// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/DestroypackTask/DestroypackTask.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"


ADestroypackTask::ADestroypackTask()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	if (Role == ROLE_Authority)
	{
		CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestroypackTaskBox"));
		CubeMesh->bHiddenInGame = false;
		CubeMesh->Mobility = EComponentMobility::Movable;
		auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/VREditor/BasicMeshes/SM_Cube_01"));
		if (MeshAsset.Succeeded())
		{
			CubeMesh->SetStaticMesh(MeshAsset.Object);
		}
		CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SetRootComponent(CubeMesh);


		CharText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DestroypackTaskBoxText"));
		CharText->SetHorizontalAlignment(EHTA_Center);
		CharText->SetTextRenderColor(FColor(0, 0, 0, 1));
		CharText->SetRelativeLocation(FVector(0, 0, 25));
		CharText->SetupAttachment(GetRootComponent());

		SetReplicates(true);
	}
}

void ADestroypackTask::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADestroypackTask, CubeMesh);
	DOREPLIFETIME(ADestroypackTask, CharText);

	DOREPLIFETIME(ADestroypackTask, Health);
}

void ADestroypackTask::BeginPlay()
{
	Super::BeginPlay();

	InitHealth();
}

void ADestroypackTask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ADestroypackTask::UpdateCharText()
{
	FString NewText = FString("Health: ") + FString::SanitizeFloat(Health);

	CharText->SetText(FText::FromString(NewText));
}

void ADestroypackTask::InitHealth()
{
	Health = MaxHealth;
	UpdateCharText();
}

void ADestroypackTask::OnRep_Health()
{
	UpdateCharText();
}


float ADestroypackTask::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (Role == ROLE_Authority)
	{
		Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

		Health -= Damage;

		if (Health <= 0)
			InitHealth();

		UpdateCharText();	
	}

	return Health;
}

bool ADestroypackTask::ServerTakeDamage_Validate(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return true;
}
void ADestroypackTask::ServerTakeDamage_Implementation(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}
