// Copyright Preshecan Muhunthan 2018

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"


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
	if (!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("PressurePlate missing from %s"), *(GetOwner()->GetName()));
	}
	//to test try: ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

void UOpenDoor::OpenDoor()
{
	//FString ownerRot = Owner->GetActorRotation().ToString();
	//FRotator newRotation = FRotator(0.0f, OpenAngle, 0.0f);
	///Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	


}

void UOpenDoor::CloseDoor()
{
	
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//CurrentTime = GetWorld()->GetTimeSeconds();
	///poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {   //to test try: PressurePlate->IsOverlappingActor(ActorThatOpens)
		OnOpen.Broadcast();
	}else{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	if (!PressurePlate) {return TotalMass;}

	TArray<AActor *> OverlappingActors;

	//find all the overlapping actors
	PressurePlate->GetOverlappingActors(OverlappingActors);
	//iterate through them and find the total mass
	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate"), *Actor->GetName());
	}


	return TotalMass;
}
