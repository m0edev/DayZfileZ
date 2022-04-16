class CfgPatches
{
	class DZ_Weapons_Firearms_Trumpet
	{
		units[]=
		{
			"Trumpet"
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
class OpticsInfoRifle;
class cfgWeapons
{
	class Rifle_Base;
	class Trumpet_Base: Rifle_Base
	{
		scope=0;
		animName="Trumpet";
		animClass="Shotgun";
		weight=2000;
		absorbency=0;
		repairableWithKits[]={1};
		repairCosts[]={25};
		chamberSize=7;
		chamberedRound="";
		chamberableFrom[]=
		{
			"Ammo_22"
		};
		magazines[]={};
		drySound[]=
		{
			"dz\sounds\weapons\shotguns\Izh43\izh43_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\firearms\trumpet\trombone_reload",
			0.80000001,
			1,
			20
		};
		reloadSound[]=
		{
			"dz\sounds\weapons\firearms\trumpet\trombone_pump",
			0.80000001,
			1,
			20
		};
		reloadAction="ReloadTrumpet";
		shotAction="ReloadTrumpet_shot";
		ejectType=0;
		recoilModifier[]={1,1,1};
		hiddenSelections[]=
		{
			"camo"
		};
		modes[]=
		{
			"Single"
		};
		class Single: Mode_Single
		{
			soundSetShot[]=
			{
				"Trumpet_Shot_SoundSet",
				"Trumpet_Tail_SoundSet",
				"Trumpet_InteriorTail_SoundSet"
			};
			soundSetShotExt[]=
			{
				
				{
					"Trumpet_silencerHomeMade_SoundSet",
					"Trumpet_silencerHomeMadeTail_SoundSet",
					"Trumpet_silencerInteriorHomeMadeTail_SoundSet"
				}
			};
			begin1[]=
			{
				"dz\sounds\weapons\firearms\trombone\trombone_shot_0",
				0.80000001,
				1,
				300
			};
			begin2[]=
			{
				"dz\sounds\weapons\firearms\trombone\trombone_shot_1",
				0.80000001,
				1,
				300
			};
			soundBegin[]=
			{
				"begin1",
				0.5,
				"begin2",
				0.5
			};
			beginSilenced_HomeMade[]=
			{
				"dz\sounds\weapons\firearms\m4a1\m4Silenced",
				0.050000001,
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
			recoil="recoil_single_primary_1outof10";
			recoilProne="recoil_single_primary_1outof10";
			dispersion=0.0015;
			magazineSlot="magazine";
		};
		class OpticsInfo: OpticsInfoRifle
		{
			modelOptics="-";
			distanceZoomMin=100;
			distanceZoomMax=100;
		};
	};
	class Trumpet: Trumpet_Base
	{
		scope=2;
		displayName="$STR_cfgWeapons_Trumpet0";
		descriptionShort="$STR_cfgWeapons_Trumpet1";
		model="\dz\weapons\firearms\Trumpet\trumpet.p3d";
		attachments[]=
		{
			"weaponOpticsHunting",
			"suppressorImpro"
		};
		itemSize[]={9,3};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\Trumpet\data\trombone_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\weapons\firearms\Trumpet\data\trumpet.rvmat"
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
								"DZ\weapons\firearms\Trumpet\data\trumpet.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\firearms\Trumpet\data\trumpet.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\firearms\Trumpet\data\trumpet_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\firearms\Trumpet\data\trumpet_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\firearms\Trumpet\data\trumpet_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class TrumpetParts: Inventory_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_TrumpetParts0";
		descriptionShort="$STR_CfgVehicles_TrumpetParts1";
		model="\dz\weapons\firearms\Trumpet\trumpet_parts.p3d";
		handheld="true";
		armAction="Disarm";
		itemSize[]={3,6};
		weight=2000;
		absorbency=0.1;
		barrelArmor=600;
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
								"DZ\weapons\firearms\Trumpet\data\trumpet.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\firearms\Trumpet\data\trumpet.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\firearms\Trumpet\data\trumpet_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\firearms\Trumpet\data\trumpet_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\firearms\Trumpet\data\trumpet_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class Melee
		{
			range=1;
			swingTime=0.5;
			action="GestureMeleeBayonetStab";
			ammo="MeleeFist";
			useCursor=1;
		};
	};
};
