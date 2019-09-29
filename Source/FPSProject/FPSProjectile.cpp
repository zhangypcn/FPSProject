// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProjectile.h"
#include "FPSProject.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 使用球体代表简单碰撞
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// 设置球体的碰撞半径
	CollisionComponent->InitSphereRadius(15.0f);
	// 将碰撞组件设为根组件
	RootComponent = CollisionComponent;

	// 使用此组件驱动该发射物的运动
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;			//发射物初始速度
	ProjectileMovementComponent->MaxSpeed = 3000.0f;				//发射物最大速度
	ProjectileMovementComponent->bRotationFollowsVelocity = true;	//发射物是否旋转
	ProjectileMovementComponent->bShouldBounce = true;				//发射物是否反弹
	ProjectileMovementComponent->Bounciness = 0.3f;					//法线上反弹后的恢复系数

	// 发射物于3秒后消亡
	InitialLifeSpan = 3.0f;//初始寿命

	// 设置新的碰撞通道
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	// 添加碰撞组件
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);

	// 创建可激活或停止的粒子系统
	FireParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticlesFireComponent"));
	FireParticleSystem->AttachTo(CollisionComponent);//附加到球体碰撞组件
	FireParticleSystem->bAutoActivate = true;//默认播放粒子动画
	//FireParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/Particles/P_Fire.P_Fire"));//静态加载资源
	if (ParticleAsset.Succeeded())//粒子资源加载成功
	{
		FireParticleSystem->SetTemplate(ParticleAsset.Object);//粒子填充资源
	}
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		// 显示调试信息五秒。-1“键”值（首个参数）说明我们无需更新或刷新此消息。
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are firing bullets."));
	}
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 在发射方向上设置发射物初速度的函数
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	// 只需提供一个发射方向，因为发射物的速度由 ProjectileMovementComponent 定义。
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// 发射物命中物体时调用的函数
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		// 在指定位置给刚体增加一个冲量。
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
}