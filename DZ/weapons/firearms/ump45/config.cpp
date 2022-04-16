class CfgPatches
{
	class DZ_Weapons_Firearms_UMP
	{
		units[]=
		{
			"UMP45"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Weapons_Firearms"
		};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;
class OpticsInfoRifle;
class cfgWeapons
{
	class Rifle_Base;
	class UMP45_Base: Rifle_Base
	{
		scope=0;
		weight=2500;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		PPDOFProperties[]={1,0.75,20,170,4,10};
		ironsightsExcludingOptics[]=
		{
			"M4_CarryHandleOptic",
			"BUISOptic",
			"M68Optic",
			"M4_T3NRDSOptic",
			"ReflexOptic"
		};
		WeaponLength=0.66352999;
		barrelArmor=0.89999998;
		initSpeedMultiplier=1.05;
		chamberSize=1;
		chamberedRound="";
		chamberableFrom[]=
		{
			"Ammo_45ACP"
		};
		magazines[]=
		{
			"Mag_UMP_25Rnd"
		};
		magazineSwitchTime=0.44999999;
		ejectType=1;
		recoilModifier[]={1,1,1};
		swayModifier[]={1.3,1.3,1};
		drySound[]=
		{
			"DZ\sounds\weapons\firearms\ump45\UMP45_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\ump45\ump45_reload",
			0.80000001,
			1,
			30
		};
		reloadAction="ReloadUMP";
		simpleHiddenSelections[]=
		{
			"hide_barrel"
		};
		hiddenSelections[]=
		{
			"camo"
		};
		modes[]=
		{
			"SemiAuto",
			"FullAuto"
		};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[]=
			{
				"UMP45_Shot_SoundSet",
				"UMP45_Tail_SoundSet",
				"UMP45_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"UMP45_silencerPro_SoundSet",
					"UMP45_silencerTail_SoundSet",
					"UMP45_silencerInteriorTail_SoundSet"
				},
				
				{
					"UMP45_silencerHomeMade_SoundSet",
					"UMP45_silencerHomeMadeTail_SoundSet",
					"UMP45_silencerInteriorHomeMadeTail_SoundSet"
				}
			};
			begin1[]=
			{
				"dz\sounds\weapons\firearms\ump45\ump45_single_0",
				1,
				1,
				700
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\ump45\ump45_single_1",
				1,
				1,
				700
			};
			begin3[]=
			{
				"dz\sounds\weapons\firearms\ump45\ump45_single_2",
				1,
				1,
				700
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
			reloadTime=0.12;
			recoil="recoil_auto_primary_7outof10";
			recoilProne="recoil_auto_primary_prone_7outof10";
			dispersion=0.003;
			magazineSlot="magazine";
			beginSilenced_Pro[]=
			{
				"dz\sounds\weapons\firearms\mp5k\mp5Silenced",
				1,
				1,
				75
			};
			beginSilenced_HomeMade[]=
			{
				"dz\sounds\weapons\firearms\mp5k\mp5Silenced",
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
		class FullAuto: Mode_FullAuto
		{
			soundSetShot[]=
			{
				"UMP45_Shot_SoundSet",
				"UMP45_Tail_SoundSet",
				"UMP45_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"UMP45_silencerPro_SoundSet",
					"UMP45_silencerTail_SoundSet",
					"UMP45_silencerInteriorTail_SoundSet"
				},
				
				{
					"UMP45_silencerHomeMade_SoundSet",
					"UMP45_silencerHomeMadeTail_SoundSet",
					"UMP45_silencerInteriorHomeMadeTail_SoundSet"
				}
			};
			begin1[]=
			{
				"dz\sounds\weapons\firearms\ump45\ump45_single_0",
				1,
				1,
				700
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\ump45\ump45_single_1",
				1,
				1,
				700
			};
			begin3[]=
			{
				"dz\sounds\weapons\firearms\ump45\ump45_single_2",
				1,
				1,
				700
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
			reloadTime=0.097999997;
			recoil="recoil_auto_primary_7outof10";
			recoilProne="recoil_auto_primary_prone_7outof10";
			dispersion=0.003;
			magazineSlot="magazine";
			beginSilenced_Pro[]=
			{
				"dz\sounds\weapons\firearms\mp5k\mp5Silenced",
				1,
				1,
				75
			};
			beginSilenced_HomeMade[]=
			{
				"dz\sounds\weapons\firearms\mp5k\mp5Silenced",
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
				class MuzzleFlash
				{
					overrideParticle="weapon_shot_ump45_01";
					ignoreIfSuppressed=1;
					illuminateWorld=1;
				};
				class SmokeCloud
				{
					overrideParticle="weapon_shot_winded_smoke_small";
				};
				class ChamberSmokeBurst
				{
					overrideParticle="weapon_shot_chamber_smoke";
					overridePoint="Nabojnicestart";
					overrideDirectionPoint="Nabojniceend";
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating=15;
				maxOverheatingValue=30;
				overheatingDecayInterval=1;
				class SmokingBarrel1
				{
					ignoreIfSuppressed=1;
					overrideParticle="smoking_barrel_small";
					onlyWithinOverheatLimits[]={0,0.30000001};
					onlyWithinRainLimits[]={0,0.2};
				};
				class SmokingBarrelHot1
				{
					ignoreIfSuppressed=1;
					overrideParticle="smoking_barrel";
					positionOffset[]={0.1,0,0};
					onlyWithinOverheatLimits[]={0.30000001,0.89999998};
					onlyWithinRainLimits[]={0,0.2};
				};
				class SmokingBarrelHot2
				{
					ignoreIfSuppressed=1;
					overrideParticle="smoking_barrel_heavy";
					positionOffset[]={0.1,0,0};
					onlyWithinOverheatLimits[]={0.89999998,1};
					onlyWithinRainLimits[]={0,0.2};
				};
				class SmokingBarrelHotSteam
				{
					overrideParticle="smoking_barrel_steam_small";
					positionOffset[]={0.1,0.050000001,0};
					onlyWithinOverheatLimits[]={0,0.5};
					onlyWithinRainLimits[]={0.2,1};
				};
				class SmokingBarrelHotSteam2
				{
					overrideParticle="smoking_barrel_steam";
					positionOffset[]={0.13,0.050000001,0};
					onlyWithinOverheatLimits[]={0.5,1};
					onlyWithinRainLimits[]={0.2,1};
				};
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
		class NoiseShoot
		{
			strength=66;
			type="shot";
		};
		class OpticsInfo: OpticsInfoRifle
		{
			memoryPointCamera="eye";
			modelOptics="-";
			distanceZoomMin=100;
			distanceZoomMax=100;
			discreteDistance[]={25};
			discreteDistanceInitIndex=0;
		};
	};
	class UMP45: UMP45_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_UMP450";
		descriptionShort="$STR_cfgWeapons_UMP451";
		model="\dz\weapons\firearms\UMP45\ump.p3d";
		attachments[]=
		{
			"weaponWrap",
			"weaponOptics",
			"weaponFlashlight",
			"pistolMuzzle"
		};
		itemSize[]={6,3};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\UMP45\data\ump45_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\weapons\firearms\UMP45\data\ump45.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=250;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\firearms\UMP45\data\ump45.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\firearms\UMP45\data\ump45.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\firearms\UMP45\data\ump45_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\firearms\UMP45\data\ump45_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\firearms\UMP45\data\ump45_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
