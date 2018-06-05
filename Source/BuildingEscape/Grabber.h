// Copyright Preshecan Muhunthan 2018

#pragma once

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Engine/EngineTypes.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//How far the player can reach to grab
private:
	float Reach = 120.f;
	AActor *targetActor;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	//ray-cast and grab waht's in reach
	void Grab();
	//Call when grab is released
	void Release();
	//Find attached Physics handle component
	void FindPhysicsHandleComponent();
	//Find attached Input Component
	void SetupInputComponent();
	//Return hit for first physics body in reaach
	const FHitResult GetFirstPhysicsBodyInReach();
	//returns current end of reach line
	FVector GetReachLineEnd();
	//returns current start pof reach line
	FVector GetReachLineStart();
};
