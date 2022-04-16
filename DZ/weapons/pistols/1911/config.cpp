class CfgPatches
{
	class DZ_Pistols_1911
	{
		units[]=
		{
			"Colt1911",
			"1911_Engraved"
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
	class Colt1911_Base: Pistol_Base
	{
		scope=0;
		weight=1106;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		PPDOFProperties[]={1,0.89999998,10,250,4,10};
		WeaponLength=0.634148;
		barrelArmor=1.523;
		initSpeedMultiplier=1;
		chamberSize=1;
		chamberedRound="";
		magazines[]=
		{
			"Mag_1911_7Rnd"
		};
		chamberableFrom[]=
		{
			"Ammo_45ACP"
		};
		magazineSwitchTime=0.23999999;
		ejectType=1;
		recoilModifier[]={1,1,1};
		swayModifier[]={1.3,1.3,0.80000001};
		drySound[]=
		{
			"dz\sounds\weapons\firearms\colt1911\Colt1911_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\colt1911\Colt1911_reload",
			0.80000001,
			1,
			20
		};
		hiddenSelections[]=
		{
			"camoGround"
		};
		modes[]=
		{
			"SemiAuto"
		};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[]=
			{
				"Colt1911_Shot_SoundSet",
				"Colt1911_Tail_SoundSet",
				"Colt1911_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"Colt1911_silencerPro_SoundSet",
					"Colt1911_silencerTail_SoundSet",
					"Colt1911_silencerInteriorTail_SoundSet"
				}
			};
			begin1[]=
			{
				"dz\sounds\weapons\firearms\colt1911\Colt1911_0",
				1,
				1,
				700
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\colt1911\Colt1911_1",
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
				"begin2",
				0.33333001
			};
			reloadTime=0.13;
			recoil="recoil_1911";
			recoilProne="recoil_1911_prone";
			dispersion=0.0044999998;
			magazineSlot="magazine";
			beginSilenced_Pro[]=
			{
				"dz\sounds\weapons\firearms\colt1911\1911Silenced",
				1,
				1,
				60
			};
			beginSilenced_HomeMade[]=
			{
				"dz\sounds\weapons\firearms\colt1911\1911Silenced",
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
	class Colt1911: Colt1911_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_Colt19110";
		descriptionShort="$STR_cfgWeapons_Colt19111";
		hiddenSelections[]=
		{
			"camoGround"
		};
		model="\dz\weapons\pistols\1911\1911.p3d";
		attachments[]=
		{
			"pistolFlashlight",
			"pistolMuzzle"
		};
		itemSize[]={3,2};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\pistols\1911\data\1911_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\weapons\pistols\1911\data\1911.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\pistols\1911\data\1911.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\pistols\1911\data\1911.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\pistols\1911\data\1911_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\pistols\1911\data\1911_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\pistols\1911\data\1911_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Engraved1911: Colt1911_Base
	{
		scope=2;
		displayName="$STR_cfgweapons_engraved19110";
		descriptionShort="$STR_cfgweapons_engraved19111";
		model="\dz\weapons\pistols\1911\1911.p3d";
		attachments[]=
		{
			"pistolFlashlight",
			"pistolMuzzle"
		};
		itemSize[]={3,2};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\pistols\1911\data\1911_engraved_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\weapons\pistols\1911\data\1911_engraved.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\pistols\1911\data\1911.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\pistols\1911\data\1911_engraved.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\pistols\1911\data\1911_engraved_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\pistols\1911\data\1911_engraved_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\pistols\1911\data\1911_engraved_destruct.rvmat"
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
	class Proxy1911: ProxyAttachment
	{
		scope=2;
		inventorySlot="pistol";
		model="\dz\weapons\pistols\1911\1911.p3d";
	};
};
