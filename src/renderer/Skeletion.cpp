// Skeleton.cpp

#include "renderer/Skeletion.h"

Skeleton::Skeleton()
{
    bones = {
    { "EFootS_1",    { NToeS_1, NHeel_1 } },
    { "EFoot_1",     { NToe_1, NHeel_1 } },
    { "EFootS_2",    { NHeel_2, NToeS_2 } },
    { "EToeC_1",     { NToe_1, NToeS_1 } },
    { "EFoot_2",     { NHeel_2, NToe_2 } },
    { "EHeel_2",     { NHeel_2, NAnkle_2 } },
    { "EToeS_1",     { NToeTip_1, NToeS_1 } },
    { "EHeep_1",     { NHeel_1, NAnkle_1 } },
    { "EInstepS_1",  { NToeS_1, NAnkle_1 } },
    { "EToe_1",      { NToeTip_1, NToe_1 } },
    { "EInstep_1",   { NToe_1, NAnkle_1 } },
    { "EToeC_2",     { NToe_2, NToeS_2 } },
    { "EInstepS_2",  { NAnkle_2, NToeS_2 } },
    { "EInstep_2",   { NToe_2, NAnkle_2 } },
    { "EToeS_2",     { NToeTip_2, NToeS_2 } },
    { "EToe_2",      { NToe_2, NToeTip_2 } },

    { "ECalf_1",     { NAnkle_1, NKnee_1 } },
    { "ECalf_2",     { NAnkle_2, NKnee_2 } },

    { "EThigh_1",    { NKnee_1, NHip_1 } },
    { "EThigh_2",    { NKnee_2, NHip_2 } },

    { "EGroin",      { NHip_1, NHip_2 } },

    { "Muscle120",    { NHip_2, NStomachS_2 } },
    { "EPelvisM",     { NStomach, NPivot } },
    { "Muscle117",    { NHip_1, NStomachS_1 } },
    { "Muscle119",    { NStomachS_2, NChestS_2 } },
    { "EStomach",     { NChest, NStomach } },

    { "EArm_1",      { NElbow_1, NShoulder_1 } },
    { "EForearm_1",  { NWrist_1, NElbow_1 } },
    { "Muscle116",    { NStomachS_1, NChestS_1 } },

    { "Edge20_2",    { MacroNode2_2, MacroNode3_2 } },
    { "Edge24_2",    { MacroNode4_2, MacroNode6_2 } },
    { "Edge22_2",    { MacroNode3_2, MacroNode4_2 } },
    { "Edge19_2",    { NWrist_2, MacroNode2_2 } },
    { "Edge25_2",    { MacroNode6_2, NFingertipsSS_2 } },
    { "Edge23_2",    { MacroNode4_2, NFingertips_2 } },
    { "Edge21_2",    { MacroNode3_2, NKnuckles_2 } },

    { "EForearm_2",  { NWrist_2, NElbow_2 } },
    { "Edge12_2",    { NFingertipsSS_2, NFingertips_2 } },
    { "EFingers_2",   { NFingertips_2, NKnuckles_2 } },
    { "EHand_2",      { NKnuckles_2, NWrist_2 } },
    { "Edge26_2",    { MacroNode5_2, NFingertipsSS_2 } },
    { "Edge7_2",     { NFingertipsS_2, NFingertips_2 } },
    { "EFingersS_2",  { NFingertips_2, NKnucklesS_2 } },
    { "Edge27_2",    { NFingertipsS_2, MacroNode5_2 } },
    { "EHandS_2",     { NKnucklesS_2, NWrist_2 } },
    { "EHandC_2",     { NKnuckles_2, NKnucklesS_2 } },
    { "Edge17_2",    { NWrist_2, MacroNode1_2 } },

    { "Edge20_1",    { MacroNode2_1, MacroNode3_1 } },
    { "EArm_2",      { NElbow_2, NShoulder_2 } },
    { "Edge19_1",    { NWrist_1, MacroNode2_1 } },
    { "Edge9_2",     { NFingertipsS_2, NKnucklesS_2 } },
    { "Edge16_2",    { NKnucklesS_2, MacroNode1_2 } },
    { "Edge22_1",    { MacroNode3_1, MacroNode4_1 } },
    { "Edge21_1",    { MacroNode3_1, NKnuckles_1 } },
    { "EHand_1",     { NKnuckles_1, NWrist_1 } },
    { "Edge24_1",    { MacroNode4_1, MacroNode6_1 } },

    { "Muscle118",    { NChestS_2, NShoulder_2 } },
    { "Edge23_1",    { MacroNode4_1, NFingertips_1 } },
    { "Edge25_1",    { MacroNode6_1, NFingertipsSS_1 } },
    { "EFingers_1",   { NFingertips_1, NKnuckles_1 } },
    { "Edge17_1",    { NWrist_1, MacroNode1_1 } },
    { "EHandS_1",     { NKnucklesS_1, NWrist_1 } },
    { "Edge12_1",    { NFingertipsSS_1, NFingertips_1 } },
    { "EHandC_1",     { NKnuckles_1, NKnucklesS_1 } },
    { "Edge7_1",     { NFingertipsS_1, NFingertips_1 } },
    { "Edge26_1",    { MacroNode5_1, NFingertipsSS_1 } },
    { "EFingersS_1",  { NFingertips_1, NKnucklesS_1 } },
    { "Edge27_1",    { NFingertipsS_1, MacroNode5_1 } },
    { "Muscle115",    { NChestS_1, NShoulder_1 } },
    { "Edge16_1",    { NKnucklesS_1, MacroNode1_1 } },
    { "Edge9_1",     { NFingertipsS_1, NKnucklesS_1 } },

    { "EClavicle_1", { NShoulder_1, NNeck } },
    { "EClavicle_2", { NShoulder_2, NNeck } },
    { "ENeck",       { NNeck, NHead } },
    { "EHead",       { NHead, NTop } }
};
}