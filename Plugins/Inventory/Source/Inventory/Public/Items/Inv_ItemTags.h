#pragma once

#include "NativeGameplayTags.h"

// #include "Inv_ItemTags.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Axe);

namespace GameItems
{
    namespace Equipment
    {
        namespace Weapons
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Axe);
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sword);

        } // namespace Weapons

        namespace Cloaks
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(RedCloak);

        }

        namespace Masks
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(SteelMask);

        }
    } // namespace Equipment

    namespace Consumables
    {
        namespace Potions
        {
            namespace Red
            {
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small);
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large);
            } // namespace Red
            namespace Blue
            {
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small);
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large);
            } // namespace Blue
        } // namespace Potions
    } // namespace Consumables

    namespace Craftables
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireFernFruit);
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(LuminDaisy);
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(ScorchPetalBlossom);
    } // namespace Craftables
} // namespace GameItems