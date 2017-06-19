// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROOMESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	float Reach = 100.0f; // How far the player can reach to grab things

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	FVector PlayerViewpointLocation, EndPoint;
	FRotator PlayerViewpointRotation;

	void FindAndSetupInputComponent();
	void FindPhysicsHandleComponent();
	const FHitResult GetFirstPhysicsBodyInReach();
	void Grab(); // Raycast and grab what is in reach
	void Release(); // Release whatever we have in our grasp
	void GetPlayerLocationAndEndPoint(FVector & PlayerViewpointLocation, FVector & EndPoint); // Store player's location and end point of their reach in vectors
	void MoveObject();

};
