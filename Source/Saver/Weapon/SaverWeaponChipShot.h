// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaverWeaponChipBase.h"
#include "SaverWeaponChipShot.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SAVER_API USaverWeaponChipShot : public USaverWeaponChipBase
{
	GENERATED_BODY()

private:
	// パーティクル
	UPROPERTY(Category = "Saver|Weapon", EditDefaultsOnly)
		UParticleSystem* Particle;

public:
	void Trigger() override ;
	
};
