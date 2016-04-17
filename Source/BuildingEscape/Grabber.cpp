// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	GetPhysicsHandleComponent();
	GetInputController();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetActorReachEnd());
	}
}

void UGrabber::GetPhysicsHandleComponent() {
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s: PhysicsHandle not found"), *GetOwner()->GetName());
	}
}

//Look for input controler
void UGrabber::GetInputController() {
	InputController = GetOwner()->InputComponent;

	if (InputController) {
		///Bind the input axis
		InputController->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputController->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s: InputController not found"), *GetOwner()->GetName());
	}
}

//Get position of Actor's grab distance
FVector UGrabber::GetActorReachEnd() {

	FVector PlayerViewPortLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPortLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPortLocation + PlayerViewPointRotation.Vector() * Reach;
}

//Get actor's centre / viewport location
FVector UGrabber::PlayerViewPortLocation() {

	FVector PlayerViewPortLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPortLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPortLocation;
}

//Grab element if it is in reach range
void UGrabber::Grab() {

	///LINE TRACE and see if we reach any actors with physics body collision channel
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, ///no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true /// to allow rotation
		);
	}
	
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

	PhysicsHandle->ReleaseComponent();
}

//Get first element in front of us in our reach 
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(

		OUT HitResult,
		PlayerViewPortLocation(),
		GetActorReachEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s || Rotation: %s"), 
	//	*PlayerViewPortLocation.ToString(),
	//	*PlayerViewPointRotation.ToString()
	//)

	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPortLocation,
	//	LineTraceEnd,
	//	FColor(255, 0, 0),
	//	false,
	//	0.f,
	//	0.f,
	//	5.f
	//);

	return HitResult;
}

