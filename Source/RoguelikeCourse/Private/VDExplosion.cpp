// Fill out your copyright notice in the Description page of Project Settings.


#include "VDExplosion.h"

#include "DrawDebugHelpers.h"
#include "VDAttributeComponent.h"

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
		// deal damage
		// UVDAttributeComponent* AttributeComponent = Cast<UVDAttributeComponent>(OtherActor->GetComponentByClass(UVDAttributeComponent::StaticClass()));
		UVDAttributeComponent* AttributeComponent = OtherActor->FindComponentByClass<UVDAttributeComponent>();
		if(AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(-50.0f);
		}
		// debug string
		FString CombinedString = FString::Printf(TEXT("Explosion hit %s at %s"), *GetNameSafe(OtherActor), *Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
	}	
}

