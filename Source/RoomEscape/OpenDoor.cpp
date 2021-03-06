// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor(){
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();
	Owner = GetOwner();
	CheckPressurePlatePointerAssigned();
}

void UOpenDoor::CheckPressurePlatePointerAssigned() {
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("No PressurePlate assigned to object %s."), *(GetOwner()->GetName()));
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll the trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > MassThreshold) { // if mass of objects is enough
		OnOpenRequest.Broadcast();
	}
	else {
		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const {
	float TotalMass = 0.0f;

	// find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// add their masses together to get total mass
	for (const auto* OverlappingActor : OverlappingActors) {
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

