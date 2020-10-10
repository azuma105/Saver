// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "SaverCharacter.generated.h"

UCLASS(config=Game)
class ASaverCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sight, meta = (AllowPrivateAccess = "true"))
	class USaverSightComponent* Sight;

public:
	ASaverCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	// ����
	void OnSight();
	// ������~
	void OnStopSighting();

public:
	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

private:
	/** ���_�f�[�^ */
	struct FPointData
	{
		FPointData()
		{
			Mass   = 1.f;
			Length = 0.f;

			Force    = FVector::ZeroVector;
			Accel    = FVector::ZeroVector;
			Velocity = FVector::ZeroVector;
			Pos      = FVector::ZeroVector;
			PrevPos  = FVector::ZeroVector;
		}

		/** ���� */
		float Mass;
		/** ���� */
		float Length;

		/** �t�H�[�X */
		FVector Force;
		/** �����x */
		FVector Accel;
		/** ���x */
		FVector Velocity;
		/** �ʒu */
		FVector Pos;
		/** �O�̈ʒu */
		FVector PrevPos;

	};


	struct FLinkedMineData
	{
		FLinkedMineData()
		{}
		FLinkedMineData(FName name)
			: LinkedMineName(name)
		{}

		FName LinkedMineName = NAME_None;
	};

	struct FMineLinkData
	{
		FName MineName = NAME_None;
		TArray<FLinkedMineData> LinkedMineDataList;
	};

private:
	void Update(float deltaTime, const FVector& basePos, const FVector& prevBasePos);

	FVector ComputePos(float deltaTime, FPointData& data);

	bool GetMineLinkData(FMineLinkData& data, FName name);

private:
	/** ���_�f�[�^���X�g */
	TArray<FPointData> PointDataList;
	/** �ȑO�̊�ʒu*/
	FVector PrevBasePos;

	bool bInit;

	TArray<FMineLinkData> MineLinkDataList;
	TArray<TArray<FName>> PolygonList;

};
