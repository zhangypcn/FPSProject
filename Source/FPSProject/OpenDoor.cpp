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

	// �����ж� ͨ�����Actor/��������
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
	// ƽ���ŵ�λ��
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
	// ���ж������غϵ�����
	TArray<AActor*> OverLappingActors;
	TriggerArea->GetOverlappingActors(OverLappingActors);
	if (TriggerArea == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TriggerArea is NULL"));
		return -1.0f;
	}

	float TotalMass = 0.0f;//������������
	for (AActor * Actor : OverLappingActors)
	{
		// ��ȡ����
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}
