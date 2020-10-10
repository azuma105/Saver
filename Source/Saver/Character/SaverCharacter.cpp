// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Saver.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "SaverCharacter.h"
#include "Character/SaverSightComponent.h"

#include "DrawDebugHelpers.h"

namespace SaverCharacterInternal
{
	//const int32 PointNum = 3;
	const int32 PointNum = 1;
	const float MineMass = 1.f;
	const float PointLength = 40.f;
	const float Gravity = 980.f;
}

//////////////////////////////////////////////////////////////////////////
// ASaverCharacter

ASaverCharacter::ASaverCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	Sight = CreateDefaultSubobject<USaverSightComponent>(TEXT("Sight"));

	using namespace SaverCharacterInternal;

	PrimaryActorTick.bCanEverTick = true;

	for (int32 lc0 = 0; lc0 < PointNum; ++lc0)
	{
		FPointData data;
		data.Length = PointLength;

		if (lc0 == PointNum - 1)
		{
			//data.Mass = MineMass;
		}

		PointDataList.Add(data);
	}

	PrevBasePos = FVector::ZeroVector;

	bInit = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASaverCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASaverCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASaverCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASaverCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASaverCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASaverCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASaverCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASaverCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Sight", IE_Pressed, this, &ASaverCharacter::OnSight);
	PlayerInputComponent->BindAction("Sight", IE_Released, this, &ASaverCharacter::OnStopSighting);


	// ïùóDêÊíTçı
#if 0
	FMineLinkData mineLinkData1;
	mineLinkData1.MineName = TEXT("1");
	mineLinkData1.LinkedMineDataList.Add(FLinkedMineData(TEXT("2")));

	FMineLinkData mineLinkData2;
	mineLinkData2.MineName = TEXT("2");
	mineLinkData2.LinkedMineDataList.Add(FLinkedMineData(TEXT("1")));
	mineLinkData2.LinkedMineDataList.Add(FLinkedMineData(TEXT("3")));
	mineLinkData2.LinkedMineDataList.Add(FLinkedMineData(TEXT("4")));

	FMineLinkData mineLinkData3;
	mineLinkData3.MineName = TEXT("3");
	mineLinkData3.LinkedMineDataList.Add(FLinkedMineData(TEXT("2")));
	mineLinkData3.LinkedMineDataList.Add(FLinkedMineData(TEXT("4")));

	FMineLinkData mineLinkData4;
	mineLinkData4.MineName = TEXT("4");
	mineLinkData4.LinkedMineDataList.Add(FLinkedMineData(TEXT("2")));
	mineLinkData4.LinkedMineDataList.Add(FLinkedMineData(TEXT("3")));

	MineLinkDataList.Add(mineLinkData1);
	MineLinkDataList.Add(mineLinkData2);
	MineLinkDataList.Add(mineLinkData3);
	MineLinkDataList.Add(mineLinkData4);
#else
	FMineLinkData mineLinkData1;
	mineLinkData1.MineName = TEXT("1");
	mineLinkData1.LinkedMineDataList.Add(FLinkedMineData(TEXT("2")));
	mineLinkData1.LinkedMineDataList.Add(FLinkedMineData(TEXT("5")));

	FMineLinkData mineLinkData2;
	mineLinkData2.MineName = TEXT("2");
	mineLinkData2.LinkedMineDataList.Add(FLinkedMineData(TEXT("1")));
	mineLinkData2.LinkedMineDataList.Add(FLinkedMineData(TEXT("4")));

	FMineLinkData mineLinkData3;
	mineLinkData3.MineName = TEXT("3");
	mineLinkData3.LinkedMineDataList.Add(FLinkedMineData(TEXT("5")));
	mineLinkData3.LinkedMineDataList.Add(FLinkedMineData(TEXT("6")));

	FMineLinkData mineLinkData4;
	mineLinkData4.MineName = TEXT("4");
	mineLinkData4.LinkedMineDataList.Add(FLinkedMineData(TEXT("2")));
	mineLinkData4.LinkedMineDataList.Add(FLinkedMineData(TEXT("5")));
	mineLinkData4.LinkedMineDataList.Add(FLinkedMineData(TEXT("6")));

	FMineLinkData mineLinkData5;
	mineLinkData5.MineName = TEXT("5");
	mineLinkData5.LinkedMineDataList.Add(FLinkedMineData(TEXT("1")));
	mineLinkData5.LinkedMineDataList.Add(FLinkedMineData(TEXT("3")));
	mineLinkData5.LinkedMineDataList.Add(FLinkedMineData(TEXT("4")));
	mineLinkData5.LinkedMineDataList.Add(FLinkedMineData(TEXT("6")));

	FMineLinkData mineLinkData6;
	mineLinkData6.MineName = TEXT("6");
	mineLinkData6.LinkedMineDataList.Add(FLinkedMineData(TEXT("3")));
	mineLinkData6.LinkedMineDataList.Add(FLinkedMineData(TEXT("4")));
	mineLinkData6.LinkedMineDataList.Add(FLinkedMineData(TEXT("5")));

	MineLinkDataList.Add(mineLinkData1);
	MineLinkDataList.Add(mineLinkData2);
	MineLinkDataList.Add(mineLinkData3);
	MineLinkDataList.Add(mineLinkData4);
	MineLinkDataList.Add(mineLinkData5);
	MineLinkDataList.Add(mineLinkData6);
