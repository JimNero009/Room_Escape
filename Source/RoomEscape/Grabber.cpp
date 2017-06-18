// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber is instantiated!"));

	// Find the physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No physics handle on object %s."), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player's viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation); // Mark with macro to remind us the values are changing 
	//UE_LOG(LogTemp, Warning, TEXT("Player viewpoint has location %s and rotation %s."), *(PlayerViewpointLocation.ToString()), *(PlayerViewpointRotation.ToString()));
	
	FVector	EndPoint = PlayerViewpointLocation + PlayerViewpointRotation.Vector()*Reach;

	DrawDebugLine(GetWorld(), 
				  PlayerViewpointLocation,
				  EndPoint,
				  FColor(255, 0, 0),
				  false,
				  -1.0f,
				  0,
				  10.0f);

	/// Set up query params
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	/// Raycast
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		EndPoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
		);

	if (Hit.GetActor()) {
		FString HitName = Hit.GetActor()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Collision detected with %s."), *HitName);
	}
}

