class CfgPatches
{
	class DZ_Pistols_Encore
	{
		units[]=
		{
			"LongHorn"
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
class Mode_Single;
class Mode_Burst;
class Mode_FullAuto;
class cfgWeapons
{
	class Pistol_Base;
	class LongHorn_Base: Pistol_Base
	{
		scope=0;
		inventorySlot[]=
		{
			""
		};
		weight=1700;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		modelOptics="-";
		distanceZoomMin=100;
		distanceZoomMax=100;
		value=0;
		chamberSize=1;
		chamberedRound="";
		magazines[]={};
		chamberableFrom[]=
		{
			"Ammo_308Win",
			"Ammo_308WinTracer"
		};
		ejectType=2;
		recoilModifier[]={1,1,1};
		drySound[]=
		{
			"dz\sounds\weapons\firearms\mosin9130\mosin_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\LongHorn\Longhorn_reload",
			0.80000001,
			1,
			20
		};
		ReloadAction="reloadLonghorn";
		modes[]=
		{
			"Single"
		};
		class Single: Mode_Single
		{
			soundSetShot[]=
			{
				"LongHorn_Shot_SoundSet",
				"LongHorn_Tail_SoundSet",
				"LongHorn_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"LongHorn_silencerHomeMade_SoundSet",
					"LongHorn_silencerHomeMadeTail_SoundSet",
					"LongHorn_silencerInteriorHomeMadeTail_SoundSet"
				}
			};
			begin1[]=
			{
				"dz\sounds\weapons\firearms\LongHorn\LongHorn_close_0",
				1,
				1,
				900
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\LongHorn\LongHorn_close_1",
				1,
				1,
				900
			};
			begin3[]=
			{
				"dz\sounds\weapons\firearms\LongHorn\LongHorn_close_2",
				1,
				1,
				900
			};
			soundBegin[]=
			{
				"begin1",
				0.33333001,
				"begin2",
				0.33333001,
				"begin3",
				0.33333001
			};
			beginSilenced_HomeMade[]=
			{
				"dz\sounds\weapons\firearms\m4a1\m4Silenced",
				1,
				1,
				150
			};
			soundBeginExt[]=
			{
				
				{
					"beginSilenced_HomeMade",
					1
				}
			};
			reloadTime=1;
			recoil="recoil_longhorn";
			recoilProne="recoil_longhorn_prone";
			dispersion=0.0020000001;
			magazineSlot="magazine";
		};
	};
	class LongHorn: LongHorn_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_LongHorn0";
		descriptionShort="$STR_cfgWeapons_LongHorn1";
		model="\dz\weapons\pistols\LongHorn\LongHorn.p3d";
		attachments[]=
		{
			"weaponOpticsCrossbow",
			"suppressorImpro"
		};
		itemSize[]={5,2};
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
								"DZ\weapons\pistols\LongHorn\data\LongHorn.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\pistols\LongHorn\data\LongHorn.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\pistols\LongHorn\data\LongHorn_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\pistols\LongHorn\data\LongHorn_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\pistols\LongHorn\data\LongHorn_destruct.rvmat"
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
	class ProxyLongHorn: ProxyAttachment
	{
		scope=2;
		inventorySlot="pistol";
		model="\dz\weapons\pistols\LongHorn\LongHorn.p3d";
	};
};