#endif

	for (const FMineLinkData& mineLinkData0 : MineLinkDataList)
	{
		TQueue<TArray<FName>> queue;

		{
			TArray<FName> vertexMineNameList;
			vertexMineNameList.Add(mineLinkData0.MineName);

			queue.Enqueue(vertexMineNameList);
		}

		bool bAddPolygon = false;

		while (!queue.IsEmpty() && !bAddPolygon)
		{
			TArray<FName> vertexMineNameList;
			if (!queue.Dequeue(vertexMineNameList)) break;

			FMineLinkData mineLinkData1;
			FName lastVertexMineName = vertexMineNameList.Last();
			if (!GetMineLinkData(mineLinkData1, lastVertexMineName)) break;
			
			for (const FLinkedMineData& linkedMineData : mineLinkData1.LinkedMineDataList)
			{
				FName linkedMineName = linkedMineData.LinkedMineName;

				if (vertexMineNameList.Num() >= 3 &&
					vertexMineNameList[0] == linkedMineName
					)
				{
					TArray<FName> newVertexMineNameList = vertexMineNameList;
					newVertexMineNameList.Add(linkedMineName);

					PolygonList.Add(newVertexMineNameList);
					bAddPolygon = true;

					break;
				}
				
				int32 index0 = 1;
				if (vertexMineNameList.Num() <= 2)
				{
					index0 = 0;
				}

				bool bExistVertexMineName = false;

				for (int32 lc0 = index0; lc0 < vertexMineNameList.Num(); ++lc0)
				{
					if (vertexMineNameList[lc0] == linkedMineName)
					{
						bExistVertexMineName = true;
						break;
					}
				}

				if (!bExistVertexMineName)
				{
					TArray<FName> newVertexMineNameList = vertexMineNameList;
					newVertexMineNameList.Add(linkedMineName);

					queue.Enqueue(newVertexMineNameList);
				}
			}
		}
	}
}

bool ASaverCharacter::GetMineLinkData(FMineLinkData& data, FName name)
{
	for (FMineLinkData& mineLinkData : MineLinkDataList)
	{
		if (mineLinkData.MineName == name)
		{
			data = mineLinkData;
			return true;
		}
	}

	return false;
}

void ASaverCharacter::OnSight()
{
	Sight->Enable();
}

void ASaverCharacter::OnStopSighting()
{
	Sight->Disable();
}

void ASaverCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASaverCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASaverCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASaverCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASaverCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASaverCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASaverCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ASaverCharacter::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction & ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	USkeletalMeshComponent* mesh = GetMesh();
	if (mesh == nullptr || !mesh->DoesSocketExist(TEXT("foot_r"))) return;

	FVector socketPos = mesh->GetSocketLocation(TEXT("foot_r"));

	if (!bInit)
	{
		bInit = true;

		FVector prevPos = socketPos;

		for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
		{
			FPointData& currentData = PointDataList[lc0];

			currentData.Pos = prevPos - FVector::UpVector * currentData.Length;
		}
	}

	Update(DeltaTime, socketPos, PrevBasePos);

	PrevBasePos = socketPos;
}

