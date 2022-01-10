// Fill out your copyright notice in the Description page of Project Settings.


#include "VDTargetDummy.h"
#include "VDAttributeComponent.h"

// Sets default values
AVDTargetDummy::AVDTargetDummy()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;

	AttributeComponent = CreateDefaultSubobject<UVDAttributeComponent>("AttributeComponent");
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AVDTargetDummy::OnHealthChanged);
	
}

void AVDTargetDummy::OnHealthChanged(AActor* InstigatorActor, UVDAttributeComponent* OwningComponent, float NewHealth,
	float DeltaHealth)
{
	if (DeltaHealth < 0.0f)
	{
		MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
		
}

