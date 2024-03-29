// K-B Enterprises 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"

#define OUT

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FVector LineTraceDirecton = PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + (LineTraceDirecton * Reach);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How far agead of the player can we reach in cm
	UPROPERTY(EditAnywhere)
	float Reach = 100.0f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	
	UInputComponent* InputComponent = nullptr;

	/// Returns current end of reach line
	FVector GetReachLineEnd();

	/// Returns current start of reach line
	FVector GetReachLineStart();

	/// Ray-cast and grab what's in reach
	void Grab();
	
	/// Called when grab is released
	void Release();

	/// Find attached physics
	void FindPhysicsHandleComponent();

	/// Setup (assumed) attached input component
	void SetupInputComponent();

	// Return hit for the first physcis body in reach
	FHitResult GetFirstPhysicsBodyInReach();
};
