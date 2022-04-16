class CfgPatches
{
	class DZ_Structures_Roads
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
class CfgVehicles
{
	class HouseNoDestruct;
	class Land_Tunnel_Biathlon_Left: HouseNoDestruct
	{
		scope=1;
		model="\dz\structures\roads\tunnels\Tunnel_Biathlon_Left.p3d";
	};
	class Land_Tunnel_Biathlon_Right: HouseNoDestruct
	{
		scope=1;
		model="\dz\structures\roads\tunnels\Tunnel_Biathlon_Right.p3d";
	};
};
