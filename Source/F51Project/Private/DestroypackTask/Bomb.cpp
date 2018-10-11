// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/DestroypackTask/Bomb.h"
#include "Public/DestroypackTask/DestroypackTask.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionSphere->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionSphere->OnComponentHit.AddDynamic(this, &ABomb::OnHit);
	CollisionSphere->InitSphereRadius(15.0f);
	CollisionSphere->bHiddenInGame = false;
	RootComponent = CollisionSphere;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComp->SetUpdatedComponent(CollisionSphere);
	ProjectileMovementComp->InitialSpeed = 3000.0f;
	ProjectileMovementComp->MaxSpeed = 3000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.3f;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/EngineMeshes/Sphere"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}
	StaticMeshComp->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	StaticMeshComp->SetupAttachment(CollisionSphere);

	SetReplicates(true);

	InitialLifeSpan = 3.0f;
}


void ABomb::BeginPlay()
{
	Super::BeginPlay();
}

void ABomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABomb::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComp->Velocity = ShootDirection * ProjectileMovementComp->InitialSpeed;
}

void ABomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role == ROLE_Authority)
	{
		if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
		{
			OtherComponent->AddImpulseAtLocation(ProjectileMovementComp->Velocity * 100.0f, Hit.ImpactPoint);
		}

		if (OtherActor->IsA<ADestroypackTask>())
		{
			ADestroypackTask* DestroypackTask = Cast<ADestroypackTask>(OtherActor);
			DestroypackTask->ServerTakeDamage(25.0f, FDamageEvent(), this->Instigator->GetController(), this);
		}
	}
}
