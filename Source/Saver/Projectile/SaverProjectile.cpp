// Fill out your copyright notice in the Description page of Project Settings.

#include "Saver.h"
#include "SaverProjectile.h"
#include "Projectile/SaverProjectileMovementComponent.h"


FName ASaverProjectile::SaverProjectileMovementName(TEXT("SaverProjectileMovement"));

ASaverProjectile::ASaverProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<USaverProjectileMovementComponent>(ASaverProjectile::SaverProjectileMovementName);
}

// Called when the game starts or when spawned
void ASaverProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	USceneComponent* SceneComponent = FindComponentByClass<USceneComponent>();
	if (SceneComponent)
	{
		ProjectileMovement->UpdatedComponent = SceneComponent;
	}
}

// Called every frame
void ASaverProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

