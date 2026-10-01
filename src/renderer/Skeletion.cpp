// Skeleton.cpp

#include "renderer/Skeletion.h"

Skeleton::Skeleton()
{
    bones = {

        // Head
        { NHead, NTop },
        { NNeck, NHead },

        // Body
        { NStomach, NHip_1 },
        { NStomach, NHip_2 },
        { NHip_1, NHip_2 },

        { NChest, NStomach },
        { NNeck, NChest },

        // Left/right legs
        { NKnee_2, NHip_2 },
        { NKnee_1, NHip_1 },

        { NAnkle_2, NKnee_2 },
        { NAnkle_1, NKnee_1 },

        { NToe_2, NAnkle_2 },
        { NToe_1, NAnkle_1 },

        // Arms
        { NElbow_2, NShoulder_2 },
        { NWrist_2, NElbow_2 },

        { NElbow_1, NShoulder_1 },
        { NWrist_1, NElbow_1 },

        // Hands
        { NKnuckles_2, NWrist_2 },
        { NFingertips_2, NKnuckles_2 },

        { NKnuckles_1, NWrist_1 },
        { NFingertips_1, NKnuckles_1 },

        // Feet
        { NHeel_2, NAnkle_2 },
        { NHeel_2, NToe_2 },
        { NToe_2, NToeTip_2 },

        { NHeel_1, NAnkle_1 },
        { NToe_1, NHeel_1 },
        { NToeTip_1, NToe_1 }
    };
}