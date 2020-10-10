// Fill out your copyright notice in the Description page of Project Settings.

#include "Saver.h"
#include "SaverSightComponent.h"


// Sets default values for this component's properties
USaverSightComponent::USaverSightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USaverSightComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
}


// Called every frame
void USaverSightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USaverSightComponent::Enable()
{
	SetComponentTickEnabled(true);
}

void USaverSightComponent::Disable()
{
	SetComponentTickEnabled(false);
}