void ASaverCharacter::Update(float deltaTime, const FVector& basePos, const FVector& prevBasePos)
{
	using namespace SaverCharacterInternal;

	static float constDrag = 0.1f;

	for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
	{
		FPointData& currentData = PointDataList[lc0];

		currentData.Force -= constDrag * currentData.Velocity;
		currentData.Force += currentData.Mass * -FVector::UpVector * Gravity;
		currentData.Accel = currentData.Force / currentData.Mass;
		currentData.Velocity += currentData.Accel * deltaTime;
		currentData.Pos += currentData.Velocity * deltaTime;

		currentData.Force = FVector::ZeroVector;
	}

	{
		static float constK = 1000.f;
		static float constDamping = 50.f;

		FVector prevPointPos = basePos;

		for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
		{
			FPointData& currentData = PointDataList[lc0];

			FVector currentToPrevVec = prevPointPos - currentData.Pos;
			float currentToPrevSize = currentToPrevVec.Size();
			currentToPrevVec = (currentToPrevSize == 0.f)
				? -FVector::UpVector
				: currentToPrevVec.GetUnsafeNormal();

			float diff = currentToPrevSize - currentData.Length;
			FVector diffVec = currentToPrevVec * diff;

			currentData.Pos += diffVec;

			currentData.Force += constK * diffVec;

			float dampingDot = FVector::DotProduct(currentData.Velocity, currentToPrevVec);
			float damping = constDamping * dampingDot;
			currentData.Force -= damping * currentToPrevVec;
		}
	}

	{
		FVector prevPointPos = basePos;

		for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
		{
			FPointData& currentData = PointDataList[lc0];
		
			DrawDebugLine(
				GetWorld(),
				prevPointPos,
				currentData.Pos,
				FColor::Red
			);

			DrawDebugSphere(
				GetWorld(),
				currentData.Pos,
				5.f,
				10,
				FColor::Red
			);
		}
	}

#if 0
	static float constK = 0.98f;
	static float constDamping = 0.1f;
	static float constDrag = 0.f;

	for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
	{
		FPointData& currentData = PointDataList[lc0];

		currentData.Velocity = currentData.Mass * -FVector::UpVector * Gravity * deltaTime;
		currentData.Pos += currentData.Velocity * deltaTime;
	}

	for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
	{
		FPointData& currentData = PointDataList[lc0];

		if (lc0 == 0)
		{
			FVector baseToCurrentVec = currentData.Pos - basePos;
			float dist = FVector::Dist(basePos, currentData.Pos);

			baseToCurrentVec = (dist == 0.f)
				? FVector::UpVector
				: baseToCurrentVec.GetUnsafeNormal();

			FVector force = constK * (dist - currentData.Length) * baseToCurrentVec;
			currentData.Velocity -= force;

			FVector relativeVel = -currentData.Velocity;
			float damping = constDamping * FVector::DotProduct(relativeVel, baseToCurrentVec);
			currentData.Velocity -= damping * baseToCurrentVec;
		}
	}

	FVector prevPointPos = basePos;

	for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
	{
		FPointData& currentData = PointDataList[lc0];

		//currentData.Velocity -= constDrag * currentData.Velocity;

		//currentData.Pos += currentData.Velocity * deltaTime;

		currentData.Pos += currentData.Velocity;

		// çSë©èàóù
		//FVector prevToCurrentVec = currentData.Pos - prevPointPos;
		//prevToCurrentVec = (prevToCurrentVec.Size() == 0.f)
		//	? -FVector::UpVector
		//	: prevToCurrentVec.GetUnsafeNormal();

		//currentData.Pos = prevPointPos + prevToCurrentVec * currentData.Length;

		DrawDebugLine(
			GetWorld(),
			prevPointPos,
			currentData.Pos,
			FColor::Red
		);

		DrawDebugSphere(
			GetWorld(),
			currentData.Pos,
			5.f,
			10,
			FColor::Red
		);
	}
#endif
#if 0
	static float constK = 50.f;
	static float constDamping = 0.5f;
	static float constDrag = 0.9f;

	for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
	{
		FPointData& currentData = PointDataList[lc0];

		currentData.Force = currentData.Mass * -FVector::UpVector * Gravity;
	}

	for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
	{
		FPointData& currentData = PointDataList[lc0];

		if (lc0 == 0)
		{
			FVector baseToCurrentVec = currentData.Pos - basePos;
			float dist = FVector::Dist(basePos, currentData.Pos);

			baseToCurrentVec = (dist == 0.f)
				? FVector::UpVector
				: baseToCurrentVec.GetUnsafeNormal();

			FVector force = constK * (dist - currentData.Length) * baseToCurrentVec;
			currentData.Force -= force;

			FVector relativeVel = -currentData.Velocity;
			float damping = constDamping * FVector::DotProduct(relativeVel, baseToCurrentVec);
			currentData.Force -= damping * baseToCurrentVec;
		}
	}

	FVector prevPointPos = basePos;

	for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
	{
		FPointData& currentData = PointDataList[lc0];

		currentData.Force -= constDrag * currentData.Velocity;

		FVector acc = currentData.Force / currentData.Mass;
		currentData.Velocity += acc * deltaTime;
		currentData.Pos += currentData.Velocity * deltaTime;

		// çSë©èàóù
		FVector prevToCurrentVec = currentData.Pos - prevPointPos;
		prevToCurrentVec = (prevToCurrentVec.Size() == 0.f)
			? -FVector::UpVector
			: prevToCurrentVec.GetUnsafeNormal();

		currentData.Pos = prevPointPos + prevToCurrentVec * currentData.Length;

		DrawDebugLine(
			GetWorld(),
			prevPointPos,
			currentData.Pos,
			FColor::Red
		);

		DrawDebugSphere(
			GetWorld(),
			currentData.Pos,
			5.f,
			10,
			FColor::Red
		);
	}
#endif
#if 0
	FVector moveDelta = basePos - prevBasePos;

	float totalLength = 0.f;

	for (const FPointData& data : PointDataList)
	{
		totalLength += data.Length;
	}

	{
		FPointData& lastData = PointDataList[PointDataList.Num() - 1];
		FVector nextLastPos = ComputePos(deltaTime, lastData);

		float dist = FVector::Dist(basePos, nextLastPos);
		if (dist >= totalLength)
		{
			FVector lastToBaseVec = nextLastPos - basePos;
			lastToBaseVec = (dist == 0.f)
				? FVector::UpVector
				: lastToBaseVec.GetUnsafeNormal();

			float dot = FVector::DotProduct(lastToBaseVec, -FVector::UpVector);
			float deg = FMath::RadiansToDegrees(FMath::Acos(dot));
		}
	}

	// ç≈èIí[Ç©ÇÁÇÃçSë©
	{
		for (int32 lc0 = PointDataList.Num() - 2; lc0 >= 0; --lc0)
		{
			FPointData& currentData = PointDataList[lc0];
			FPointData& nextData = PointDataList[lc0 + 1];

			currentData.Pos = ComputePos(deltaTime, currentData);

			FVector nextToCurrentVec = currentData.Pos - nextData.Pos;
			nextToCurrentVec = (nextToCurrentVec.Size() == 0.f)
				? FVector::UpVector
				: nextToCurrentVec.GetUnsafeNormal();

			currentData.Pos = nextData.Pos + nextToCurrentVec * nextData.Length;
		}
	}

	// äÓèÄà íuÇ©ÇÁÇÃçSë©
	{
		FVector prevPointPos = basePos;

		for (int32 lc0 = 0; lc0 < PointDataList.Num(); ++lc0)
		{
			FPointData& data = PointDataList[lc0];

			FVector prevToCurrentVec = data.Pos - prevPointPos;
			prevToCurrentVec = (prevToCurrentVec.Size() == 0.f)
				? -FVector::UpVector
				: prevToCurrentVec.GetUnsafeNormal();

			data.Pos = prevPointPos + prevToCurrentVec * data.Length;

			DrawDebugLine(
				GetWorld(),
				prevPointPos,
				data.Pos,
				FColor::Red
			);

			DrawDebugSphere(
				GetWorld(),
				data.Pos,
				5.f,
				10,
				FColor::Red
			);

			prevPointPos = data.Pos;
		}
	}
#endif

	FVector offset = FVector::ForwardVector * 2000.f;

	for (int32 lc0 = 0; lc0 < (int32)EAlphaBlendOption::Custom; ++lc0)
	{
		UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAlphaBlendOption"));
		FString enumStr = pEnum->GetNameStringByIndex(static_cast<uint8>(lc0));

		DrawDebugString(
			GetWorld(),
			offset,
			enumStr
		);

		FAlphaBlend alphaBlend(1.f);
		alphaBlend.SetBlendOption((EAlphaBlendOption)lc0);

		float alpha = 0.f;
		FVector prevPos = FVector::ZeroVector;

		while (!alphaBlend.IsComplete())
		{
			alphaBlend.SetAlpha(alpha);

			float blendedValue = alphaBlend.GetBlendedValue();

			FVector pos = FVector(alpha, -blendedValue, 0.f);

			if (alpha != 0.f)
			{
				DrawDebugLine(
					GetWorld(),
					offset + prevPos * 100.f,
					offset + pos * 100.f,
					FColor::Red
				);
			}

			alpha += 0.05f;
			prevPos = pos;
		}

		offset += FVector::ForwardVector * 100.f;
	}

}

FVector ASaverCharacter::ComputePos(float deltaTime, FPointData & data)
{
	using namespace SaverCharacterInternal;

	// â¡ë¨ìx
	FVector acc = FVector::ZeroVector;
	acc.Z = data.Mass * Gravity;

	// ë¨ìx
	FVector vel = FVector::ZeroVector;
	vel = acc * deltaTime;

	// à íu
	return vel * 0.5f * deltaTime;
}
