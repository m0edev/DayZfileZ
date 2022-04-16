class CfgPatches
{
	class DZ_Pistols_pmm
	{
		units[]=
		{
			"ij70"
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
	class MakarovIJ70_Base: Pistol_Base
	{
		scope=0;
		weight=760;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		PPDOFProperties[]={1,0.5,10,280,4,10};
		WeaponLength=0.56051803;
		barrelArmor=4.5;
		initSpeedMultiplier=1;
		chamberSize=1;
		chamberedRound="";
		magazines[]=
		{
			"Mag_IJ70_8Rnd"
		};
		chamberableFrom[]=
		{
			"Ammo_380"
		};
		ejectType=1;
		recoilModifier[]={1,1,1};
		swayModifier[]={1,1,0.69999999};
		drySound[]=
		{
			"dz\sounds\weapons\firearms\ij70\Makarov_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\ij70\Makarov_reload",
			0.80000001,
			1,
			20
		};
		reloadAction="ReloadIJ70";
		modes[]=
		{
			"SemiAuto"
		};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[]=
			{
				"Makarov_Shot_SoundSet",
				"Makarov_Tail_SoundSet",
				"Makarov_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"Makarov_silencerPro_SoundSet",
					"Makarov_silencerTail_SoundSet",
					"Makarov_silencerInteriorTail_SoundSet"
				}
			};
			begin1[]=
			{
				"dz\sounds\weapons\firearms\ij70\ij70_close_0",
				1,
				1,
				600
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\ij70\ij70_close_1",
				1,
				1,
				600
			};
			soundBegin[]=
			{
				"begin1",
				0.5,
				"begin2",
				0.5
			};
			reloadTime=0.13;
			recoil="recoil_ij70";
			recoilProne="recoil_if70_prone";
			dispersion=0.0060000001;
			magazineSlot="magazine";
			beginSilenced_Pro[]=
			{
				"dz\sounds\weapons\firearms\ij70\MakarovSilenced",
				1,
				1,
				60
			};
			beginSilenced_HomeMade[]=
			{
				"dz\sounds\weapons\firearms\ij70\MakarovSilenced",
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
		class Particles
		{
			class OnFire
			{
				class MuzzleFlashForward
				{
					ignoreIfSuppressed=1;
					illuminateWorld=1;
					overrideParticle="weapon_shot_fnx_01";
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
		};
		class NoiseShoot
		{
			strength=50;
			type="shot";
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
	class MakarovIJ70: MakarovIJ70_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_MakarovIJ700";
		descriptionShort="$STR_cfgWeapons_MakarovIJ701";
		model="\DZ\weapons\pistols\pmm\ij70.p3d";
		attachments[]=
		{
			"pistolMuzzle"
		};
		itemSize[]={2,2};
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
								"DZ\weapons\pistols\pmm\data\pmm.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\pistols\pmm\data\pmm.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\pistols\pmm\data\pmm_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\pistols\pmm\data\pmm_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\pistols\pmm\data\pmm_destruct.rvmat"
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
	class Proxyij70: ProxyAttachment
	{
		scope=2;
		inventorySlot="pistol";
		model="\DZ\weapons\pistols\pmm\ij70.p3d";
	};
};
