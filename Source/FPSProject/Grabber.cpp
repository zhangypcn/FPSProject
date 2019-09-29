// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	SetPhysicsHandleComponent();
	SetInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->SetTargetLocation(GetLineEndPosition());
	}
}

// ��ȡ�������λ��
FVector UGrabber::GetLineStartPosition()
{
	//�������λ�úͷ���
	FVector PlayerViewPointLocation;//��ɫλ��
	FRotator playViewPointRotation;//�泯����
	//��ȡ��ɫ��λ�ú���ת����
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, playViewPointRotation);
	return PlayerViewPointLocation;
}

// ��ȡ�����յ�λ��
FVector UGrabber::GetLineEndPosition()
{
	//�������λ�úͷ���
	FVector PlayerViewPointLocation;//��ɫλ��
	FRotator playViewPointRotation;//�泯����
	//��ȡ��ɫ��λ�ú���ת����
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, playViewPointRotation);
	//�����յ�λ�ã��������������ϵ��
	FVector LineTraceEndLocation = PlayerViewPointLocation + playViewPointRotation.Vector() * Reach;
	return LineTraceEndLocation;
}

// ��ȡ������ײ�ĵ�һ��������Ϣ
FHitResult UGrabber::GetFirstPhysicsBobyInReach()
{
	//��������
	//DrawDebugLine(
	//	GetWorld(),
	//	GetLineStartPosition(),
	//	GetLineEndPosition(),
	//	FColor(255, 0, 0),
	//	false, 0.0f, 0.0f, 10.0f
	//);

	//��ײ����
	FHitResult HitResult;
	FCollisionQueryParams QueryParams = FCollisionQueryParams("", false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		GetLineStartPosition(),
		GetLineEndPosition(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParams
	);

	AActor *ActorHit = HitResult.GetActor();
	if (ActorHit != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Name: %s"), *ActorHit->GetName());
	}
	return HitResult;
}

// ��ȡ����������
void UGrabber::SetPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle HOT FOUND!"));
		return;
	}
}

// ��ȡ�������
void UGrabber::SetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent HOT FOUND!"));
		return;
	}
	else
	{
		// ������
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grad);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}

}

void UGrabber::Grad()
{
	/* ���������Ƿ���� */
	//AActor *ActorHit = GetFirstPhysicsBobyInReach().GetActor();
	//if (ActorHit == nullptr)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ActorHit is NULL!"));
	//	return;
	//}
	//UE_LOG(LogTemp, Warning, TEXT("GAME NAME: %s"), *ActorHit->GetName());

	FHitResult HitResut = GetFirstPhysicsBobyInReach();
	UPrimitiveComponent *ComponentToGrab = HitResut.GetComponent();
	if (HitResut.GetActor() != nullptr)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			HitResut.GetActor()->GetActorLocation(),
			HitResut.GetActor()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

