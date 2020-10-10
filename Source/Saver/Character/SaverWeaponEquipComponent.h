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
	// �f�t�H���g ���탁�b�V��
	UPROPERTY(Category = "Saver|Weapon", EditDefaultsOnly)
		USkeletalMesh* DefaultWeaponMesh;

	// �f�t�H���g ����`�b�v�N���X
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
	// �g�p����
	UPROPERTY()
		class ASaverWeapon* UseWeapon;
	
};
