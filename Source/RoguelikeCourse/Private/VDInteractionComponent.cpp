// Fill out your copyright notice in the Description page of Project Settings.


#include "VDInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "VDGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("vd.InteractionDebugDraw"), false,
	TEXT("Enable Debug lines for Interact Component"));

// Sets default values for this component's properties
UVDInteractionComponent::UVDInteractionComponent()
{
}

void UVDInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	// FHitResult Hit;
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	FCollisionShape Shape;
	float Radius = 30.0f;
	Shape.SetSphere(Radius);
	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult Hit : Hits)
	{
		if(bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		}
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<UVDGameplayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);
			IVDGameplayInterface::Execute_Interact(HitActor, MyPawn);
			break;
		}
	}
	if(bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0 , 2.0f);
	}
}