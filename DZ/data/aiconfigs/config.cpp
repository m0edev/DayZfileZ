class CfgPatches
{
	class DZ_data_aiconfigs
	{
	};
};
class AIParamsDebug
{
	showViewCones="true";
	showLifeCycle="true";
};
class AIParams
{
	maxNoiseRange=300;
	rainToNoiseMultiplier=10;
	seaToNoiseMultiplier=15;
	noiseDampeningMultiplier=0.69999999;
	noiseCollisionDampeningMultiplier=0.2;
	noiseShotCollisionDampeningMultiplier=0.5;
	groupLODDistance=1000;
	class AgentTeams
	{
		TeamList[]=
		{
			"Player",
			"BigGame",
			"Zombies",
			"Predator",
			"AmbientLife"
		};
		class Player
		{
			friends[]=
			{
				"Player"
			};
		};
		class Predator
		{
			friends[]=
			{
				"Predator",
				"Zombies",
				"AmbientLife"
			};
		};
		class BigGame
		{
			friends[]=
			{
				"BigGame",
				"AmbientLife"
			};
		};
		class Zombies
		{
			friends[]=
			{
				"Zombies",
				"Predator",
				"AmbientLife"
			};
		};
		class AmbientLife
		{
			friends[]=
			{
				"AmbientLife"
			};
		};
	};
};
class GroupBehaviourTemplates
{
	class DZAmbientLifeGroupBeh
	{
		type="AmbientLifeGroup";
		groupRadius=10;
		groupMinCount=2;
		groupMaxCount=10;
		spawnMinDistanceBetweenAgents=3;
	};
	class DZWolfGroupBeh
	{
		type="Predators";
		alertDistributionSpeed=20;
		catchUpTestDelay=4;
		catchUpStartRadius=80;
		catchUpTargetRadius=7;
		groupRadius=10;
		spawnMinDistanceBetweenAgents=1;
		agentPathLength=100;
		atNeedMinDuration=60;
		atNeedMaxDuration=120;
		singleAgentSafeKeeperDelayMin=100;
		singleAgentSafeKeeperDelayMax=100;
		preyAttractionRange=350;
		innerOuterCircleRatio=0.60000002;
		endAttractionRange=10;
		attractionCooldown=60;
		endAttractionTime=300;
		targetEscapingSpeed=1.5;
		killAddFear=34;
		eatingTime=600;
		huntingCooldown=120;
		maxHuntingTime=60;
		safeKeeperIntervalMin=20;
		safeKeeperIntervalMax=40;
		siegeAttackCountdownMin=1;
		siegeAttackCountdownMax=12;
		huntAttackCountdownMin=1;
		huntAttackCountdownMax=10;
		changeTargetCooldown=5;
		changeTargetAlertRatio=1.3;
		changeTargetEffectRadius=6;
		subgroupSpacingMax=500;
		class LifeCycleDayTime
		{
			class Activity1
			{
				endTimeMin=8.5;
				endTimeMax=8.5;
				zoneType="Rest";
			};
			class Activity2
			{
				endTimeMin=16;
				endTimeMax=16;
				zoneType="HuntingGround";
				stayInZone="false";
			};
			class Activity3
			{
				endTimeMin=18.5;
				endTimeMax=19.5;
				zoneType="Rest";
			};
			class Activity4
			{
				endTimeMin=22.5;
				endTimeMax=23.5;
				zoneType="HuntingGround";
				stayInZone="false";
			};
		};
	};
	class DZDeerGroupBeh
	{
		type="WildHerbivores";
		alertDistributionSpeed=20;
		catchUpTestDelay=4;
		catchUpStartRadius=30;
		catchUpTargetRadius=7;
		groupRadius=10;
		spawnMinDistanceBetweenAgents=3;
		agentPathLength=60;
		atNeedMinDuration=60;
		atNeedMaxDuration=60;
		singleAgentSafeKeeperDelayMin=10;
		singleAgentSafeKeeperDelayMax=100;
		safeKeeperIntervalMin=10;
		safeKeeperIntervalMax=30;
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
				zoneType="Rest";
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
				zoneType="Water";
			};
		};
	};
	class DZdomesticGroupBeh
	{
		type="DomesticHerbivores";
		alertDistributionSpeed=0;
		groupMaxAlertedSpreadRadius=30;
		catchUpStartRadius=50;
		catchUpTargetRadius=15;
		groupRadius=10;
		spawnMinDistanceBetweenAgents=3;
		agentPathLength=60;
		atNeedMinDuration=60;
		atNeedMaxDuration=60;
		singleAgentSafeKeeperDelayMin=20;
		singleAgentSafeKeeperDelayMax=20;
		pauseLifeCycleAfterEscapeDuration=60;
		safeKeeperIntervalMin=10;
		safeKeeperIntervalMax=30;
		class LifeCycleDayTime
		{
			class Activity1
			{
				endTimeMin=8;
				endTimeMax=9;
				zoneType="Rest";
			};
			class Activity2
			{
				endTimeMin=11;
				endTimeMax=12;
				zoneType="Water";
			};
			class Activity3
			{
				endTimeMin=13;
				endTimeMax=14;
				zoneType="Graze";
			};
			class Activity4
			{
				endTimeMin=18;
				endTimeMax=19;
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
	class DZSheepGroupBeh
	{
		type="DomesticHerbivores";
		alertDistributionSpeed=0;
		groupMaxAlertedSpreadRadius=10;
		catchUpTestDelay=1;
		catchUpStartRadius=15;
		catchUpTargetRadius=5;
		groupRadius=10;
		spawnMinDistanceBetweenAgents=3;
		agentPathLength=30;
		atNeedMinDuration=60;
		atNeedMaxDuration=60;
		singleAgentSafeKeeperDelayMin=20;
		singleAgentSafeKeeperDelayMax=20;
		pauseLifeCycleAfterEscapeDuration=60;
		safeKeeperIntervalMin=10;
		safeKeeperIntervalMax=20;
		class LifeCycleDayTime
		{
			class Activity1
			{
				endTimeMin=8;
				endTimeMax=9;
				zoneType="Rest";
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
				endTimeMin=18;
				endTimeMax=19;
				zoneType="Rest";
			};
			class Activity5
			{
				endTimeMin=20;
				endTimeMax=21;
				zoneType="Graze";
			};
		};
	};
	class DZInfectedPackBeh
	{
		type="InfectedPack";
		alertDistributionSpeed=20;
		catchUpTestDelay=4;
		catchUpStartRadius=10;
		catchUpTargetRadius=7;
		groupRadius=10;
		spawnMinDistanceBetweenAgents=1;
		agentPathLength=100;
		maxEnragedTime=20;
		lifeCycleGroupSpeed=3;
		enragedRingDistanceMax=35;
		enragedRingDistanceMin=10;
		enragedReachDistanceMax=15;
		enragedReachDistanceMin=5;
		enragedOnReachCount=25;
		predictFollowingMaxDistance=3;
		predictFollowingTime=60;
		predictReachFollowingMaxDistance=30;
		predictReachFollowingTime=50;
		singleAgentSafeKeeperDelayMin=10;
		singleAgentSafeKeeperDelayMax=30;
		safeKeeperIntervalMin=10;
		safeKeeperIntervalMax=20;
		changeTargetCooldown=60;
		changeTargetAlertRatio=10;
		changeTargetEffectRadius=5;
		subgroupSpacingMax=5;
	};
};
class PathGraphFilters
{
	class ZombieCalm
	{
		class Flags
		{
			include[]=
			{
				"walk",
				"door",
				"inside"
			};
			exclude[]=
			{
				"disabled",
				"swim",
				"swimsea",
				"jump",
				"climb",
				"crawl",
				"crouch"
			};
		};
		class Costs
		{
			building=5;
		};
	};
	class ZombieAlerted
	{
		class Flags
		{
			include[]=
			{
				"walk",
				"door",
				"inside",
				"jump",
				"climb"
			};
			exclude[]=
			{
				"disabled",
				"crawl",
				"crouch"
			};
		};
		class Costs
		{
			jump0=3;
			jump1=0;
			jump2=0;
			jump3=0;
			jump4=0;
			water=5;
		};
	};
	class ZombieCrawl
	{
		class Flags
		{
			include[]=
			{
				"walk",
				"door",
				"inside"
			};
			exclude[]=
			{
				"disabled",
				"swim",
				"swimsea",
				"jump",
				"climb",
				"crouch"
			};
		};
	};
	class ChickenOnRun
	{
		class Flags
		{
			include[]=
			{
				"walk",
				"door",
				"inside",
				"jump"
			};
			exclude[]=
			{
				"disabled",
				"swim",
				"swimsea",
				"climb"
			};
		};
		class Costs
		{
			jump0=0;
			jump1=0;
			jump2=0;
			jump3=0;
			jump4=0.1;
			water=5;
		};
	};
	class DeerOnRun
	{
		class Flags
		{
			include[]=
			{
				"walk",
				"door",
				"inside",
				"jumpover"
			};
			exclude[]=
			{
				"disabled",
				"swim",
				"swimsea",
				"climb",
				"crawl",
				"crouch"
			};
		};
		class Costs
		{
			jump0=0;
			jump1=0;
			jump2=1;
			jump3=1;
			jump4=0;
			water=1;
			building=5;
		};
	};
	class RoeDeerOnRun
	{
		class Flags
		{
			include[]=
			{
				"walk",
				"door",
				"inside",
				"jumpover"
			};
			exclude[]=
			{
				"disabled",
				"swim",
				"swimsea",
				"climb",
				"crawl",
				"crouch"
			};
		};
		class Costs
		{
			jump0=0;
			jump1=0;
			jump2=1;
			jump3=0;
			jump4=0;
			water=1;
			building=5;
		};
	};
	class CowOnRun
	{
		class Flags
		{
			include[]=
			{
				"walk",
				"door",
				"inside",
				"jumpover"
			};
			exclude[]=
			{
				"disabled",
				"swim",
				"swimsea",
				"climb",
				"crawl",
				"crouch"
			};
		};
		class Costs
		{
			jump0=0;
			jump1=0;
			jump2=0.5;
			jump3=0;
			jump4=0;
			water=1;
			building=5;
		};
	};
	class WolfOnHunt
	{
		class Flags
		{
			include[]=
			{
				"walk",
				"door",
				"inside",
				"jumpover"
			};
			exclude[]=
			{
				"disabled",
				"swim",
				"swimsea",
				"climb",
				"crawl",
				"crouch"
			};
		};
		class Costs
		{
			jump0=0;
			jump1=0;
			jump2=1;
			jump3=1;
			jump4=0;
			water=1;
		};
	};
	class WolfOnOuterCircle
	{
		class Flags
		{
			include[]=
			{
				"walk",
				"jumpover"
			};
			exclude[]=
			{
				"disabled",
				"swim",
				"swimsea",
				"climb",
				"crawl",
				"crouch"
			};
		};
		class Costs
		{
			jump0=0;
			jump1=0;
			jump2=1;
			jump3=1;
			jump4=0;
			water=1;
		};
	};
	class NoJumping
	{
		class Flags
		{
			include[]=
			{
				"walk"
			};
			exclude[]=
			{
				"disabled",
				"swim",
				"swimsea",
				"climb",
				"crawl",
				"crouch",
				"jumpover"
			};
		};
		class Costs
		{
			jump0=0;
			jump1=0;
			jump2=1;
			jump3=0;
			jump4=0;
			water=1;
		};
	};
};
