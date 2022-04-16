class CfgPatches
{
	class DZ_Animals_sus_scrofa
	{
		units[]=
		{
			"Animal_SusScrofa"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Animals"
		};
	};
};
class CfgVehicles
{
	class AnimalBase;
	class Animal_SusScrofa: AnimalBase
	{
		simulation="dayzanimal";
		scope=2;
		model="\DZ\animals\sus_scrofa\sus_scrofa.p3d";
		displayName="$STR_CfgVehicles_Animal_SusScrofa0";
		descriptionShort="$STR_CfgVehicles_Animal_SusScrofa1";
		aiAgentTemplate="Herbivores_SusScrofa";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=350;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
				class Blood
				{
					hitpoints=5000;
				};
				class Shock
				{
					hitpoints=100;
				};
			};
			class DamageZones
			{
				class Zone_Head
				{
					componentNames[]=
					{
						"Zone_Head"
					};
					transferToZonesNames[]={};
					transferToZonesCoefs[]={};
					fatalInjuryCoef=0.15000001;
					canBleed=0;
					class Health
					{
						hitpoints=250;
						transferToGlobalCoef=1;
					};
					class Blood: Health
					{
						hitpoints=0;
					};
					class Shock: Health
					{
						hitpoints=0;
					};
				};
				class Zone_Neck: Zone_Head
				{
					componentNames[]=
					{
						"Zone_Neck"
					};
					transferToZonesNames[]=
					{
						"Zone_Chest"
					};
					transferToZonesCoefs[]={0.2};
					fatalInjuryCoef=0.050000001;
					class Health: Health
					{
						hitpoints=250;
					};
				};
				class Zone_Chest: Zone_Head
				{
					componentNames[]=
					{
						"Zone_Chest"
					};
					transferToZonesNames[]=
					{
						"Zone_Neck",
						"Zone_Belly"
					};
					transferToZonesCoefs[]={0.15000001,0.15000001};
					fatalInjuryCoef=0.050000001;
					class Health: Health
					{
						hitpoints=300;
					};
				};
				class Zone_Belly: Zone_Head
				{
					componentNames[]=
					{
						"Zone_Belly"
					};
					transferToZonesNames[]=
					{
						"Zone_Pelvis",
						"Zone_Chest"
					};
					transferToZonesCoefs[]={0.15000001,0.15000001};
					fatalInjuryCoef=0.050000001;
					class Health: Health
					{
						hitpoints=300;
					};
				};
				class Zone_Spine: Zone_Head
				{
					componentNames[]=
					{
						"Zone_Spine"
					};
					transferToZonesNames[]=
					{
						"Zone_Pelvis",
						"Zone_Belly",
						"Zone_Chest"
					};
					transferToZonesCoefs[]={0.1,0.1,0.1};
					fatalInjuryCoef=0.050000001;
					class Health: Health
					{
						hitpoints=300;
					};
				};
				class Zone_Pelvis: Zone_Head
				{
					componentNames[]=
					{
						"Zone_Pelvis"
					};
					transferToZonesNames[]=
					{
						"Zone_Spine",
						"Zone_Belly"
					};
					transferToZonesCoefs[]={0.15000001,0.15000001};
					fatalInjuryCoef=0.050000001;
					class Health: Health
					{
						hitpoints=300;
					};
				};
				class Zone_Legs: Zone_Head
				{
					componentNames[]=
					{
						"Zone_Legs"
					};
					transferToZonesNames[]=
					{
						"Zone_Pelvis",
						"Zone_Belly",
						"Zone_Chest"
					};
					transferToZonesCoefs[]={0.1,0.1,0.1};
					fatalInjuryCoef=0;
					class Health: Health
					{
						hitpoints=100;
					};
				};
			};
		};
		class Skinning
		{
			class ObtainedSteaks
			{
				item="PigSteakMeat";
				count=13;
				itemZones[]=
				{
					"Zone_Chest",
					"Zone_Belly",
					"Zone_Pelvis"
				};
				countByZone[]={5,3,5};
				quantityMinMaxCoef[]={0.5,1};
			};
			class ObtainedPelt
			{
				item="WildboarPelt";
				count=1;
				itemZones[]=
				{
					"Zone_Chest",
					"Zone_Belly"
				};
				quantityCoef=1;
				transferToolDamageCoef=1;
			};
			class ObtainedGuts
			{
				item="Guts";
				count=2;
				quantityMinMaxCoef[]={0.5,0.80000001};
			};
			class ObtainedLard
			{
				item="Lard";
				count=1;
				quantityMinMaxCoef[]={0.5,0.80000001};
			};
			class ObtainedBones
			{
				item="Bone";
				count=2;
				quantityMinMaxCoef[]={0.80000001,1};
				transferToolDamageCoef=1;
			};
		};
		class enfanimsys
		{
			meshObject="dz\animals\sus_scrofa\data\sus_scrofa_skeleton.xob";
			graphname="dz\animals\animations\!graph_files\Herbivores\Herbivores_Graph.agr";
			defaultinstance="dz\animals\animations\!graph_files\Herbivores\WildBoar_AnimInstance.asi";
			startnode="Master_BoarAndPig_SM";
			skeletonName="sus_scrofa_skeleton.xob";
		};
		class CommandMoveSettings
		{
			useSpeedMapping=1;
			movementSpeedMapping[]={0,0.25,0.80000001,2.51,6,17};
		};
		class CommandLookAtSettings
		{
			lookAtFilterTimeout=0.5;
			lookAtFilterSpeed=1.5700001;
		};
		class AnimEvents
		{
			class Steps
			{
				class Walk1
				{
					soundLookupTable="HoofSmallWalk_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=1;
				};
				class Walk2
				{
					soundLookupTable="HoofSmallWalk_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=2;
				};
				class Walk3
				{
					soundLookupTable="HoofSmallWalk_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=3;
				};
				class Walk4
				{
					soundLookupTable="HoofSmallWalk_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=4;
				};
				class Run1
				{
					soundLookupTable="HoofSmallRun_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=5;
				};
				class Run2
				{
					soundLookupTable="HoofSmallRun_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=6;
				};
				class Run3
				{
					soundLookupTable="HoofSmallRun_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=7;
				};
				class Run4
				{
					soundLookupTable="HoofSmallRun_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=8;
				};
				class Grazing1
				{
					soundLookupTable="HoofSmallGrazing_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=9;
				};
				class Grazing2
				{
					soundLookupTable="HoofSmallGrazing_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=10;
				};
				class Bodyfall
				{
					soundLookupTable="HoofSmallBodyfall_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=11;
				};
				class Settle
				{
					soundLookupTable="HoofSmallSettle_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=12;
				};
				class Rest2standA
				{
					soundLookupTable="HoofSmallRest2standA_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=13;
				};
				class Rest2standB
				{
					soundLookupTable="HoofSmallRest2standB_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=14;
				};
				class Stand2restA
				{
					soundLookupTable="HoofSmallStand2restA_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=15;
				};
				class Stand2restB
				{
					soundLookupTable="HoofSmallStand2restB_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=16;
				};
				class Stand2restC
				{
					soundLookupTable="HoofSmallStand2restC_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=17;
				};
				class Rub1
				{
					soundLookupTable="HoofSmallRub1_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=18;
				};
				class Rub2
				{
					soundLookupTable="HoofSmallRub2_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=19;
				};
				class GrazingHard1
				{
					soundLookupTable="HoofSmallGrazingHard_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=20;
				};
				class GrazingHard2
				{
					soundLookupTable="HoofSmallGrazingHard_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=21;
				};
				class GrazingLeave1
				{
					soundLookupTable="HoofSmallGrazingLeave_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=22;
				};
				class GrazingLeave2
				{
					soundLookupTable="HoofSmallGrazingLeave_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=23;
				};
			};
			class Sounds
			{
				class HogGrunt_A
				{
					soundSet="HogGrunt_A_SoundSet";
					noise="DeerRoarNoise";
					id=1;
				};
				class HogGrunt_A_Squeal
				{
					soundSet="HogGrunt_A_Squeal_SoundSet";
					noise="DeerRoarNoise";
					id=2;
				};
				class HogGrunt_B
				{
					soundSet="HogGrunt_B_SoundSet";
					noise="DeerRoarNoise";
					id=3;
				};
				class HogGrunt_B_Long
				{
					soundSet="HogGrunt_B_Long_SoundSet";
					noise="DeerRoarNoise";
					id=4;
				};
				class HogGrunt_C
				{
					soundSet="HogGrunt_C_SoundSet";
					noise="DeerRoarNoise";
					id=5;
				};
				class HogGrunt_D
				{
					soundSet="HogGrunt_D_SoundSet";
					noise="DeerRoarNoise";
					id=6;
				};
				class HogGrunt_E
				{
					soundSet="HogGrunt_E_SoundSet";
					noise="DeerRoarNoise";
					id=7;
				};
				class HogGrunt_F
				{
					soundSet="HogGrunt_F_SoundSet";
					noise="DeerRoarNoise";
					id=8;
				};
				class HogGrunt_G
				{
					soundSet="HogGrunt_G_SoundSet";
					noise="DeerRoarNoise";
					id=9;
				};
				class HogGrunt_H
				{
					soundSet="HogGrunt_H_SoundSet";
					noise="DeerRoarNoise";
					id=10;
				};
				class HogGrunt_I
				{
					soundSet="HogGrunt_I_SoundSet";
					noise="DeerRoarNoise";
					id=11;
				};
				class HogSniff
				{
					soundSet="HogSniff_SoundSet";
					noise="DeerRoarNoise";
					id=12;
				};
				class HogSqueal
				{
					soundSet="HogSqueal_SoundSet";
					noise="DeerRoarNoise";
					id=13;
				};
				class HogScratch3
				{
					soundSet="HogSscratch3_SoundSet";
					noise="DeerRoarNoise";
					id=14;
				};
				class HogScratch4
				{
					soundSet="HogSscratch4_SoundSet";
					noise="DeerRoarNoise";
					id=15;
				};
				class HogClaw
				{
					soundSet="HogClaw_SoundSet";
					noise="DeerRoarNoise";
					id=16;
				};
				class HogGrowl1
				{
					soundSet="HogGrowl1_SoundSet";
					noise="DeerRoarNoise";
					id=17;
				};
				class HogGrowl2
				{
					soundSet="HogGrowl2_SoundSet";
					noise="DeerRoarNoise";
					id=18;
				};
				class HogGrowl3
				{
					soundSet="HogGrowl3_SoundSet";
					noise="DeerRoarNoise";
					id=19;
				};
				class HogGrowl4
				{
					soundSet="HogGrowl4_SoundSet";
					noise="DeerRoarNoise";
					id=20;
				};
				class HogGrowl5
				{
					soundSet="HogGrowl5_SoundSet";
					noise="DeerRoarNoise";
					id=21;
				};
			};
			class Damages
			{
				class Bite
				{
					damage="DeerBiteDamage";
					id=1;
				};
			};
		};
	};
};
