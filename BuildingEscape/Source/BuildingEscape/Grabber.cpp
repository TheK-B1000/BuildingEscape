// K-B Enterprises 2019


#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/Controller.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		/// if Physics Handle is found
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle Component is not found in %s!"), *(GetOwner()->GetName()));
	}
}

// Look for attached pawn input component
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("An InputComponent is found in %s!"), *(GetOwner()->GetName()));
		/// Bind The Input Action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("An Input Component is not found in %s!"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));

	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceDirecton = PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + (LineTraceDirecton * Reach);

	/// If we hit something attached a physics hanndle 
	if (ActorHit)
	{
		/// Attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd,
			LineTraceDirecton.Rotation()

		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));

	/// TODO release physics handle
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceDirecton = PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + (LineTraceDirecton * Reach);

	/// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// move the object we are holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceDirecton = PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + (LineTraceDirecton * Reach);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-casting out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType
	(
		Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters

	);

	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}
	return Hit;
}

