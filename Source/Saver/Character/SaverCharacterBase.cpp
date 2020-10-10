// Fill out your copyright notice in the Description page of Project Settings.

#include "Saver.h"
#include "SaverCharacterBase.h"


// Sets default values
ASaverCharacterBase::ASaverCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASaverCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASaverCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASaverCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
