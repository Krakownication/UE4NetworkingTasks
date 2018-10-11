// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/MyPlayerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AMyPlayerHUD::AMyPlayerHUD()
{
	auto CrosshairTexAsset = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	if (CrosshairTexAsset.Succeeded())
	{
		CrosshairTex = CrosshairTexAsset.Object;
	}

	auto FontAsset = ConstructorHelpers::FObjectFinder<UFont>(TEXT("/Engine/EngineFonts/Roboto"));
	if (FontAsset.Succeeded())
	{
		SelectedFont = FontAsset.Object;
	}
}

void AMyPlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTex != nullptr)
	{
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		const FVector2D CrosshairDrawPosition((Center.X), (Center.Y + 20.0f));

		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}

	if (SelectedFont != nullptr)
	{
		TArray<FText> Messages = {
		   FText::FromString("Controls: WSAD + Mouse"),
		   FText::FromString("To spawn Light press R (allowed once)"),
		   FText::FromString("To toogle Light press T (after spawning)"),
		   FText::FromString("To spawn Bomb press Space")
		};

		FLinearColor TheFontColour = FLinearColor(1.0f, 1.0f, 1.0f);

		for (int32 Index = 0; Index != Messages.Num(); ++Index)
		{
			FCanvasTextItem NewText(FVector2D(10, 10 + (Index * 20)), Messages[Index], SelectedFont, TheFontColour);
			Canvas->DrawItem(NewText);
		}
	}
}



