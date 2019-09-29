// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProjectile.h"
#include "FPSProject.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ʹ������������ײ
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// �����������ײ�뾶
	CollisionComponent->InitSphereRadius(15.0f);
	// ����ײ�����Ϊ�����
	RootComponent = CollisionComponent;

	// ʹ�ô���������÷�������˶�
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;			//�������ʼ�ٶ�
	ProjectileMovementComponent->MaxSpeed = 3000.0f;				//����������ٶ�
	ProjectileMovementComponent->bRotationFollowsVelocity = true;	//�������Ƿ���ת
	ProjectileMovementComponent->bShouldBounce = true;				//�������Ƿ񷴵�
	ProjectileMovementComponent->Bounciness = 0.3f;					//�����Ϸ�����Ļָ�ϵ��

	// ��������3�������
	InitialLifeSpan = 3.0f;//��ʼ����

	// �����µ���ײͨ��
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	// �����ײ���
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);

	// �����ɼ����ֹͣ������ϵͳ
	FireParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticlesFireComponent"));
	FireParticleSystem->AttachTo(CollisionComponent);//���ӵ�������ײ���
	FireParticleSystem->bAutoActivate = true;//Ĭ�ϲ������Ӷ���
	//FireParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/Particles/P_Fire.P_Fire"));//��̬������Դ
	if (ParticleAsset.Succeeded())//������Դ���سɹ�
	{
		FireParticleSystem->SetTemplate(ParticleAsset.Object);//���������Դ
	}
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		// ��ʾ������Ϣ���롣-1������ֵ���׸�������˵������������»�ˢ�´���Ϣ��
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are firing bullets."));
	}
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// �ڷ��䷽�������÷�������ٶȵĺ���
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	// ֻ���ṩһ�����䷽����Ϊ��������ٶ��� ProjectileMovementComponent ���塣
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// ��������������ʱ���õĺ���
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		// ��ָ��λ�ø���������һ��������
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
}