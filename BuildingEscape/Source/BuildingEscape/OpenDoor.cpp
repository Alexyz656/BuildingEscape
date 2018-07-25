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
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();	
	if (!pressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("THIS DOESNT WORK"), );
	}
}

float UOpenDoor::GetMassOnPlate()
{
	if (!pressurePlate) {
		return 0.f;
	}
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


	if (GetMassOnPlate() > TriggerMass) {
		OnOpenRequest.Broadcast();
	}else{
		OnCloseRequest.Broadcast();
	}
}

