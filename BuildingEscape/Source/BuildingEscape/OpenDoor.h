// Copyright Hernan Medina 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OpenDoor();
	virtual void CloseDoor();
	float TriggerMass = 30.f;

private:
	UPROPERTY(VisibleAnywhere)	
		float openAngle = 0.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* pressurePlate;

	UPROPERTY(EditAnywhere)
		float doorCloseDelay=1;

	float LastDoorOpenTime;
	AActor* Owner;	
	float GetMassOnPlate();
};

