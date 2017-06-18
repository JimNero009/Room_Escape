// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
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
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player's viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation); // Mark with macro to remind us the values are changing 
	//UE_LOG(LogTemp, Warning, TEXT("Player viewpoint has location %s and rotation %s."), *(PlayerViewpointLocation.ToString()), *(PlayerViewpointRotation.ToString()));
	
	FVector	EndPoint = PlayerViewpointLocation + PlayerViewpointRotation.Vector()*Reach;
	//FVector	EndPoint = PlayerViewpointLocation + FVector(0.0f, 0.0f, 100.0f);

	DrawDebugLine(GetWorld(), 
				  PlayerViewpointLocation,
				  EndPoint,
				  FColor(255, 0, 0),
				  false,
				  -1.0f,
				  0,
				  10.0f);
	
	// Raycast out to reach distance

	// See what we hit
}

