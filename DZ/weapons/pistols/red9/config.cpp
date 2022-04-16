class CfgPatches
{
	class DZ_Pistols_Red9
	{
		units[]=
		{
			"Red9"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Pistols"
		};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;
class cfgWeapons
{
	class Pistol_Base;
	class Red9_Base: Pistol_Base
	{
		scope=0;
		debug_ItemCategory=1;
		weight=960;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		modelOptics="-";
		weaponInfoType="RscWeaponZeroing";
		distanceZoomMin=100;
		distanceZoomMax=100;
		discreteDistance[]={50,100,200,300};
		discreteDistanceInitIndex=1;
		value=0;
		chamberSize=10;
		chamberedRound="";
		magazines[]={};
		chamberableFrom[]=
		{
			"Ammo_9x19",
			"Mag_CLIP9x19_10Rnd"
		};
		damagePerShot[]={0.050000001,0.050000001};
		ejectType=1;
		recoilModifier[]={1,1,1};
		drySound[]=
		{
			"DZ\sounds\weapons\firearms\red9\Red9_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"DZ\sounds\weapons\firearms\red9\Red9_reload",
			1,
			1,
			30
		};
		reloadAction="ReloadSks";
		hiddenSelections[]=
		{
			"camo"
		};
		modes[]=
		{
			"SemiAuto"
		};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[]=
			{
				"Red9_Shot_SoundSet",
				"Red9_Tail_SoundSet",
				"Red9_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"Red9_silencerHomeMade_SoundSet",
					"Red9_silencerHomeMadeTail_SoundSet",
					"Red9_silencerInteriorHomeMadeTail_SoundSet"
				}
			};
			reloadTime=0.18000001;
			recoil="recoil_red9";
			recoilProne="recoil_red9_prone";
			dispersion=0.0089999996;
			magazineSlot="magazine";
			begin1[]=
			{
				"dz\sounds\weapons\firearms\red9\mauser_0",
				1,
				1,
				600
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\red9\mauser_1",
				1,
				1,
				600
			};
			soundBegin[]=
			{
				"begin1",
				0.33333001,
				"begin2",
				0.33333001,
				"begin1",
				0.33333001,
				"begin2",
				0.33333001
			};
			beginSilenced_Pro1[]=
			{
				"dz\sounds\weapons\firearms\CZ75\cz75Silenced",
				1,
				1,
				60
			};
			beginSilenced_Pro2[]=
			{
				"dz\sounds\weapons\firearms\CZ75\cz75Silenced2",
				1,
				1,
				60
			};
			beginSilenced_HomeMade[]=
			{
				"dz\sounds\weapons\firearms\CZ75\cz75Silenced",
				1,
				1,
				100
			};
			soundBeginExt[]=
			{
				
				{
					"beginSilenced_Pro1",
					0.5,
					"beginSilenced_Pro2",
					0.5
				},
				
				{
					"beginSilenced_HomeMade",
					1
				}
			};
		};
	};
	class Red9: Red9_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_Red90";
		descriptionShort="$STR_cfgWeapons_Red91";
		model="\dz\weapons\pistols\Red9\Red9.p3d";
		attachments[]=
		{
			"weaponButtstockRed9",
			"suppressorImpro"
		};
		itemSize[]={4,2};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\pistols\Red9\data\red9_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\weapons\pistols\Red9\data\red9.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\pistols\Red9\data\red9.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\pistols\Red9\data\red9.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\pistols\Red9\data\red9_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\pistols\Red9\data\red9_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\pistols\Red9\data\red9_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyRed9: ProxyAttachment
	{
		scope=2;
		inventorySlot="pistol";
		model="\dz\weapons\pistols\Red9\Red9.p3d";
	};
};
