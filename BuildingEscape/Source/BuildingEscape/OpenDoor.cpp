// Copyright Hernan Medina 2018

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Gameframework/Actor.h"



// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();	
}

void UOpenDoor::OpenDoor() {
	Owner->SetActorRotation(FRotator(0.f, openAngle, 0.f));
}

void UOpenDoor::CloseDoor() {
	Owner->SetActorRotation(FRotator(0.f, -90.f, 0.f));
}

float UOpenDoor::GetMassOnPlate()
{
	TArray<AActor*> OverlappingActors;
	pressurePlate->GetOverlappingActors(OUT OverlappingActors);
	float mass = 0.f;
	for (const auto* actor : OverlappingActors)
	{
		mass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	} 

	return mass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Error, TEXT("%f"), GetMassOnPlate());
	if (pressurePlate && GetMassOnPlate() > TriggerMass) {
		OpenDoor(); 
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > doorCloseDelay) {
		CloseDoor();
	}
}

