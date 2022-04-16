class CfgPatches
{
	class DZ_Pistols_mkii
	{
		units[]=
		{
			"mkii"
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
class OpticsInfoPistol;
class cfgWeapons
{
	class Pistol_Base;
	class MKII_Base: Pistol_Base
	{
		scope=0;
		weight=1300;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		PPDOFProperties[]={1,0.5,10,280,4,10};
		WeaponLength=0.56051803;
		barrelArmor=3.2049999;
		initSpeedMultiplier=0.85000002;
		chamberSize=1;
		chamberedRound="";
		magazines[]=
		{
			"Mag_MKII_10Rnd"
		};
		chamberableFrom[]=
		{
			"Ammo_22"
		};
		ejectType=1;
		recoilModifier[]={1,1,1};
		swayModifier[]={1,1,0.69999999};
		drySound[]=
		{
			"dz\sounds\weapons\firearms\FNX45\FNX_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\MkII\MkII_reload",
			0.69999999,
			1,
			20
		};
		reloadAction="ReloadRugerMK2";
		modes[]=
		{
			"SemiAuto"
		};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[]=
			{
				"AmphibianS_Shot_SoundSet",
				"AmphibianS_Tail_SoundSet",
				"AmphibianS_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"AmphibianS_silencerHomeMade_SoundSet",
					"AmphibianS_silencerHomeMadeTail_SoundSet",
					"AmphibianS_InteriorTail_SoundSet"
				}
			};
			begin1[]=
			{
				"dz\sounds\weapons\firearms\MkII\MkII_close_0",
				0.80000001,
				1,
				80
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\MkII\MkII_close_1",
				0.80000001,
				1,
				80
			};
			begin3[]=
			{
				"dz\sounds\weapons\firearms\MkII\MkII_close_2",
				0.80000001,
				1,
				80
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
				"dz\sounds\weapons\firearms\MkII\MkII_close_2",
				0.02,
				1,
				20
			};
			soundBeginExt[]=
			{
				
				{
					"beginSilenced_HomeMade",
					1
				}
			};
			reloadTime=0.13;
			recoil="recoil_mkii";
			recoilProne="recoil_mkii_prone";
			dispersion=0.0060000001;
			magazineSlot="magazine";
		};
		class NoiseShoot
		{
			strength=5;
			type="sound";
		};
		class OpticsInfo: OpticsInfoPistol
		{
			memoryPointCamera="eye";
			discreteDistance[]={25};
			discreteDistanceInitIndex=0;
			modelOptics="-";
			distanceZoomMin=100;
			distanceZoomMax=100;
		};
	};
	class MKII: MKII_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_MKII0";
		descriptionShort="$STR_cfgWeapons_MKII1";
		model="\dz\weapons\pistols\mkii\rugerMKII.p3d";
		attachments[]={};
		itemSize[]={4,2};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=120;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\pistols\mkii\data\ruger_metal_1.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\pistols\mkii\data\ruger_metal_1.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\pistols\mkii\data\ruger_metal_1_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\pistols\mkii\data\ruger_metal_1_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\pistols\mkii\data\ruger_metal_1_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash
				{
					overrideParticle="weapon_shot_fnx_02";
				};
				class SmokeCloud
				{
					overrideParticle="weapon_shot_winded_smoke_small";
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating=5;
				maxOverheatingValue=15;
				overheatingDecayInterval=1;
				class OpenChamberSmoke
				{
					onlyIfBoltIsOpen=1;
					overrideParticle="smoking_barrel_small";
					overridePoint="Nabojnicestart";
				};
			};
			class OnBulletCasingEject
			{
				class ChamberSmokeRaise
				{
					overrideParticle="weapon_shot_chamber_smoke_raise";
					overridePoint="Nabojnicestart";
				};
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class Proxymkii: ProxyAttachment
	{
		scope=2;
		inventorySlot="pistol";
		model="\dz\weapons\pistols\mkii\rugerMKII.p3d";
	};
};
