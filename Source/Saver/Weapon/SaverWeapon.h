// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaverWeapon.generated.h"

UCLASS()
class SAVER_API ASaverWeapon : public AActor
{
	GENERATED_BODY()
	
private:
	// ���b�V��
	UPROPERTY(Category = "Saver|Weapon", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;

public:
	ASaverWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ���b�V�� Get
	class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	// ����`�b�v Set
	void SetWeaponChip(TSubclassOf<class USaverWeaponChipBase> ChipClass);

	// �����V���b�g �g���K�[
	void Trigger();

public:
	// Mesh ��
	static FName SaverWeaponMeshName;

private:
	// ����@�\�z��
	UPROPERTY()
		TArray<class USaverWeaponChipBase*> WeaponChipArray;

};
