// Fill out your copyright notice in the Description page of Project Settings.


#include "VDExplosion.h"

// Sets default values
AVDExplosion::AVDExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName("PhysicsActor");
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AVDExplosion::OnComponentHit);
	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->Radius = 500;
	RadialForceComponent->ImpulseStrength = 1000;
	RadialForceComponent->bImpulseVelChange = 1;
	RadialForceComponent->bIgnoreOwningActor = 0;
	RadialForceComponent->SetAutoActivate(false);
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AVDExplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVDExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVDExplosion::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
		RadialForceComponent->FireImpulse();
	}	
}

