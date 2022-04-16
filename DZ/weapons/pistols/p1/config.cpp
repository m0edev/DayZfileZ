class CfgPatches
{
	class DZ_Pistols_p1
	{
		units[]=
		{
			"P1"
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
	class P1_Base: Pistol_Base
	{
		scope=0;
		weight=960;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		modelOptics="-";
		distanceZoomMin=100;
		distanceZoomMax=100;
		value=0;
		chamberSize=1;
		chamberedRound="";
		magazines[]=
		{
			"Mag_P1_8Rnd"
		};
		chamberableFrom[]=
		{
			"Ammo_9x19"
		};
		ejectType=1;
		recoilModifier[]={1,1,1};
		drySound[]=
		{
			"DZ\sounds\weapons\firearms\waltherP1\WaltherP1_closure",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\waltherP1\WaltherP1_reload_00",
			0.80000001,
			1,
			20
		};
		reloadAction="ReloadRugerP1";
		modes[]=
		{
			"Single"
		};
		class Single: Mode_SemiAuto
		{
			soundSetShot[]=
			{
				"WaltherP1_Shot_SoundSet",
				"WaltherP1_Tail_SoundSet",
				"WaltherP1_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"WaltherP1_silencerPro_SoundSet",
					"WaltherP1_silencerTail_SoundSet",
					"WaltherP1_silencerInteriorTail_SoundSet"
				},
				
				{
					"WaltherP1_silencerHomeMade_SoundSet",
					"WaltherP1_silencerHomeMadeTail_SoundSet",
					"WaltherP1_silencerInteriorHomeMadeTail_SoundSet"
				}
			};
			begin1[]=
			{
				"dz\sounds\weapons\firearms\waltherP1\WaltherP1_close_0",
				1,
				1,
				600
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\waltherP1\WaltherP1_close_1",
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
			reloadTime=0.13;
			recoil="recoil_p1";
			recoilProne="recoil_p1_prone";
			dispersion=0.011;
			magazineSlot="magazine";
			beginSilenced_Pro[]=
			{
				"dz\sounds\weapons\firearms\waltherP1\P1Silenced",
				1,
				1,
				60
			};
			beginSilenced_HomeMade[]=
			{
				"dz\sounds\weapons\firearms\waltherP1\P1Silenced",
				1,
				1,
				100
			};
			soundBeginExt[]=
			{
				
				{
					"beginSilenced_Pro",
					1
				},
				
				{
					"beginSilenced_HomeMade",
					1
				}
			};
		};
	};
	class P1: P1_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_P10";
		descriptionShort="$STR_cfgWeapons_P11";
		model="\dz\weapons\pistols\p1\p1.p3d";
		attachments[]=
		{
			"pistolMuzzle",
			"suppressorImpro"
		};
		itemSize[]={3,2};
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
								"DZ\weapons\pistols\p1\data\p38.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\pistols\p1\data\p38.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\pistols\p1\data\p38_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\pistols\p1\data\p38_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\pistols\p1\data\p38_destruct.rvmat"
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
	class Proxyp1: ProxyAttachment
	{
		scope=2;
		inventorySlot="pistol";
		model="\dz\weapons\pistols\p1\p1.p3d";
	};
};
