// Fill out your copyright notice in the Description page of Project Settings.

#include "Saver.h"
#include "SaverWeapon.h"

#include "Weapon/SaverWeaponChipBase.h"


FName ASaverWeapon::SaverWeaponMeshName(TEXT("SaverWeaponMesh"));

ASaverWeapon::ASaverWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(ASaverWeapon::SaverWeaponMeshName);
}

// Called when the game starts or when spawned
void ASaverWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASaverWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaverWeapon::SetWeaponChip(TSubclassOf<class USaverWeaponChipBase> FuncClass)
{
	if (FuncClass == nullptr)
	{
		return;
	}

	WeaponChipArray.Empty();

	USaverWeaponChipBase* chip = NewObject<USaverWeaponChipBase>(GetOuter(), FuncClass);
	if (chip)
	{
		WeaponChipArray.Add(chip);
	}
}


void ASaverWeapon::Trigger()
{
	for (USaverWeaponChipBase* chip : WeaponChipArray)
	{
		chip->Trigger();
	}
}

