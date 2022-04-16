class CfgPatches
{
	class DZ_Pistols_DE
	{
		units[]=
		{
			"Deagle",
			"Deagle_g"
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
	class Deagle_Base: Pistol_Base
	{
		scope=0;
		weight=1500;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		PPDOFProperties[]={1,0.89999998,10,250,5,10};
		WeaponLength=0.60000002;
		barrelArmor=0.80000001;
		initSpeedMultiplier=1;
		chamberSize=1;
		chamberedRound="";
		magazines[]=
		{
			"Mag_Deagle_9rnd"
		};
		chamberableFrom[]=
		{
			"Ammo_357"
		};
		ejectType=1;
		recoilModifier[]={1,1,1};
		swayModifier[]={1.3,1.3,0.89999998};
		simpleHiddenSelections[]=
		{
			"hide_barrel"
		};
		drySound[]=
		{
			"dz\sounds\weapons\firearms\FNX45\FNX_dry",
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
				"DEG_Shot_SoundSet",
				"DEG_Tail_SoundSet",
				"DEG_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"DEG_silencerPro_SoundSet",
					"DEG_silencerTail_SoundSet",
					"DEG_silencerInteriorTail_SoundSet"
				}
			};
			reloadTime=0.16;
			dispersion=0.003;
			magazineSlot="magazine";
		};
		class NoiseShoot
		{
			strength=82;
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
	class Deagle: Deagle_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_DE0";
		descriptionShort="$STR_cfgWeapons_DE1";
		model="\dz\weapons\pistols\DE\DE.p3d";
		attachments[]=
		{
			"weaponOpticsCrossbow",
			"pistolMuzzle"
		};
		itemSize[]={4,2};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\pistols\DE\data\deagle_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\weapons\pistols\DE\data\deagle.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=220;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"dz\weapons\pistols\DE\data\deagle.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"dz\weapons\pistols\DE\data\deagle.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"dz\weapons\pistols\DE\data\deagle_damage.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"dz\weapons\pistols\DE\data\deagle_damage.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"dz\weapons\pistols\DE\data\deagle_destruct.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Deagle_Gold: Deagle
	{
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\pistols\DE\data\deagle_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\weapons\pistols\DE\data\deagle_g.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=220;
					RefTexsMats[]=
					{
						"dz\weapons\pistols\de\data\deagle.rvmat"
					};
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"dz\weapons\pistols\DE\data\deagle_g.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"dz\weapons\pistols\DE\data\deagle_g.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"dz\weapons\pistols\DE\data\deagle_g_damage.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"dz\weapons\pistols\DE\data\deagle_g_damage.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"dz\weapons\pistols\DE\data\deagle_g_destruct.rvmat",
								"dz\weapons\pistols\DE\data\deagle_plastic_destruct.rvmat"
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
	class ProxyDE: ProxyAttachment
	{
		scope=2;
		inventorySlot="pistol";
		model="\dz\weapons\pistols\DE\DE.p3d";
	};
};
