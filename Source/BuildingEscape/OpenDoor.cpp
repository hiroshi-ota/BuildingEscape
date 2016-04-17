// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


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

	whichActorOpenes = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();

	// find the owning object
	
}

void UOpenDoor::OpenDoor() {

	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
	Owner->SetActorRotation(NewRotation);

}

void UOpenDoor::CloseDoor() {

	FRotator NewRotation = FRotator(0.0f, 0.0f, 0.0f);
	Owner->SetActorRotation(NewRotation);

}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PressurePlate->IsOverlappingActor(whichActorOpenes)) { 
		
		OpenDoor(); 
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	if (GetWorld()->GetTimeSeconds() > lastDoorOpenTime + doorCloseDelay) {

		CloseDoor();
	}
	
}

