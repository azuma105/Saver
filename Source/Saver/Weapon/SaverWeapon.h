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
	// メッシュ
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

	// メッシュ Get
	class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	// 武器チップ Set
	void SetWeaponChip(TSubclassOf<class USaverWeaponChipBase> ChipClass);

	// ワンショット トリガー
	void Trigger();

public:
	// Mesh 名
	static FName SaverWeaponMeshName;

private:
	// 武器機能配列
	UPROPERTY()
		TArray<class USaverWeaponChipBase*> WeaponChipArray;

};
