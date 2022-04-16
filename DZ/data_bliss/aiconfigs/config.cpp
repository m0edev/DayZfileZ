class CfgPatches
{
	class DZ_data_aiconfigs_bliss
	{
	};
};
class GroupBehaviourTemplates
{
	class BlissBearGroupBeh
	{
		type="Bear";
		agentPathLength=20;
		simpleLodGroupSpeed=1;
		singleAgentSafeKeeperDelayMin=30;
		singleAgentSafeKeeperDelayMax=50;
		safeKeeperIntervalMin=30;
		safeKeeperIntervalMax=50;
		class LifeCycleDayTime
		{
			class Activity1
			{
				endTimeMin=8;
				endTimeMax=9;
				zoneType="Graze";
			};
			class Activity2
			{
				endTimeMin=10;
				endTimeMax=11;
				zoneType="Graze";
			};
			class Activity3
			{
				endTimeMin=13;
				endTimeMax=14;
				zoneType="Graze";
			};
			class Activity4
			{
				endTimeMin=17;
				endTimeMax=18;
				zoneType="Graze";
			};
			class Activity5
			{
				endTimeMin=20;
				endTimeMax=21;
				zoneType="Graze";
			};
		};
	};
};
