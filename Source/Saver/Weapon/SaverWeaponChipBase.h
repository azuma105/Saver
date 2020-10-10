// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaverWeaponChipBase.generated.h"

/**
 * 
 */
UCLASS()
class SAVER_API USaverWeaponChipBase : public UObject
{
	GENERATED_BODY()
	
public:
	// ワンショット トリガー
	virtual void Trigger() {}
	
};
