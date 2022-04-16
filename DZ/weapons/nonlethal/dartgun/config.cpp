class CfgPatches
{
	class DZ_weapons_nonlethal_DartGun
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;
class cfgWeapons
{
	class NonlethalPistol;
	class DartGun: NonlethalPistol
	{
		scope=2;
		displayName="$STR_cfgWeapons_DartGun0";
		descriptionShort="$STR_cfgWeapons_DartGun1";
		model="\dz\weapons\nonlethal\dartgun\dartgun.p3d";
		weight=950;
		itemSize[]={6,3};
		absorbency=0;
		attachments[]=
		{
			"weaponOptics"
		};
		chamberedRound="D_Syringe";
		magazines[]=
		{
			"D_Syringe_Single",
			"Mag_Dartgun_CO2"
		};
		drySound[]=
		{
			"DZ\sounds\weapons\nonlethal\dartgun\Dartgun_dry",
			0.5,
			1,
			20
		};
		reloadMagazineSound[]=
		{
			"dz\sounds\weapons\bows\crossbow_reload",
			0.056234129,
			1,
			30
		};
		reloadSound[]=
		{
			"dz\sounds\weapons\bows\crossbow_magazine_reload",
			0.056234129,
			1,
			30
		};
		repairableWithKits[]={1};
		repairCosts[]={25};
		modes[]=
		{
			"Single"
		};
		recoilModifier[]={1,1,1};
		modelOptics="-";
		distanceZoomMin=100;
		distanceZoomMax=100;
		value=0;
		class Single: Mode_SemiAuto
		{
			soundSetShot[]=
			{
				"Dartgun_Shot_SoundSet",
				"Dartgun_Tail_SoundSet",
				"Dartgun_InteriorTail_SoundSet"
			};
			begin1[]=
			{
				"dz\sounds\weapons\x2_dart_gun\x2dart_gun_single_1",
				0.80000001,
				1,
				20
			};
			begin2[]=
			{
				"dz\sounds\weapons\x2_dart_gun\x2dart_gun_single_2",
				0.80000001,
				1,
				20
			};
			soundBegin[]=
			{
				"begin1",
				0.5,
				"begin2",
				0.5
			};
			reloadTime=0.69999999;
			recoil="recoil_bow";
			recoilProne="recoil_bow";
			dispersion=0.003;
			magazineSlot="magazine";
		};
		class Damage
		{
			tex[]={};
			mat[]=
			{
				"DZ\weapons\nonlethal\dartgun\data\dartgun.rvmat",
				"DZ\weapons\nonlethal\dartgun\data\dartgun_damage.rvmat",
				"DZ\weapons\nonlethal\dartgun\data\dartgun_destruct.rvmat"
			};
		};
		class NoiseShoot
		{
			strength=10;
			type="shot";
		};
	};
};
