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

void UGrabber::GetPhysicsHandleComponent() {
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle) {

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s: PhysicsHandle not found"), *GetOwner()->GetName());
	}
}

void UGrabber::GetInputController() {

	//Look for input controler
	InputController = GetOwner()->InputComponent;

	if (InputController) {
		UE_LOG(LogTemp, Warning, TEXT("%s: InputController found"), *GetOwner()->GetName());
		//Bind the input axis
		InputController->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputController->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s: InputController not found"), *GetOwner()->GetName());
	}
}


void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		GetFirstPhysicsBodyInReach();
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // to allow rotation
		);
	}
	
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FVector PlayerViewPortLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPortLocation,
		OUT PlayerViewPointRotation
		);

	FVector LineTraceEnd = PlayerViewPortLocation + PlayerViewPointRotation.Vector() * Reach;

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

	FVector PlayerViewPortLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPortLocation,
		OUT PlayerViewPointRotation
		);

	FVector LineTraceEnd = PlayerViewPortLocation + PlayerViewPointRotation.Vector() * Reach;


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

	//Checking which element Actor is pointing

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(

		OUT Hit,
		PlayerViewPortLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
		);

	AActor* HitObject = Hit.GetActor();

	if (HitObject) {
		UE_LOG(LogTemp, Warning, TEXT("Actor is pointing on: %s"), *HitObject->GetName());
	}

	return Hit;
}

