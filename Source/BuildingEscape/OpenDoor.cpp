// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	// find the owning object
	
}

void UOpenDoor::OpenDoor() {

	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
	//Owner->SetActorRotation(NewRotation);
	OnOpenRequest.Broadcast();

}

void UOpenDoor::CloseDoor() {

	FRotator NewRotation = FRotator(0.0f, 0.0f, 0.0f);
	//Owner->SetActorRotation(NewRotation);
	OnCloseRequest.Broadcast();

}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	if (GetTotalMassOfActorsOnPlate() > 30.f) { //TODO: mass to variable
		OpenDoor(); 
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	if (GetWorld()->GetTimeSeconds() > lastDoorOpenTime + doorCloseDelay) {

		CloseDoor();
	}
	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {

	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}


	return TotalMass;
}
