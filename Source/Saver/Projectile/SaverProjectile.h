// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaverProjectile.generated.h"

UCLASS()
class SAVER_API ASaverProjectile : public AActor
{
	GENERATED_BODY()
	
private:
	// ProjectileMovementComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Saver|Projectile", meta = (AllowPrivateAccess = "true"))
	class USaverProjectileMovementComponent* ProjectileMovement;

public:	
	ASaverProjectile(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ProjectileMovementComponent –¼
	static FName SaverProjectileMovementName;
	
};
