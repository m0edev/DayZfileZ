class CfgPatches
{
	class DZ_Weapons_Shotguns_MP133
	{
		units[]=
		{
			"ShotgunMp133",
			"ShotgunMp133_Pistol_Grip"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Sounds_Weapons"
		};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class Mode_Single;
class Mode_Burst;
class Mode_FullAuto;
class OpticsInfoShotgun;
class cfgWeapons
{
	class Shotgun_Base;
	class Mp133Shotgun_Base: Shotgun_Base
	{
		scope=0;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		chamberSize=6;
		chamberedRound="";
		chamberableFrom[]=
		{
			"Ammo_12gaPellets",
			"Ammo_12gaSlug",
			"Ammo_12gaRubberSlug",
			"Ammo_12gaBeanbag"
		};
		magazines[]={};
		DisplayMagazine=0;
		PPDOFProperties[]={1,0.5,10,140,4,10};
		WeaponLength=1.0352499;
		barrelArmor=2.25;
		initSpeedMultiplier=1.25;
		ejectType=0;
		recoilModifier[]={1,1,1};
		swayModifier[]={2.3,2.3,1.2};
		cartridgeCreateTime[]={0.55000001,0.81999999};
		reloadSkips[]={0.25999999,0.40000001,0.52999997,0.63999999,0.73500001};
		drySound[]=
		{
			"dz\sounds\weapons\shotguns\Izh43\izh43_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\shotguns\mp133\mp133_reload_0",
			0.80000001,
			1,
			20
		};
		reloadSound[]=
		{
			"dz\sounds\weapons\shotguns\mp133\mp133_pump_0",
			1,
			1,
			20
		};
		reloadAction="ReloadMP133";
		shotAction="ReloadMP133Shot";
		hiddenSelections[]=
		{
			"camo"
		};
		modes[]=
		{
			"Single"
		};
		class Particles
		{
			class OnFire
			{
				class SmokeCloud
				{
					overrideParticle="weapon_shot_winded_smoke";
				};
				class SmokeCloud2
				{
					overrideParticle="weapon_shot_winded_smoke";
				};
				class SmokeCloud3BadlyDamaged
				{
					overrideParticle="weapon_shot_winded_smoke";
					onlyWithinHealthLabel[]={3,4};
				};
				class MuzzleFlash
				{
					overrideParticle="weapon_shot_mp133_01";
					illuminateWorld=1;
				};
				class BadlyDamagedChamberSmoke
				{
					overrideParticle="weapon_shot_chamber_smoke";
					overridePoint="Nabojnicestart";
					overrideDirectionPoint="Nabojniceend";
					onlyWithinHealthLabel[]={3,4};
				};
				class BadlyDamagedChamberSmokeRaise
				{
					overrideParticle="weapon_shot_chamber_smoke_raise";
					overridePoint="Nabojnicestart";
					onlyWithinHealthLabel[]={3,4};
				};
			};
			class OnOverheating
			{
				maxOverheatingValue=4;
				shotsToStartOverheating=1;
				overheatingDecayInterval=3;
				class SmokingBarrel
				{
					overrideParticle="smoking_barrel_small";
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
		class Single: Mode_Single
		{
			soundSetShot[]=
			{
				"Mp133_Shot_SoundSet",
				"Mp133_Tail_SoundSet",
				"Mp133_InteriorTail_SoundSet"
			};
			begin1[]=
			{
				"dz\sounds\weapons\shotguns\mp133\mp133_single_0",
				1,
				1,
				800
			};
			begin2[]=
			{
				"dz\sounds\weapons\shotguns\mp133\mp133_single_1",
				1,
				1,
				800
			};
			begin3[]=
			{
				"dz\sounds\weapons\shotguns\mp133\mp133_single_2",
				1,
				1,
				800
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
			reloadTime=1;
			recoil="recoil_Mp133";
			recoilProne="recoil_Mp133_prone";
			dispersion=0.0099999998;
			firespreadangle=1.5;
			magazineSlot="magazine";
		};
		class OpticsInfo: OpticsInfoShotgun
		{
			memoryPointCamera="eye";
			discreteDistance[]={25};
			discreteDistanceInitIndex=0;
			modelOptics="-";
			distanceZoomMin=100;
			distanceZoomMax=100;
		};
	};
	class Mp133Shotgun: Mp133Shotgun_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_Mp133Shotgun0";
		descriptionShort="$STR_cfgWeapons_Mp133Shotgun1";
		model="\dz\weapons\shotguns\mp133\mp133.p3d";
		animName="ShotgunMp133";
		attachments[]=
		{
			"pistolOptics"
		};
		itemSize[]={9,3};
		weight=3300;
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\shotguns\mp133\data\mp133_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\weapons\shotguns\mp133\data\mp133.rvmat"
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
								"DZ\weapons\shotguns\Mp133\data\mp133.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\shotguns\Mp133\data\mp133.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\shotguns\Mp133\data\mp133_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\shotguns\Mp133\data\mp133_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\shotguns\Mp133\data\mp133_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Mp133Shotgun_PistolGrip: Mp133Shotgun_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_Mp133Shotgun_PistolGrip0";
		descriptionShort="$STR_cfgWeapons_Mp133Shotgun_PistolGrip1";
		model="\dz\weapons\shotguns\mp133\mp133_pistol_grip.p3d";
		animName="ShotgunMp133_Pistol_Grip";
		attachments[]=
		{
			"pistolOptics"
		};
		itemSize[]={8,3};
		weight=2800;
		chamberSize=4;
		reloadAction="ReloadMP133_grip";
		shotAction="ReloadMP133_grip_shot";
		recoilModifier[]={1,1,1};
		swayModifier[]={1.5,1.5,1};
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
								"DZ\weapons\shotguns\Mp133\data\mp133_pistol_grip.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\shotguns\Mp133\data\mp133_pistol_grip.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\shotguns\Mp133\data\mp133_pistol_grip_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\shotguns\Mp133\data\mp133_pistol_grip_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\shotguns\Mp133\data\mp133_pistol_grip_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
