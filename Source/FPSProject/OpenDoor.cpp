// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

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

	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TriggerArea == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TriggerArea is NULL"));
		return;
	}
	if (ActorThatOpen == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ActorThatOpen is NULL"));
		return;
	}

	// 两种判定 通过检测Actor/重量开门
	if (TriggerArea->IsOverlappingActor(ActorThatOpen) || GetTotalMassOfActors() > MassToOpen)
	{
		// OPEN
		UE_LOG(LogTemp, Warning, TEXT("Open the Door!"));
		OpenDoor();
	}
	else
	{
		// CLOSE
		CloseDoor();
	}

}

void UOpenDoor::OpenDoor() 
{
	// 平移门的位置
	//FVector DoorLocation = DoorLocation + Offset;
	//AActor *Owner = GetOwner();
	//Owner->SetActorLocation(DoorLocation);

	OnOpenDoorRequst.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	//FVector DoorLocation = DoorLocation;
	//AActor *Owner = GetOwner();
	//Owner->SetActorLocation(DoorLocation);

	OnCloseDoorRequst.Broadcast();
}

float UOpenDoor::GetTotalMassOfActors()
{
	// 与判定区域重合的重量
	TArray<AActor*> OverLappingActors;
	TriggerArea->GetOverlappingActors(OverLappingActors);
	if (TriggerArea == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TriggerArea is NULL"));
		return -1.0f;
	}

	float TotalMass = 0.0f;//区域内总重量
	for (AActor * Actor : OverLappingActors)
	{
		// 获取重量
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}
