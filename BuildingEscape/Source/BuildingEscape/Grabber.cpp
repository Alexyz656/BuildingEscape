// Copyright Hernan Medina 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "BuildingEscape.h"

#define OUT

FVector PlayerViewPointLocation;
FRotator PlayerViewPointRotation;

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
	FindInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PhysicsHandle) return;

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * reach;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("This should not happen for %s PHYSICS"), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBody()
{
	FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());
	FHitResult FHitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT FHitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return FHitResult;
}

void UGrabber::FindInputComponent() {

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("This should not happen for %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	auto HitResult = GetFirstPhysicsBody();
	auto ComponentToGrab = HitResult.GetComponent();
	if (HitResult.GetActor()) {
		if (!PhysicsHandle) return;

		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation());
	}
}

void UGrabber::Release() {
	if (!PhysicsHandle) return;
	PhysicsHandle->ReleaseComponent();
}