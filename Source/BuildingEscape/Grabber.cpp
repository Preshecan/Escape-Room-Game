// Copyright Preshecan Muhunthan 2018

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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
	SetupInputComponent();
}


void UGrabber::FindPhysicsHandleComponent()
{
	///Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Physics handle component missing from %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Input Component missing from %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab()
{
	//Line trace and see if we reach any actors with a physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto targetActor = HitResult.GetActor();

	if (!PhysicsHandle) {return;}
	//if we hit something then attach a physics handle
	if (targetActor) {
		//attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			targetActor->GetActorLocation(),
			GetOwner()->GetActorRotation()
		);
	}
	

	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"));
}

void UGrabber::Release()
{
	if (!PhysicsHandle) {return;}
	//TODO detach physics handle
	PhysicsHandle->ReleaseComponent();

	UE_LOG(LogTemp, Warning, TEXT("grab released"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) {return;}
	//if Physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	//move object that is held

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//draws a red trace in the world to visualise grabber
	DrawDebugLine(
		GetWorld(),
		GetReachLineStart(),
		GetReachLineEnd(),
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	//ray-cast/line trace out to reach distance
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	targetActor = Hit.GetActor();

	if (targetActor) {
		UE_LOG(LogTemp, Warning, TEXT("Target is: %s"), *(targetActor->GetName()));
	}
	return Hit;
}

FVector UGrabber::GetReachLineEnd()
{
	//get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,   //changed by fuction and returns result
		PlayerViewPointRotation	//changed by fuction and returns result
	);

	/*UE_LOG(LogTemp, Warning, TEXT("location is: %s , and rotation is: %s"),
	*PlayerViewPointLocation.ToString(),
	*PlayerViewPointRotation.ToString()
	);*/

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	return LineTraceEnd;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation, 
		PlayerViewPointRotation	
	);
	return PlayerViewPointLocation;
}
