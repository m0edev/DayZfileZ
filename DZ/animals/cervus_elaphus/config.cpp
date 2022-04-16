class CfgPatches
{
	class DZ_Animals_cervus_elaphus
	{
		units[]=
		{
			"Animal_CervusElaphus"
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
	class Animal_CervusElaphus: AnimalBase
	{
		simulation="dayzanimal";
		scope=2;
		model="\DZ\animals\cervus_elaphus\cervus_elaphus.p3d";
		displayName="$STR_CfgVehicles_Animal_CervusElaphus0";
		descriptionShort="$STR_CfgVehicles_Animal_CervusElaphus1";
		aiAgentTemplate="Herbivores_CervusElaphus";
		injuryLevels[]={1,0.5,0.2,0};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=400;
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
						hitpoints=200;
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
						hitpoints=200;
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
						hitpoints=250;
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
						hitpoints=350;
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
				item="DeerSteakMeat";
				count=17;
				itemZones[]=
				{
					"Zone_Chest",
					"Zone_Belly",
					"Zone_Pelvis"
				};
				countByZone[]={7,5,6};
				quantityMinMaxCoef[]={0.75,1};
			};
			class ObtainedGuts
			{
				item="Guts";
				count=3;
				quantityMinMaxCoef[]={0.5,0.80000001};
			};
			class ObtainedPelt
			{
				item="DeerPelt";
				count=1;
				itemZones[]=
				{
					"Zone_Chest",
					"Zone_Belly"
				};
				quantityCoef=1;
				transferToolDamageCoef=1;
			};
			class ObtainedLard
			{
				item="Lard";
				count=3;
				quantityMinMaxCoef[]={0.5,1};
			};
			class ObtainedBones
			{
				item="Bone";
				count=3;
				quantityMinMaxCoef[]={0.80000001,1};
				transferToolDamageCoef=1;
			};
		};
		class enfanimsys
		{
			meshObject="dz\animals\cervus_elaphus\Data\cervus_elaphus_skeleton.xob";
			graphname="dz\animals\animations\!graph_files\Herbivores\Herbivores_Graph.agr";
			defaultinstance="dz\animals\animations\!graph_files\Herbivores\DeerSkeleton_AnimInstance.asi";
			startnode="MasterDeers_SM";
			skeletonName="cervus_elaphus_skeleton.xob";
		};
		class AnimEvents
		{
			class Steps
			{
				class Walk1
				{
					soundLookupTable="HoofMediumWalk_LookupTable";
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
					soundLookupTable="HoofMediumWalk_LookupTable";
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
					soundLookupTable="HoofMediumWalk_LookupTable";
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
					soundLookupTable="HoofMediumWalk_LookupTable";
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
					soundLookupTable="HoofMediumRun_LookupTable";
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
					soundLookupTable="HoofMediumRun_LookupTable";
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
					soundLookupTable="HoofMediumRun_LookupTable";
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
					soundLookupTable="HoofMediumRun_LookupTable";
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
					soundLookupTable="HoofMediumGrazing_LookupTable";
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
					soundLookupTable="HoofMediumGrazing_LookupTable";
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
					soundLookupTable="HoofMediumBodyfall_LookupTable";
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
					soundLookupTable="HoofMediumSettle_LookupTable";
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
					soundLookupTable="HoofMediumRest2standA_LookupTable";
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
					soundLookupTable="HoofMediumRest2standB_LookupTable";
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
					soundLookupTable="HoofMediumStand2restA_LookupTable";
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
					soundLookupTable="HoofMediumStand2restB_LookupTable";
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
					soundLookupTable="HoofMediumStand2restC_LookupTable";
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
					soundLookupTable="HoofMediumRub1_LookupTable";
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
					soundLookupTable="HoofMediumRub2_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=19;
				};
			};
			class Sounds
			{
				class DeerBleat
				{
					soundSet="DeerBleat_SoundSet";
					noise="DeerRoarNoise";
					id=1;
				};
				class DoeBleat
				{
					soundSet="DoeBleat_SoundSet";
					noise="DeerRoarNoise";
					id=2;
				};
				class DeerBreath
				{
					soundSet="DeerBreath_SoundSet";
					noise="DeerRoarNoise";
					id=3;
				};
				class DeerBark
				{
					soundSet="DeerBark_SoundSet";
					noise="DeerRoarNoise";
					id=4;
				};
				class DoeBark
				{
					soundSet="DoeBark_SoundSet";
					noise="DeerRoarNoise";
					id=5;
				};
				class RoeBark
				{
					soundSet="RoeBark_SoundSet";
					noise="DeerRoarNoise";
					id=6;
				};
				class DeerCall
				{
					soundSet="DeerCall_SoundSet";
					noise="DeerRoarNoise";
					id=7;
				};
				class DeerCallFear
				{
					soundSet="DeerCallFear_SoundSet";
					noise="DeerRoarNoise";
					id=8;
				};
				class DeerGrunt
				{
					soundSet="DeerGrunt_SoundSet";
					noise="DeerRoarNoise";
					id=9;
				};
				class DeerRoar
				{
					soundSet="DeerRoar_SoundSet";
					noise="DeerRoarNoise";
					id=10;
				};
				class DeerRoarLong
				{
					soundSet="DeerRoarLong_SoundSet";
					noise="DeerRoarNoise";
					id=11;
				};
				class DeerRoarShort
				{
					soundSet="DeerRoarShort_SoundSet";
					noise="DeerRoarNoise";
					id=12;
				};
				class DeerSnort
				{
					soundSet="DeerSnort_SoundSet";
					noise="DeerRoarNoise";
					id=13;
				};
				class DeerRoarX1
				{
					soundSet="DeerRoarX1_SoundSet";
					noise="DeerRoarNoise";
					id=14;
				};
				class DeerRoarX1_Tail
				{
					soundSet="DeerRoarX1_Tail_SoundSet";
					noise="DeerRoarNoise";
					id=15;
				};
				class DeerRoarX2
				{
					soundSet="DeerRoarX2_SoundSet";
					noise="DeerRoarNoise";
					id=16;
				};
				class DeerRoarX2_Tail
				{
					soundSet="DeerRoarX2_Tail_SoundSet";
					noise="DeerRoarNoise";
					id=17;
				};
				class DeerRoarX3
				{
					soundSet="DeerRoarX3_SoundSet";
					noise="DeerRoarNoise";
					id=18;
				};
				class DeerRoarX3_Tail
				{
					soundSet="DeerRoarX3_Tail_SoundSet";
					noise="DeerRoarNoise";
					id=19;
				};
				class DeerRoarX4
				{
					soundSet="DeerRoarX4_SoundSet";
					noise="DeerRoarNoise";
					id=20;
				};
				class DeerRoarX4_Tail
				{
					soundSet="DeerRoarX4_Tail_SoundSet";
					noise="DeerRoarNoise";
					id=21;
				};
				class DeerRoarX5
				{
					soundSet="DeerRoarX5_SoundSet";
					noise="DeerRoarNoise";
					id=22;
				};
				class DeerRoarX5_Tail
				{
					soundSet="DeerRoarX5_Tail_SoundSet";
					noise="DeerRoarNoise";
					id=23;
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
		class CommandMoveSettings
		{
			useSpeedMapping=1;
			movementSpeedMapping[]={0,0.25,0.80000001,3.2,6,17};
		};
		class CommandLookAtSettings
		{
			lookAtFilterTimeout=0.5;
			lookAtFilterSpeed=1.5700001;
		};
	};
};
