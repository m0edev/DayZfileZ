class CfgPatches
{
	class DZ_Pistols_Flaregun
	{
		units[]={};
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
class OpticsInfoPistol;
class cfgWeapons
{
	class Pistol_Base;
	class Flaregun_Base: Pistol_Base
	{
		scope=0;
		weight=400;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		PPDOFProperties[]={1,0.5,10,250,4,10};
		WeaponLength=0.634148;
		barrelArmor=3.125;
		isSuicideWeapon=0;
		chamberSize=1;
		chamberedRound="";
		magazines[]={};
		chamberableFrom[]=
		{
			"Ammo_Flare",
			"Ammo_FlareRed",
			"Ammo_FlareGreen",
			"Ammo_FlareBlue"
		};
		DisplayMagazine=0;
		ejectType=2;
		recoilModifier[]={1,1,1};
		drySound[]=
		{
			"dz\sounds\weapons\firearms\FNX45\FNX_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\magnum\magnum_reload2",
			0.80000001,
			1,
			20
		};
		modes[]=
		{
			"Single"
		};
		class Single: Mode_Single
		{
			soundSetShot[]=
			{
				"Flare_Gun_Shot_SoundSet",
				"Flare_Gun_Tail_SoundSet",
				"Flare_Gun_InteriorTail_SoundSet"
			};
			begin1[]=
			{
				"dz\sounds\weapons\firearms\flare_gun\flare_shot_0",
				1.7782794,
				1,
				300
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\flare_gun\flare_shot_1",
				1.7782794,
				1,
				300
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
			dispersion=0.029999999;
			magazineSlot="magazine";
		};
		class NoiseShoot
		{
			strength=50;
			type="shot";
		};
		class NoiseFlare
		{
			strength=30;
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
	class Flaregun: Flaregun_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_Flaregun0";
		descriptionShort="$STR_cfgWeapons_Flaregun1";
		model="\dz\weapons\pistols\flaregun\flaregun.p3d";
		attachments[]={};
		itemSize[]={2,2};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=150;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\pistols\flaregun\data\flaregun.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\pistols\flaregun\data\flaregun.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\pistols\flaregun\data\flaregun_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\pistols\flaregun\data\flaregun_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\pistols\flaregun\data\flaregun_destruct.rvmat"
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
				shotsToStartOverheating=1;
				maxOverheatingValue=1;
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
	class ProxyFlaregun: ProxyAttachment
	{
		scope=2;
		inventorySlot="pistol";
		model="\dz\weapons\pistols\flaregun\flaregun.p3d";
	};
};
