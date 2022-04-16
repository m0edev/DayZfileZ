class CfgPatches
{
	class DZ_Gear_Optics
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
class CfgVehicles
{
	class ItemOptics;
	class Rangefinder: ItemOptics
	{
		scope=2;
		displayName="$STR_CfgVehicles_Rangefinder0";
		descriptionShort="$STR_CfgVehicles_Rangefinder1";
		model="\dz\gear\optics\Rangefinder.p3d";
		simulation="itemoptics";
		animClass="Binoculars";
		itemSize[]={2,1};
		weight=270;
		rotationFlags=17;
		memoryPointCamera="eyeScope";
		cameraDir="cameraDir";
		class OpticsInfo
		{
			memoryPointCamera="eyeScope";
			cameraDir="cameraDir";
			modelOptics="\dz\gear\optics\opticview_rangefinder.p3d";
			distanceZoomMin=500;
			distanceZoomMax=500;
			opticsZoomMin="0.3926/4";
			opticsZoomMax="0.3926/4";
			opticsZoomInit="0.3926/4";
		};
		attachments[]=
		{
			"BatteryD"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=70;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\optics\data\Rangefinder.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\optics\data\Rangefinder.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\optics\data\Rangefinder_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\optics\data\Rangefinder_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\optics\data\Rangefinder_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		repairableWithKits[]={7};
		repairCosts[]={25};
		class EnergyManager
		{
			hasIcon=1;
			energyUsagePerSecond=0.0099999998;
			plugType=1;
			attachmentAction=1;
		};
		class MeleeModes
		{
			class Default
			{
				ammo="MeleeLightBlunt";
				range=1;
			};
			class Heavy
			{
				ammo="MeleeLightBlunt_Heavy";
				range=1;
			};
			class Sprint
			{
				ammo="MeleeLightBlunt_Heavy";
				range=2.8;
			};
		};
	};
	class Binoculars: ItemOptics
	{
		scope=2;
		displayName="$STR_cfgvehicles_optics_binoculars0";
		descriptionShort="$STR_cfgvehicles_optics_binoculars1";
		model="\dz\gear\optics\binoculars.p3d";
		simulation="itemoptics";
		animClass="Binoculars";
		itemSize[]={3,3};
		weight=980;
		rotationFlags=17;
		memoryPointCamera="eyeScope";
		cameraDir="cameraDir";
		class OpticsInfo
		{
			memoryPointCamera="eyeScope";
			cameraDir="cameraDir";
			modelOptics="\DZ\gear\optics\binoculars_view.p3d";
			distanceZoomMin=100;
			distanceZoomMax=100;
			opticsZoomMin=0.050000001;
			opticsZoomMax=0.050000001;
			opticsZoomInit=0.050000001;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=70;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\optics\data\binoculars.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\optics\data\binoculars.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\optics\data\binoculars_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\optics\data\binoculars_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\optics\data\binoculars_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class MeleeModes
		{
			class Default
			{
				ammo="MeleeLightBlunt";
				range=1;
			};
			class Heavy
			{
				ammo="MeleeLightBlunt_Heavy";
				range=1;
			};
			class Sprint
			{
				ammo="MeleeLightBlunt_Heavy";
				range=2.8;
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyEncore: ProxyAttachment
	{
		scope=2;
		inventorySlot[]=
		{
			"Pistol"
		};
		model="\dz\gear\optics\binoculars.p3d";
	};
};
