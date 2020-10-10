// Fill out your copyright notice in the Description page of Project Settings.

#include "Saver.h"
#include "SaverWeaponEquipComponent.h"

#include "Weapon/SaverWeapon.h"
#include "Character/SaverCharacter.h"


// Sets default values for this component's properties
USaverWeaponEquipComponent::USaverWeaponEquipComponent()
{
}


// Called when the game starts
void USaverWeaponEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	UseWeapon = Cast<ASaverWeapon>(GetWorld()->SpawnActor(ASaverWeapon::StaticClass()));	
	if (UseWeapon)
	{
		UseWeapon->GetMesh()->SetSkeletalMesh(DefaultWeaponMesh);
		UseWeapon->SetWeaponChip(DefaultWeaponChipClass);
	}
}


// Called every frame
void USaverWeaponEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

