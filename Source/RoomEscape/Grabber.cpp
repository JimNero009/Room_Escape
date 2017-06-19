// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"

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

	FindPhysicsHandleComponent();
	FindAndSetupInputComponent();

}

void UGrabber::FindAndSetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("No input component on object %s."), *(GetOwner()->GetName()));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Input component found on object %s."), *(GetOwner()->GetName()));
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

	// TODO refactor this
	/// Get player's viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation); // Mark with macro to remind us the values are changing 
																														  //UE_LOG(LogTemp, Warning, TEXT("Player viewpoint has location %s and rotation %s."), *(PlayerViewpointLocation.ToString()), *(PlayerViewpointRotation.ToString()));

	FVector	EndPoint = PlayerViewpointLocation + PlayerViewpointRotation.Vector()*Reach;

	// if the physics handle is attached, move the object
	if (PhysicsHandle->GrabbedComponent) {
		// move object we are holding
		PhysicsHandle->SetTargetLocation(EndPoint);
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	/// Get player's viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation); // Mark with macro to remind us the values are changing 
																														  //UE_LOG(LogTemp, Warning, TEXT("Player viewpoint has location %s and rotation %s."), *(PlayerViewpointLocation.ToString()), *(PlayerViewpointRotation.ToString()));

	FVector	EndPoint = PlayerViewpointLocation + PlayerViewpointRotation.Vector()*Reach;

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

	return Hit;
}

void UGrabber::Grab() {	
	UE_LOG(LogTemp, Warning, TEXT("Grab called!"));
	/// Try and reach any actors with physics body collision channel set using raycast
	auto HitResult = GetFirstPhysicsBodyInReach();
	
	/// If we hit something, attach a physics handle
	if (HitResult.GetActor()) {
		auto ComponentToGrab = HitResult.GetComponent();
		PhysicsHandle->GrabComponent(ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Release called!"));
	/// TODO release physics handle
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->ReleaseComponent();
	}
}

