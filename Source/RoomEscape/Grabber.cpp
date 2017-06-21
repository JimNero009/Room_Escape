// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() {
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	FindAndSetupInputComponent();
}

void UGrabber::FindAndSetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("No input component on object %s."), *(GetOwner()->GetName()));
	} else {
		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandleComponent() {
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

	if (!PhysicsHandle) { return; }
	// if the physics handle is attached, move the object
	if (PhysicsHandle->GrabbedComponent) {
		MoveObject();
	}
}

void UGrabber::MoveObject() {
	GetPlayerLocationAndEndPoint(OUT PlayerViewpointLocation, OUT EndPoint);
	if (!PhysicsHandle) { return; }
	// move object we are holding
	PhysicsHandle->SetTargetLocation(EndPoint);
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	GetPlayerLocationAndEndPoint(OUT PlayerViewpointLocation, OUT EndPoint);

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

	return Hit;
}

void UGrabber::GetPlayerLocationAndEndPoint(FVector& PlayerViewpointLocation, FVector& EndPoint) {
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation); // Mark with macro to remind us the values are changing 
	EndPoint = PlayerViewpointLocation + PlayerViewpointRotation.Vector()*Reach;
}

void UGrabber::Grab() {	
	/// Try and reach any actors with physics body collision channel set using raycast
	auto HitResult = GetFirstPhysicsBodyInReach();
	
	/// If we hit something, attach a physics handle
	if (HitResult.GetActor()) {
		auto ComponentToGrab = HitResult.GetComponent();
		if (!PhysicsHandle) { return;  }
		PhysicsHandle->GrabComponent(ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true);
	}
}

void UGrabber::Release() {
	if (!PhysicsHandle) { return;  }
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->ReleaseComponent();
	}
}

