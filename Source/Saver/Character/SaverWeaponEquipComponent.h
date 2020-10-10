// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SaverWeaponEquipComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAVER_API USaverWeaponEquipComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// デフォルト 武器メッシュ
	UPROPERTY(Category = "Saver|Weapon", EditDefaultsOnly)
		USkeletalMesh* DefaultWeaponMesh;

	// デフォルト 武器チップクラス
	UPROPERTY(Category = "Saver|Weapon", EditDefaultsOnly)
		TSubclassOf<class USaverWeaponChipBase> DefaultWeaponChipClass;

public:	
	// Sets default values for this component's properties
	USaverWeaponEquipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 使用武器
	UPROPERTY()
		class ASaverWeapon* UseWeapon;
	
};
