// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Utils/Inv_WidgetUtils.h"

#include "Items/Components/Inv_ItemComponent.h"
#include "Math/MathFwd.h"

int32 UInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
    return Position.X + Position.Y * Columns;
}

FIntPoint UInv_WidgetUtils::GetPositionFromIndex(const int32 Index, const int32 Columns)
{
    if (Columns == 0) return FIntPoint(0, 0);
    return FIntPoint(Index % Columns, Index / Columns);
}

