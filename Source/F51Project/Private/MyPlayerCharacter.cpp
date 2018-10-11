// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/MyPlayerCharacter.h"
#include "Public/LightTask/LightTask.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"


AMyPlayerCharacter::AMyPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* CapsuleComponent = GetCapsuleComponent();
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent->bHiddenInGame = false;
	CapsuleComponent->bOwnerNoSee = true;
}

void AMyPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerCharacter, SpawnedLightTask);
}

void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("SpawnLight", IE_Pressed, this, &AMyPlayerCharacter::SpawnLight);
	PlayerInputComponent->BindAction("ToogleLight", IE_Pressed, this, &AMyPlayerCharacter::ToogleLight);

	PlayerInputComponent->BindAction("SpawnBomb", IE_Pressed, this, &AMyPlayerCharacter::SpawnBomb);
}


void AMyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMyPlayerCharacter::MoveForward(float Amount)
{
	if (Amount)
	{
		if (Controller)
		{
			AddMovementInput(GetActorForwardVector(), Amount);
		}
	}
}
void AMyPlayerCharacter::MoveRight(float Amount)
{
	if (Amount)
	{
		if (Controller)
		{
			AddMovementInput(GetActorRightVector(), Amount);
		}
	}
}


void AMyPlayerCharacter::SpawnLight()
{
	ServerSpawnLight();
}
void AMyPlayerCharacter::ToogleLight()
{
	ServerToogleLight();
}
void AMyPlayerCharacter::SpawnBomb()
{
	ServerSpawnBomb();
}

bool AMyPlayerCharacter::ServerSpawnLight_Validate()
{
	return true;
}
void AMyPlayerCharacter::ServerSpawnLight_Implementation()
{
	if (Role == ROLE_Authority)
	{
		if (SpawnedLightTask == nullptr)
		{
			FVector Location = GetActorLocation() + (GetActorForwardVector() * 500);
			FRotator Rotation(0.0f);

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;
			SpawnParameters.Instigator = Instigator;

			SpawnedLightTask = GetWorld()->SpawnActor<ALightTask>(Location, Rotation, SpawnParameters);
		}
	}
}

bool AMyPlayerCharacter::ServerToogleLight_Validate()
{
	return true;
}
void AMyPlayerCharacter::ServerToogleLight_Implementation()
{
	if (Role == ROLE_Authority)
	{
		if (SpawnedLightTask != nullptr)
		{
			SpawnedLightTask->ToogleLight();
		}
	}
}

bool AMyPlayerCharacter::ServerSpawnBomb_Validate()
{
	return true;
}
void AMyPlayerCharacter::ServerSpawnBomb_Implementation()
{
	if (Role == ROLE_Authority)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(FVector(0, 0, 0));
		FRotator MuzzleRotation = CameraRotation;

		FRotator LaunchRotation = CameraRotation;
		FVector LaunchDirection = LaunchRotation.Vector();

		// Skew the aim to be slightly upwards.
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			ABomb* Projectile = World->SpawnActor<ABomb>(MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}
