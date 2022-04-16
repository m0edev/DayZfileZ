enum CarDoorState
{
	DOORS_MISSING,
	DOORS_OPEN,
	DOORS_CLOSED
}

enum CarHeadlightBulbsState
{
	NONE,
	LEFT,
	RIGHT,
	BOTH
}

enum CarRearLightType
{
	NONE,
	BRAKES_ONLY,
	REVERSE_ONLY,
	BRAKES_AND_REVERSE
}

class CarContactData
{
	vector localPos;
	IEntity other;
	float impulse;
	
	void CarContactData(vector _localPos, IEntity _other, float _impulse)
	{
		localPos = _localPos;
		other = _other;
		impulse = _impulse;
	}
}

typedef map<string, ref array<ref CarContactData>> CarContactCache

/*!
	Base script class for all motorized wheeled vehicles.
*/
class CarScript extends Car
{
	static ref map<typename, ref TInputActionMap> m_CarTypeActionsMap = new map<typename, ref TInputActionMap>;
	TInputActionMap m_InputActionMap;
	bool	m_ActionsInitialize;
	
	ref CarContactCache m_ContactCache;

	protected float m_Time;

	static float DROWN_ENGINE_THRESHOLD = 0.5;
	static float DROWN_ENGINE_DAMAGE = 350.0;
	
	//! keeps ammount of each fluid
	protected float m_FuelAmmount;
	protected float m_CoolantAmmount;
	protected float m_OilAmmount;
	protected float m_BrakeAmmount;

	//!
	protected float m_dmgContactCoef;
	protected float m_EnviroHeatComfortOverride;

	//!
	protected float m_DrownTime;
	static vector m_DrownEnginePos;
	
	//!
	protected float m_EngineHealth;
	protected float m_RadiatorHealth;
	protected float m_FuelTankHealth;
	protected float m_BatteryHealth;
	protected float m_PlugHealth;
	
	protected float	m_BatteryConsume = 15; 				//Battery energy consumption upon engine start
	protected float m_BatteryContinuousConsume = 0.25; 	//Battery consumption with lights on and engine is off
	protected float m_BatteryRecharge = 0.15; 			//Battery recharge rate when engine is on
	private	  float m_BatteryTimer = 0; 				//Used to factor energy consumption / recharging
	private const float BATTERY_UPDATE_DELAY = 100;

	//! Particles
	protected ref EffVehicleSmoke m_coolantFx;
	protected ref EffVehicleSmoke m_engineFx;
	protected ref EffVehicleSmoke m_exhaustFx;
		
	protected int m_enginePtcFx;
	protected int m_coolantPtcFx;
	protected int m_exhaustPtcFx;
	
	protected vector m_exhaustPtcPos;
	protected vector m_exhaustPtcDir;
	protected vector m_enginePtcPos;
	protected vector m_coolantPtcPos;
	
	protected vector m_fuelPos;

	protected vector m_enginePos;
	protected vector m_frontPos;
	protected vector m_backPos;
	protected vector m_side_1_1Pos;
	protected vector m_side_1_2Pos;
	protected vector m_side_2_1Pos;
	protected vector m_side_2_2Pos;	
	
	//!Sounds
	string m_EngineStartOK = "";
	string m_EngineStartBattery = "";
	string m_EngineStartPlug = "";
	string m_EngineStartFuel = "";
	string m_EngineStopFuel = "";
	
	string m_CarDoorOpenSound = "";
	string m_CarDoorCloseSound = "";
	
	ref EffectSound m_CrashSoundLight;
	ref EffectSound m_CrashSoundHeavy;
	ref EffectSound m_WindowSmall;
	ref EffectSound m_WindowLarge;
	
	protected bool m_PlayCrashSoundLight;
	protected bool m_PlayCrashSoundHeavy;
	
	protected bool m_HeadlightsOn;
	protected bool m_HeadlightsState;
	protected bool m_BrakesArePressed; // synchronized variable
	
	protected bool m_ForceUpdateLights;
	protected bool m_EngineDestroyed;
	
	CarLightBase 		m_Headlight;
	CarRearLightBase 	m_RearLight;
	
	// Memory points for lights
	static string m_ReverseLightPoint = "light_reverse";
	static string m_LeftHeadlightPoint = "light_left";
	static string m_RightHeadlightPoint = "light_right";
	static string m_LeftHeadlightTargetPoint = "light_left_dir";
	static string m_RightHeadlightTargetPoint = "light_right_dir";
	
	static string m_DrownEnginePoint = "drown_engine";
	
	// Model selection IDs for texture/material changes
	// If each car needs different IDs, then feel free to remove the 'static' flag and overwrite these numbers down the hierarchy
	static const int SELECTION_ID_FRONT_LIGHT_L 	= 0;
	static const int SELECTION_ID_FRONT_LIGHT_R 	= 1;
	static const int SELECTION_ID_BRAKE_LIGHT_L 	= 2;
	static const int SELECTION_ID_BRAKE_LIGHT_R 	= 3;
	static const int SELECTION_ID_REVERSE_LIGHT_L 	= 4;
	static const int SELECTION_ID_REVERSE_LIGHT_R 	= 5;
	static const int SELECTION_ID_TAIL_LIGHT_L 		= 6;
	static const int SELECTION_ID_TAIL_LIGHT_R 		= 7;
	static const int SELECTION_ID_DASHBOARD_LIGHT 	= 8;
	
	void CarScript()
	{
		SetEventMask(/*EntityEvent.CONTACT |*/ EntityEvent.POSTSIMULATE);
		
		m_ContactCache = new CarContactCache;
		
		m_Time = 0;

		// sets max health for all components at init
		m_EngineHealth = 1;
		m_FuelTankHealth = 1;
		m_RadiatorHealth = -1;
		m_BatteryHealth = -1;
		m_PlugHealth = -1;

		m_enginePtcFx = -1;
		m_coolantPtcFx = -1;
		m_exhaustPtcFx = -1;

		m_dmgContactCoef = 0;
		m_EnviroHeatComfortOverride = 0;

		m_PlayCrashSoundLight = false;
		m_PlayCrashSoundHeavy = false;
		
		RegisterNetSyncVariableBool("m_PlayCrashSoundLight");
		RegisterNetSyncVariableBool("m_PlayCrashSoundHeavy");
		RegisterNetSyncVariableBool("m_HeadlightsOn");
		RegisterNetSyncVariableBool("m_BrakesArePressed");
		RegisterNetSyncVariableBool("m_ForceUpdateLights");
		
		if 	( MemoryPointExists("ptcExhaust_end") )
		{
			//Print( "CarScript Init " + GetType() );
			m_exhaustPtcPos = GetMemoryPointPos("ptcExhaust_end");
			if ( MemoryPointExists("ptcExhaust_start") )
			{
				vector exhaustStart = GetMemoryPointPos("ptcExhaust_start");
				vector tempOri = vector.Direction( exhaustStart, m_exhaustPtcPos);//.Normalized();//.VectorToAngles();
				
				//m_exhaustPtcDir = tempOri;

				m_exhaustPtcDir[0] = -tempOri[2];
				m_exhaustPtcDir[1] = tempOri[1];
				m_exhaustPtcDir[2] = tempOri[0];
				
				m_exhaustPtcDir = m_exhaustPtcDir.Normalized().VectorToAngles();

			}
		}
		else
		{
			m_exhaustPtcPos = "0 0 0";
			m_exhaustPtcDir = "1 1 1";
		}
	
		if ( MemoryPointExists("ptcEnginePos") )
			m_enginePtcPos = GetMemoryPointPos("ptcEnginePos");
		else
			m_enginePtcPos = "0 0 0";		

		if ( MemoryPointExists("ptcCoolantPos") )
			m_coolantPtcPos = GetMemoryPointPos("ptcCoolantPos");
		else
			m_coolantPtcPos = "0 0 0";

		if ( MemoryPointExists("refill") )
			m_fuelPos = GetMemoryPointPos("refill");
		else
			m_fuelPos = "0 0 0";
		
		if ( MemoryPointExists("drown_engine") )
			m_DrownEnginePos = GetMemoryPointPos("drown_engine");
		else
			m_DrownEnginePos = "0 0 0";	

		if ( MemoryPointExists("dmgZone_engine") )
			m_enginePos = GetMemoryPointPos("dmgZone_engine");
		else
			m_enginePos = "0 0 0";

		if ( MemoryPointExists("dmgZone_front") )
			m_frontPos = GetMemoryPointPos("dmgZone_front");
		else
			m_frontPos = "0 0 0";

		if ( MemoryPointExists("dmgZone_back") )
			m_backPos = GetMemoryPointPos("dmgZone_back");
		else
			m_backPos = "0 0 0";		
				
		if ( MemoryPointExists("dmgZone_fender_1_1") )
			m_side_1_1Pos = GetMemoryPointPos("dmgZone_fender_1_1");
		else
			m_side_1_1Pos = "0 0 0";

		if ( MemoryPointExists("dmgZone_fender_1_2") )
			m_side_1_2Pos = GetMemoryPointPos("dmgZone_fender_1_2");
		else
			m_side_1_2Pos = "0 0 0";
		
		if ( MemoryPointExists("dmgZone_fender_2_1") )
			m_side_2_1Pos = GetMemoryPointPos("dmgZone_fender_2_1");
		else
			m_side_2_1Pos = "0 0 0";

		if ( MemoryPointExists("dmgZone_fender_2_2") )
			m_side_2_2Pos = GetMemoryPointPos("dmgZone_fender_2_2");
		else
			m_side_2_2Pos = "0 0 0";
	}

	vector GetEnginePosWS()
	{
		return ModelToWorld( m_DrownEnginePos );
	}
	
	vector GetCoolantPtcPosWS()
	{
		return ModelToWorld( m_coolantPtcPos );
	}

	vector GetRefillPointPosWS()
	{
		return ModelToWorld( m_fuelPos );
	}
	
	vector GetEnginePointPosWS()
	{	
		return ModelToWorld( m_enginePos );
	}
	vector GetFrontPointPosWS()
	{	
		return ModelToWorld( m_frontPos );
	}
	vector GetBackPointPosWS()
	{	
		return ModelToWorld( m_backPos );
	}
	vector Get_1_1PointPosWS()
	{	
		return ModelToWorld( m_side_1_1Pos );
	}
	vector Get_1_2PointPosWS()
	{	
		return ModelToWorld( m_side_1_2Pos );
	}
	vector Get_2_1PointPosWS()
	{	
		return ModelToWorld( m_side_2_1Pos );
	}
	vector Get_2_2PointPosWS()
	{	
		return ModelToWorld( m_side_2_2Pos );
	}


	//here we should handle the damage dealt in OnContact event, but maybe we will react even in that event 
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		ForceUpdateLightsStart();
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( ForceUpdateLightsEnd, 100, false );
		
		//string debugStr = "";
		//Print("CarScript>>> EEHitBy");
		//Print( dmgZone );
		//Print( damageResult );
		//Print( source );
		//debugStr = "Component" + component.ToString();
		//Print(  debugStr  );
		//Print( damageResult.GetDamage(dmgZone, "Health") );

		//if ( dmgZone == "Engine" && GetHealth("Engine","") < 0.1 )
		//{
		//	if ( GetHealth01("engine", "") <= 0.1 )
		//	{
		//		if ( !m_EngineSmoke )
		//		{
		//			Print("Smoke");
		//			EffVehicleSmoke engSmk = new EffEngineSmoke();
		//			SEffectManager.PlayOnObject(engSmk, this, "0 0.95 1.25" );
		//			// Particle is now playing on oject 'this'
		//		}
		//	}
		//}
	}


	override void EEDelete(EntityAI parent)
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			SEffectManager.DestroyEffect(m_coolantFx);
			SEffectManager.DestroyEffect(m_exhaustFx);
			SEffectManager.DestroyEffect(m_engineFx);
			
			if ( m_Headlight )
				m_Headlight.Destroy();
			
			if ( m_RearLight )
				m_RearLight.Destroy();
			
			SEffectManager.DestroyEffect( m_CrashSoundLight );			
			SEffectManager.DestroyEffect( m_CrashSoundHeavy );			
			SEffectManager.DestroyEffect( m_WindowSmall );			
			SEffectManager.DestroyEffect( m_WindowLarge );
		}
	}
	
	void CleanupSound(EffectSound sound)
	{
		SEffectManager.DestroyEffect(sound);
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
				
		if ( GetCrashHeavySound() )
			PlayCrashHeavySound();
		else if ( GetCrashLightSound() )
			PlayCrashLightSound();
		
		UpdateLights();
	}
	
	
	void CreateCarDestroyedEffect()
	{
		if ( !SEffectManager.IsEffectExist( m_enginePtcFx ) && GetGame().GetWaterDepth( GetEnginePosWS() ) <= 0 )
		{
			m_engineFx = new EffEngineSmoke();
			m_engineFx.SetParticleStateHeavy();
			m_enginePtcFx = SEffectManager.PlayOnObject( m_engineFx, this, m_enginePtcPos, Vector(0,0,0));
			
		}
	}
	
	override void EEItemAttached( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );
		if ( GetGame().IsServer() )
		{
			if ( slot_name == "Reflector_1_1" )
				SetHealth( "Reflector_1_1", "Health", item.GetHealth() );

			if ( slot_name == "Reflector_2_1" )
				SetHealth( "Reflector_2_1", "Health", item.GetHealth() );

			if ( slot_name == "CarBattery" )
				m_BatteryHealth = item.GetHealth01();
					
			if ( slot_name == "TruckBattery" )
				m_BatteryHealth = item.GetHealth01();

			if ( slot_name == "SparkPlug" )
				m_PlugHealth = item.GetHealth01();
			
			if ( slot_name == "GlowPlug" )
				m_PlugHealth = item.GetHealth01();
			
			Synchronize();
		}
		UpdateHeadlightState();
		UpdateLights();
	}
	
	// Updates state of attached headlight bulbs for faster access
	void UpdateHeadlightState()
	{
		EntityAI bulb_L = FindAttachmentBySlotName("Reflector_1_1");
		EntityAI bulb_R = FindAttachmentBySlotName("Reflector_2_1");
		
		if (bulb_L  &&  !bulb_L.IsRuined()  &&  bulb_R  &&  !bulb_R.IsRuined())
		{
			m_HeadlightsState = CarHeadlightBulbsState.BOTH;
		}
		else if (bulb_L  &&  !bulb_L.IsRuined())
		{
			m_HeadlightsState = CarHeadlightBulbsState.LEFT;
		}
		else if (bulb_R  &&  !bulb_R.IsRuined())
		{
			m_HeadlightsState = CarHeadlightBulbsState.RIGHT;
		}
		else if ( ( !bulb_L  ||  bulb_L.IsRuined() )  &&  ( !bulb_R  ||  bulb_R.IsRuined() ) )
		{
			m_HeadlightsState = CarHeadlightBulbsState.NONE;
		}
	}
	
	override void OnAttachmentRuined(EntityAI attachment)
	{
		super.OnAttachmentRuined(attachment);
		
		UpdateHeadlightState();
		UpdateLights();
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		if ( GetGame().IsServer() )
		{
			//int slot_id = InventorySlots.GetSlotIdFromString(slot_name);
			if ( IsScriptedLightsOn() )
			{
				if ( slot_name == "CarBattery" || slot_name == "TruckBattery" )
				{
					ToggleHeadlights();
				}
			}

			if ( EngineIsOn() )
			{
				//EngineBelt
				if ( slot_name == "GlowPlug" || slot_name == "SparkPlug" || slot_name == "CarBattery" || slot_name == "TruckBattery" )
					EngineStop();
			}
			
			if ( slot_name == "CarBattery" )
				m_BatteryHealth = -1;
					
			if ( slot_name == "TruckBattery" )
				m_BatteryHealth = -1;

			if ( slot_name == "SparkPlug" )
				m_PlugHealth = -1;
			
			if ( slot_name == "GlowPlug" )
				m_PlugHealth = -1;

			if ( slot_name == "CarRadiator" )
			{
				LeakAll( CarFluid.COOLANT );
				
				if ( m_DamageZoneMap.Contains( "Radiator" ) )
					SetHealth( "Radiator", "Health", 0);
			}

			Synchronize();
		}
		
		UpdateHeadlightState();
		UpdateLights();

	}

	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if ( !super.CanReleaseAttachment( attachment ) )
			return false;

		if ( EngineIsOn() && IsMoving() )
			return false;

		return true;
	}

	override void EOnPostSimulate(IEntity other, float timeSlice)
	{
		m_Time += timeSlice;
		
		if ( GetGame().IsServer() )
			CheckContactCache();

		//! move it to constants.c const float CAR_UPDATE_INTERVAL = 1.0
		if ( m_Time >= GameConstants.CARS_FLUIDS_TICK )
		{
			m_Time = 0;

			CarPartsHealthCheck();

			//First of all check if the car should stop the engine
			if ( GetGame().IsServer() && EngineIsOn() )
			{
				if ( GetFluidFraction(CarFluid.FUEL) <= 0 || m_EngineHealth <= 0 )
					EngineStop();

				CheckVitalItem( IsVitalCarBattery(), "CarBattery" );
				CheckVitalItem( IsVitalTruckBattery(), "TruckBattery" );
				CheckVitalItem( IsVitalSparkPlug(), "SparkPlug" );
				CheckVitalItem( IsVitalGlowPlug(), "GlowPlug" );
				// engine belt is not needed right now
				//CheckVitalItem( IsVitalEngineBelt(), "EngineBelt" );
			}

			//! actions runned when the engine on
			if ( EngineIsOn() )
			{
				if ( GetGame().IsServer() )
				{
					float dmg;

					if ( EngineGetRPM() >= EngineGetRPMRedline() )
					{
						if ( EngineGetRPM() > EngineGetRPMMax() )
							AddHealth( "Engine", "Health", -GetMaxHealth("Engine", "")); //CAR_RPM_DMG
							
						dmg = EngineGetRPM() * 0.001 * Math.RandomFloat( 0.02, 1.0 );  //CARS_TICK_DMG_MIN; //CARS_TICK_DMG_MAX
						//AddHealth( "Engine", "Health", -dmg); //CAR_RPM_DMG
						ProcessDirectDamage( DT_CUSTOM, null, "Engine", "EnviroDmg", "0 0 0", dmg );
					}

					//! leaking of coolant from radiator when damaged
					if ( IsVitalRadiator() )
					{
						if ( GetFluidFraction(CarFluid.COOLANT) > 0 && m_RadiatorHealth < 0.5 ) //CARS_LEAK_THRESHOLD
							LeakFluid( CarFluid.COOLANT );
					}

					if ( GetFluidFraction(CarFluid.FUEL) > 0 && m_FuelTankHealth < 0.5 )
						LeakFluid( CarFluid.FUEL );

					if ( GetFluidFraction(CarFluid.BRAKE) > 0 && m_EngineHealth < 0.5 )
						LeakFluid( CarFluid.BRAKE );

					if ( GetFluidFraction(CarFluid.OIL) > 0 && m_EngineHealth < 0.5 )
						LeakFluid( CarFluid.OIL );

					if ( m_EngineHealth < 0.25 )
						LeakFluid( CarFluid.OIL );
/*Commented out till indicator of Oil in HUD will be ready
					if ( GetFluidFraction( CarFluid.OIL ) < 1 )
					{
						dmg = Math.Lerp( 0.02, 10, 1 - GetFluidFraction( CarFluid.OIL ) );  //CARS_TICK_DMG_MIN; //CARS_TICK_DMG_MAX
						AddHealth( "Engine", "Health", -dmg);
					}
*/
					if ( IsVitalRadiator() )
					{
						if ( GetFluidFraction( CarFluid.COOLANT ) < 0.5 && GetFluidFraction( CarFluid.COOLANT ) >= 0 )
						{
							dmg = ( 1 - GetFluidFraction(CarFluid.COOLANT) ) * Math.RandomFloat( 0.02, 10.00 );  //CARS_DMG_TICK_MIN_COOLANT; //CARS_DMG_TICK_MAX_COOLANT
							AddHealth( "Engine", "Health", -dmg );
						}
					}
				}
				
				//FX only on Client and in Single
				if ( !GetGame().IsDedicatedServer() )
				{
					if ( !SEffectManager.IsEffectExist( m_exhaustPtcFx ) )
					{
						m_exhaustFx = new EffExhaustSmoke();
						m_exhaustPtcFx = SEffectManager.PlayOnObject( m_exhaustFx, this, m_exhaustPtcPos, m_exhaustPtcDir );
					}

					m_exhaustFx.SetParticleStateLight();
				
					if ( IsVitalRadiator() && SEffectManager.IsEffectExist( m_coolantPtcFx ) )
						SEffectManager.Stop(m_coolantPtcFx);
					
					if ( IsVitalRadiator() && GetFluidFraction( CarFluid.COOLANT ) < 0.5 )
					{
						if ( !SEffectManager.IsEffectExist( m_coolantPtcFx ) )
						{
							m_coolantFx = new EffCoolantSteam();
							m_coolantPtcFx = SEffectManager.PlayOnObject( m_coolantFx, this, m_coolantPtcPos, Vector(0,0,0));
						}

						if ( GetFluidFraction( CarFluid.COOLANT ) > 0 )
							m_coolantFx.SetParticleStateLight();
						else
							m_coolantFx.SetParticleStateHeavy();
/*
						Particle ptc;
						if ( GetCarDoorsState("NivaHood") == CarDoorState.DOORS_CLOSED )
						{
							if ( Class.CastTo(ptc, m_coolantFx.GetParticle() ) )
							{
								ptc.SetParameter( -1, EmitorParam.AIR_RESISTANCE, 55 );
								ptc.SetParameter( -1, EmitorParam.SIZE, 0.05 );
							}
						}
						else
						{
							if ( Class.CastTo(ptc, m_coolantFx.GetParticle() ) )
							{
								ptc.SetParameter( -1, EmitorParam.AIR_RESISTANCE, 2 );
								ptc.SetParameter( -1, EmitorParam.SIZE, 0.5 );
							}
						}
*/
					}
					else
					{
						if ( SEffectManager.IsEffectExist( m_coolantPtcFx ) )
							SEffectManager.Stop(m_coolantPtcFx);
					}
				}
			}
			else
			{
				//FX only on Client and in Single
				if ( !GetGame().IsDedicatedServer() )
				{
					if ( SEffectManager.IsEffectExist( m_exhaustPtcFx ) )
						SEffectManager.Stop(m_exhaustPtcFx);
					
					if ( SEffectManager.IsEffectExist( m_coolantPtcFx ) )
						SEffectManager.Stop(m_coolantPtcFx);
				}
			}
		}
	}
	
	void OnBrakesPressed()
	{
		UpdateLights();
	}
	
	void OnBrakesReleased()
	{
		UpdateLights();
	}
	
	override void OnUpdate( float dt )
    {
/*
//-----------------------------------------------------/
//------PREARATION FOR DROWNING PLAYRS IN THE CARS-----/
//-----------------------------------------------------/
        int crewCnt = CrewSize();
        if ( crewCnt > 0 )
        {
            for( int i = 0; i < CrewSize(); i++ )
            {
                Human crew = CrewMember( i );
                if ( !crew )
                    continue;
                
                int boneIdx = crew.GetBoneIndexByName("Head");
                if( boneIdx != -1 )
                {
                    vector pos1 = crew.GetBonePositionLS( boneIdx );
                    vector pos2 = crew.GetBonePositionMS( boneIdx );
                    vector pos3 = crew.GetBonePositionWS( boneIdx );

                    if ( GetGame().GetWaterDepth( pos1 ) > 0 )
                        Debug.DrawSphere(pos1, 0.03, COLOR_RED);
                    
					if ( GetGame().GetWaterDepth( pos2 ) > 0 )
                        Debug.DrawSphere(pos2, 0.03, COLOR_GREEN);
                    
                    if ( GetGame().GetWaterDepth( pos3 ) > 0 )
                        Debug.DrawSphere(pos3, 0.03, COLOR_YELLOW, ShapeFlags.NOZBUFFER | ShapeFlags.NOZWRITE);

                }
            }
        }
*/
		
		if ( GetGame().IsServer() )
		{
			ItemBase battery = GetBattery();
			if ( battery )
			{
				if ( EngineIsOn() )
				{
					m_BatteryTimer += dt;
					if ( m_BatteryTimer >= BATTERY_UPDATE_DELAY )
					{
						battery.GetCompEM().ConsumeEnergy(GetBatteryRechargeRate() * m_BatteryTimer);
						m_BatteryTimer = 0;
					}
				}
				else if ( !EngineIsOn() && IsScriptedLightsOn() )
				{
					m_BatteryTimer += dt;
					if ( m_BatteryTimer >= BATTERY_UPDATE_DELAY )
					{
						battery.GetCompEM().ConsumeEnergy(GetBatteryRuntimeConsumption() * m_BatteryTimer);
						m_BatteryTimer = 0;
						
						if ( battery.GetCompEM().GetEnergy() <= 0 )
						{
							ToggleHeadlights();
						}
					}
				}
			}
			
			if ( GetGame().GetWaterDepth( GetEnginePosWS() ) > 0 )
			{
				m_DrownTime += dt;
				if ( m_DrownTime > DROWN_ENGINE_THRESHOLD )
				{
					// *dt to get damage per second
					AddHealth( "Engine", "Health", -DROWN_ENGINE_DAMAGE * dt);
				}
				//Debug.DrawSphere(GetCoolantPtcPosWS(), 0.2, COLOR_RED, ShapeFlags.NOZBUFFER | ShapeFlags.NOZWRITE);
			}
			else
			{
				m_DrownTime = 0;
			}
		}
		// Visualisation of brake lights for all players
		float brake_coef = GetController().GetBrake();
		
		if ( brake_coef > 0 )
		{
			if ( !m_BrakesArePressed )
			{
				m_BrakesArePressed = true;
				SetSynchDirty();
				OnBrakesPressed();
			}
		}
		else
		{
			if ( m_BrakesArePressed )
			{
				m_BrakesArePressed = false;
				SetSynchDirty();
				OnBrakesReleased();
			}
		}
		
		if ( (!GetGame().IsDedicatedServer()) && m_ForceUpdateLights )
		{
			UpdateLights();
			m_ForceUpdateLights = false;
		}
    }
	
	override void EEKilled(Object killer)
	{
		m_EngineDestroyed = true;
	}
	
	//! WARNING: Can be called very frequently in one frame, use with caution
	override void OnContact( string zoneName, vector localPos, IEntity other, Contact data )
	{
		if ( GetGame().IsServer() )
		{
			array<ref CarContactData> ccd;
			if (!m_ContactCache.Find(zoneName, ccd))
			{
				ccd = new array<ref CarContactData>;
				m_ContactCache.Insert(zoneName, ccd);
			}
			
			ccd.Insert(new CarContactData(localPos, other, data.Impulse));
		}
	}
	
	//! Responsible for damaging the car according to contact events from OnContact
	void CheckContactCache()
	{
		int contactZonesCount = m_ContactCache.Count();
		
		if ( contactZonesCount == 0 )
			return;
		
		bool playLightSound = false;
		bool playHeavySound = false;
		
		for ( int i = 0; i < contactZonesCount; ++i)
		{
			string zoneName = m_ContactCache.GetKey(i);
			array<ref CarContactData> data = m_ContactCache[zoneName];

			float dmg;
			
			int contactCount = data.Count();
			for ( int j = 0; j < contactCount; ++j )
				dmg = data[j].impulse * m_dmgContactCoef;
			
			if ( dmg < GameConstants.CARS_CONTACT_DMG_MIN )
				continue;
			
			int pddfFlags;

			if ( dmg < GameConstants.CARS_CONTACT_DMG_THRESHOLD )
			{				
				playLightSound = true;
				pddfFlags = ProcessDirectDamageFlags.NO_TRANSFER;
			}
			else
			{		
				DamageCrew(dmg);
				playHeavySound = true;	
				pddfFlags = 0;
			}
			
			ProcessDirectDamage( DT_CUSTOM, null, zoneName, "EnviroDmg", "0 0 0", dmg, pddfFlags );
		}
		
		if (playLightSound)
			SynchCrashLightSound( true );
		
		if (playHeavySound)
			SynchCrashHeavySound( true );
		
		UpdateHeadlightState();
		UpdateLights();
		
		m_ContactCache.Clear();
	}
	
	//! Responsible for damaging crew in a car crash
	void DamageCrew(float dmg)
	{
		for ( int c = 0; c < CrewSize(); ++c )
		{
			Human crew = CrewMember( c );
			if ( !crew )
				continue;

			PlayerBase player;
			if ( Class.CastTo(player, crew ) )
			{
				if ( dmg > GameConstants.CARS_CONTACT_DMG_KILLCREW )
				{		
					player.SetHealth(0.0);
				}
				else
				{
					float shockTemp = Math.InverseLerp(GameConstants.CARS_CONTACT_DMG_THRESHOLD, GameConstants.CARS_CONTACT_DMG_KILLCREW, dmg);
					float shock = Math.Lerp( 50, 100, shockTemp );
					float hp = Math.Lerp( 2, 60, shockTemp );

					// These should ideally be ProcessDirectDamage...
					player.AddHealth("", "Shock", -shock );
					player.AddHealth("", "Health", -hp );
					//Print( "SHOCK..........." + shock );
					//Print( "HEALTH..........." + hp );
				}
			}
		}
	}

	/*!
		Gets called every sound simulation step.
		In this callback, user can modify behaviour of sound controllers
	*/
	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		if ( GetGame().IsServer() )
		{
			if ( GetCrashLightSound() )
				m_PlayCrashSoundLight = false;
		
			if ( GetCrashHeavySound() )
				m_PlayCrashSoundHeavy = false;
		}
		
		switch ( ctrl )
		{
			// in this controller the higher returned value the more muted sounds inside the car
			case CarSoundCtrl.DOORS:
				break;
		}

		// if you don't wanna change the behaviour, just return the old value.
		return oldValue;
	}
	
	float GetEnviroHeatComfortOverride()
	{
		return Math.Clamp(m_EnviroHeatComfortOverride, -1, 1);
	}

	/*!
		Gets called everytime when the specified vehicle's fluid
		changes its current value eg. when car is consuming fuel.

		This callback is called on both server and client.
	*/
	override void OnFluidChanged( CarFluid fluid, float newValue, float oldValue )
	{
		switch ( fluid )
		{
			case CarFluid.FUEL:
				m_FuelAmmount = newValue;
				break;

			case CarFluid.OIL:
				m_OilAmmount = newValue;
				break;

			case CarFluid.BRAKE:
				m_BrakeAmmount = newValue;
				break;

			case CarFluid.COOLANT:
				m_CoolantAmmount = newValue;
				break;
		}
	}

	/*!
		Gets called everytime the game wants to start the engine.
		This callback is called on server only.

		\return true if the engine can start, false otherwise.
	*/
	override bool OnBeforeEngineStart()
	{
		// todo :: check if the battery is plugged-in
		//         check if we have enough power to start the engine
/*
		if (  GetFluidFraction( CarFluid.FUEL ) <= 0 )
		{
			if ( !GetGame().IsDedicatedServer() )
				SEffectManager.PlaySound("offroad_engine_failed_start_fuel_SoundSet", GetPosition() );

			return false;
		}
		
		EntityAI item = null;
		//---------------
		if ( IsVitalCarBattery() )
		{
			item = FindAttachmentBySlotName("CarBattery");

			if ( !item || (item && item.IsRuined() ) )
			{
				if ( !GetGame().IsDedicatedServer() )
					SEffectManager.PlaySound("offroad_engine_failed_start_battery_SoundSet", GetPosition() );
				return false;
			}
		}

		//---------------
		item = null;
		if ( IsVitalTruckBattery() )
		{
			item = FindAttachmentBySlotName("TruckBattery");

			if ( !item || ( item && item.IsRuined() ) )
			{
				if ( !GetGame().IsDedicatedServer() )
					SEffectManager.PlaySound("offroad_engine_failed_start_battery_SoundSet", GetPosition() );
				return false;
			}
		}

		//---------------
		item = null;
		if ( IsVitalSparkPlug() )
		{
			item = FindAttachmentBySlotName("SparkPlug");

			if ( !item || ( item && item.IsRuined() ) )
			{
				if ( !GetGame().IsDedicatedServer() )
					SEffectManager.PlaySound("offroad_engine_failed_start_sparkplugs_SoundSet", GetPosition() );
				return false;
			}
		}
	
		//---------------
		item = null;
		if ( IsVitalGlowPlug() )
		{
			item = FindAttachmentBySlotName("GlowPlug");

			if ( !item || ( item && item.IsRuined() ) )
			{
				if ( !GetGame().IsDedicatedServer() )
					SEffectManager.PlaySound("offroad_engine_failed_start_sparkplugs_SoundSet", GetPosition() );
				return false;
			}
		}
		

		//TODO:: make it proper when the belt will be ready
		//item = null;
		//m_BeltCon = true;
		
		//if ( IsVitalEngineBelt() )
		//{
			//item = FindAttachmentBySlotName("EngineBelt");

			//if ( !item || (item && item.IsRuined() ) )
			//{
				//if ( !GetGame().IsDedicatedServer() )
					//SEffectManager.PlaySound("", GetPosition() );
				//return false;
			//}
		//}

*/
		return true;
	}

	override void OnGearChanged( int newGear, int oldGear )
	{
		UpdateLights(newGear);
	}
	
	//! Gets called everytime the engine starts.
	override void OnEngineStart()
	{
		
		ItemBase battery = GetBattery();
		/*if ( IsVitalCarBattery() ) 
			battery = ItemBase.Cast( FindAttachmentBySlotName("CarBattery") );
		else if ( IsVitalTruckBattery() ) 
			battery = ItemBase.Cast( FindAttachmentBySlotName("TruckBattery") );*/
		
		if (GetGame().IsServer() && battery)
		{
			battery.GetCompEM().ConsumeEnergy(GetBatteryConsumption());
		}
		
		UpdateLights();
	}

	//! Gets called everytime the engine stops.
	override void OnEngineStop()
	{
		UpdateLights();
/*
		if ( !GetGame().IsDedicatedServer() )
		{		
			EffectSound sound =	SEffectManager.PlaySound("offroad_engine_stop_fuel_SoundSet", GetPosition() );
			sound.SetAutodestroy( true );
		}
*/
	}

	/*!
		Gets called everytime the game wants to switch the lights.
		\return true when lights can be switched, false otherwise.
	*/
	bool OnBeforeSwitchLights( bool toOn )
	{
		if ( toOn )
		{
			// todo :: check if the battery is plugged-in
			//         check if we have enough power to turn on the lights

			// this is the case from off -> on
			
			EntityAI battery = null;
			
			EntityAI item1 = null;
			EntityAI item2 = null;
			
			
			if ( IsVitalCarBattery() ) battery = FindAttachmentBySlotName("CarBattery");
			if ( IsVitalTruckBattery() ) battery = FindAttachmentBySlotName("TruckBattery");
				
			if ( !battery || battery.IsRuined() )
				return false;
			
			item1 = FindAttachmentBySlotName("Reflector_1_1");
			item2 = FindAttachmentBySlotName("Reflector_2_1");
			
			if ( !item1 && !item2 )
				return false;
			else if ( item1 && item1.IsRuined() && item2 && item2.IsRuined() )
				return false;

/* 
			//Should be redundand with the check above (for the headlights)
			if ( GetHealthLevel( "Reflector_1_1" ) >= STATE_RUINED && GetHealthLevel( "Reflector_2_1" ) >= STATE_RUINED )
				return false;
*/
		}

		// this is the case on -> off
		return true;
	}
	
	//! Propper way to get if light is swiched on. Use instead of IsLightsOn().
	bool IsScriptedLightsOn()
	{
		return m_HeadlightsOn;
	}
	
	//! Switches headlights on/off, including the illumination of the control panel and synchronizes this change to all clients.
	void ToggleHeadlights()
	{
		m_HeadlightsOn = !m_HeadlightsOn;
		SetSynchDirty();
		
		UpdateLights();
	}
	
	//! Updates representation of lights. Call on server and client after change in relevant mechanics.
	void UpdateLights(int new_gear = -1) // -1 is invalid gear.
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // client side
		{
			ItemBase battery;
			
			if ( IsVitalCarBattery() ) 
				battery = ItemBase.Cast( FindAttachmentBySlotName("CarBattery") );
			else if ( IsVitalTruckBattery() ) 
				battery = ItemBase.Cast( FindAttachmentBySlotName("TruckBattery") );

			if ( battery && battery.IsAlive() && battery.GetCompEM().GetEnergy() > 0 )
			{
				// HEADLIGHTS
				
				if (m_HeadlightsOn)
				{
					DashboardShineOn();
					
					if (!m_Headlight  &&  m_HeadlightsState != CarHeadlightBulbsState.NONE)
					{
						m_Headlight = CreateFrontLight();
						TailLightsShineOn();
					}
					
					if (m_HeadlightsState == CarHeadlightBulbsState.LEFT)
					{
						m_Headlight.AttachOnMemoryPoint(this, m_LeftHeadlightPoint, m_LeftHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						LeftFrontLightShineOn();
						RightFrontLightShineOff();
					}
					else if (m_HeadlightsState == CarHeadlightBulbsState.RIGHT)
					{
						m_Headlight.AttachOnMemoryPoint(this, m_RightHeadlightPoint, m_RightHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						RightFrontLightShineOn();
						LeftFrontLightShineOff();
					}
					else if (m_HeadlightsState == CarHeadlightBulbsState.BOTH)
					{
						vector local_pos_left = GetMemoryPointPos(m_LeftHeadlightPoint);
						vector local_pos_right = GetMemoryPointPos(m_RightHeadlightPoint);
						
						vector local_pos_middle = (local_pos_left + local_pos_right) *0.5;
						m_Headlight.AttachOnObject(this, local_pos_middle);
						m_Headlight.AggregateLight();
						LeftFrontLightShineOn();
						RightFrontLightShineOn();
					}
					
					if (m_Headlight  &&  m_HeadlightsState == CarHeadlightBulbsState.NONE)
					{
						m_Headlight.FadeOut();
						m_Headlight = null;
						LeftFrontLightShineOff();
						RightFrontLightShineOff();
						TailLightsShineOff();
					}
				}
				else
				{
					TailLightsShineOff();
					DashboardShineOff();
					LeftFrontLightShineOff();
					RightFrontLightShineOff();
					
					if (m_Headlight)
					{
						m_Headlight.FadeOut();
						m_Headlight = null;
					}
				}
			
				// REAR LIGHTS (brakes, reverse & tail lights)
			
				if ( EngineIsOn() )
				{
					int reverse_light_state = CarRearLightType.NONE;
					
					// reverse
					
					int gear;
					
					if (new_gear == -1)
						gear = GetController().GetGear();
					else
						gear = new_gear;
					
					
					if (gear == CarGear.REVERSE)
					{
						reverse_light_state = CarRearLightType.REVERSE_ONLY;
					}
					else
					{
						reverse_light_state = CarRearLightType.NONE;
					}
					
					// brakes
					
					if (m_BrakesArePressed)
					{
						if (reverse_light_state == CarRearLightType.REVERSE_ONLY)
						{
							reverse_light_state = CarRearLightType.BRAKES_AND_REVERSE;
						}
						else
						{
							reverse_light_state = CarRearLightType.BRAKES_ONLY;
						}
					}
					
					if (reverse_light_state != CarRearLightType.NONE    &&  m_HeadlightsState != CarHeadlightBulbsState.NONE  &&  !m_RearLight)
					{
						m_RearLight = CreateRearLight();
						vector local_pos = GetMemoryPointPos(m_ReverseLightPoint);
						m_RearLight.AttachOnObject(this, local_pos, "180 0 0");
					}
					
					if (m_RearLight)
					{
						if (reverse_light_state == CarRearLightType.REVERSE_ONLY)
						{
							m_RearLight.SetAsSegregatedReverseLight();

							if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
							{
								ReverseLightsShineOn();
								BrakeLightsShineOff();
							}
						}
						else if (reverse_light_state == CarRearLightType.BRAKES_ONLY)
						{
							m_RearLight.SetAsSegregatedBrakeLight();
							
							if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
							{
								ReverseLightsShineOff();
								BrakeLightsShineOn();
							}
						}
						else if (reverse_light_state == CarRearLightType.BRAKES_AND_REVERSE)
						{
							m_RearLight.AggregateLight();
							m_RearLight.SetFadeOutTime(1);
							
							if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
							{
								BrakeLightsShineOn();
								ReverseLightsShineOn();
							}
						}
						else if (reverse_light_state == CarRearLightType.NONE)
						{
							m_RearLight.FadeOut();
							m_RearLight = null;
							ReverseLightsShineOff();
						}
					}
					else
					{
						ReverseLightsShineOff();
						BrakeLightsShineOff();
					}
				}
				else
				{
					BrakeLightsShineOff();
					ReverseLightsShineOff();
					
					if (m_RearLight)
					{
						m_RearLight.FadeOut();
						m_RearLight = null;
					}
				}
			}
			else
			{
				LeftFrontLightShineOff();
				RightFrontLightShineOff();
				DashboardShineOff();
				BrakeLightsShineOff();
				ReverseLightsShineOff();
				
				if (m_RearLight)
				{
					m_RearLight.FadeOut();
					m_RearLight = null;
				}
				
				if (m_Headlight)
				{
					m_Headlight.FadeOut();
					m_Headlight = null;
				}
			}
		}
	}
	
	void LeftFrontLightShineOn()
	{
		string material = ConfigGetString("frontReflectorMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_FRONT_LIGHT_L, material);
		}
	}
	
	void RightFrontLightShineOn()
	{
		string material = ConfigGetString("frontReflectorMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_FRONT_LIGHT_R, material);
		}
	}
	
	void LeftFrontLightShineOff()
	{
		string material = ConfigGetString("frontReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_FRONT_LIGHT_L, material);
		}
	}
	
	void RightFrontLightShineOff()
	{
		string material = ConfigGetString("frontReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_FRONT_LIGHT_R, material);
		}
	}
	
	void ReverseLightsShineOn()
	{
		string material = ConfigGetString("ReverseReflectorMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_REVERSE_LIGHT_L, material);
			SetObjectMaterial(SELECTION_ID_REVERSE_LIGHT_R, material);
		}
	}
	
	void ReverseLightsShineOff()
	{
		string material = ConfigGetString("ReverseReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_REVERSE_LIGHT_L, material);
			SetObjectMaterial(SELECTION_ID_REVERSE_LIGHT_R, material);
		}
	}
	
	void BrakeLightsShineOn()
	{
		string material = ConfigGetString("brakeReflectorMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_BRAKE_LIGHT_L, material);
			SetObjectMaterial(SELECTION_ID_BRAKE_LIGHT_R, material);
		}
	}
	
	void BrakeLightsShineOff()
	{
		string material = ConfigGetString("brakeReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_BRAKE_LIGHT_L, material);
			SetObjectMaterial(SELECTION_ID_BRAKE_LIGHT_R, material);
		}
	}
	
	void TailLightsShineOn()
	{
		string material = ConfigGetString("TailReflectorMatOn");
		string material_off = ConfigGetString("TailReflectorMatOff");
		
		if (material != ""  &&  material_off != "")
		{
			if (m_HeadlightsState == CarHeadlightBulbsState.LEFT)
			{
				SetObjectMaterial(SELECTION_ID_TAIL_LIGHT_L, material);
				SetObjectMaterial(SELECTION_ID_TAIL_LIGHT_R, material_off);
			}
			else if (m_HeadlightsState == CarHeadlightBulbsState.RIGHT)
			{
				SetObjectMaterial(SELECTION_ID_TAIL_LIGHT_R, material);
				SetObjectMaterial(SELECTION_ID_TAIL_LIGHT_L, material_off);
			}
			else if (m_HeadlightsState == CarHeadlightBulbsState.BOTH)
			{
				SetObjectMaterial(SELECTION_ID_TAIL_LIGHT_R, material);
				SetObjectMaterial(SELECTION_ID_TAIL_LIGHT_L, material);
			}
		}
	}
	
	void TailLightsShineOff()
	{
		string material = ConfigGetString("TailReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_TAIL_LIGHT_L, material);
			SetObjectMaterial(SELECTION_ID_TAIL_LIGHT_R, material);
		}
	}
	
	void DashboardShineOn()
	{
		string material = ConfigGetString("dashboardMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_DASHBOARD_LIGHT, material);
		}
	}
	
	void DashboardShineOff()
	{
		string material = ConfigGetString("dashboardMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(SELECTION_ID_DASHBOARD_LIGHT, material);
		}
	}
	
	// Override this for a car-specific light type
	CarRearLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight(OffroadHatchbackFrontLight) ); // just a default value
	}
	
	// Override this for a car-specific light type
	CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(OffroadHatchbackFrontLight) ); // just a default value
	}
	
	protected void CheckVitalItem( bool isVital, string itemName )
	{
		if ( !isVital )
			return;

		EntityAI item = FindAttachmentBySlotName(itemName);

		if ( !item )
			EngineStop();
		else if ( item.IsRuined() )
			EngineStop();
	}

	protected void LeakFluid(CarFluid fluid)
	{
		float ammount = 0;
		
		switch (fluid)
		{
			case CarFluid.COOLANT:
				ammount = (1- m_RadiatorHealth) * Math.RandomFloat(0.02, 0.05);//CARS_LEAK_TICK_MIN; CARS_LEAK_TICK_MAX
				Leak(fluid, ammount);
			break;
			
			case CarFluid.OIL:
				ammount =  ( 1 - m_EngineHealth ) * Math.RandomFloat(0.02, 1.0);//CARS_LEAK_OIL_MIN; CARS_LEAK_OIL_MAX
				Leak(fluid, ammount);
			break;
			
			case CarFluid.FUEL:
				ammount = ( 1 - m_FuelTankHealth ) * Math.RandomFloat(0.02, 0.05);//CARS_LEAK_TICK_MIN; CARS_LEAK_TICK_MAX
				Leak(fluid, ammount);
			break;
		}
	}

	protected void CarPartsHealthCheck()
	{
		if ( GetGame().IsServer() )
		{
			ItemBase radiator;
			Class.CastTo( radiator, FindAttachmentBySlotName("CarRadiator"));
			if ( radiator )
			{
				m_RadiatorHealth = radiator.GetHealth01("", "");
			}
			else
			{
				m_RadiatorHealth = 0;
			}
			
			m_EngineHealth = GetHealth01("Engine", "");

			m_FuelTankHealth = GetHealth01("FuelTank", "");
		}
	}
	
	bool GetCrashLightSound()
	{
		return m_PlayCrashSoundLight;
	}
	
	void SynchCrashLightSound( bool play )
	{
		if (m_PlayCrashSoundLight != play)
		{
			m_PlayCrashSoundLight = play;
			SetSynchDirty();
		}
	}

	void PlayCrashLightSound()
	{
		PlaySound("offroad_hit_light_SoundSet", m_CrashSoundLight, m_PlayCrashSoundLight);
	}

	bool GetCrashHeavySound()
	{
		return m_PlayCrashSoundHeavy;
	}

	void SynchCrashHeavySound( bool play )
	{
		if (m_PlayCrashSoundHeavy != play)
		{
			m_PlayCrashSoundHeavy = play;
			SetSynchDirty();
		}
	}
	
	void PlayCrashHeavySound()
	{
		PlaySound("offroad_hit_heavy_SoundSet", m_CrashSoundHeavy, m_PlayCrashSoundHeavy);
	}
	
	void PlaySound(string soundset, ref EffectSound sound, out bool soundbool)
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			if (!sound)
			{
				sound =	SEffectManager.PlaySoundCachedParams(soundset, GetPosition());
			}
			else
			{
				if (!sound.IsSoundPlaying())
				{
					sound.SetPosition(GetPosition());
					sound.SoundPlay();
				}
			}
			
			soundbool = false;
		}
	}
	
	string GetAnimSourceFromSelection( string selection )
	{
		return "";
	}
	
	string GetDoorConditionPointFromSelection( string selection )
	{
		switch( selection )
		{
			case "seat_driver":
				return "seat_con_1_1";
			break;
			case "seat_codriver":
				return "seat_con_2_1";
			break;
			case "seat_cargo1":
				return "seat_con_1_2";
			break;
			case "seat_cargo2":
				return "seat_con_2_2";
			break;
		}
		
		return "";
	}
	
	string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		return "";
	}
	
	string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		return "";
	}
	
	int GetCrewIndex( string selection )
	{
		return -1;
	}

	override bool CanReachSeatFromDoors( string pSeatSelection, vector pFromPos, float pDistance = 1.0 )
	{
		string conPointName = GetDoorConditionPointFromSelection(pSeatSelection);
		if (conPointName.Length() > 0)
		{
			if ( MemoryPointExists(conPointName) )
			{
				vector conPointMS = GetMemoryPointPos(conPointName);
				vector conPointWS = ModelToWorld(conPointMS);
				
				//! skip the height for now
				conPointWS[1] = 0;
				pFromPos[1] = 0;
				
				if (vector.Distance(pFromPos, conPointWS) <= pDistance)
				{
					return true;
				}
			}
		}

		return false;		
	}

	bool IsVitalCarBattery()
	{
		return true;
	}
	
	bool IsVitalTruckBattery()
	{
		return true;
	}
	
	bool IsVitalSparkPlug()
	{
		return true;
	}
	
	bool IsVitalGlowPlug()
	{
		return true;
	}
	
	bool IsVitalEngineBelt()
	{
		return true;
	}
	
	bool IsVitalRadiator()
	{
		return true;
	}
	
	//!
	bool IsMoving()
	{
		if ( GetSpeedometer() > 3.5 )
			return true;
		
		return false;
	}

	//! camera type
	override int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;

	}

	int GetCarDoorsState( string slotType )
	{
		return -1;
	}

	string GetActionCompNameCoolant()
	{
		return "radiator";
	}

	float GetActionDistanceCoolant()
	{
		return 2.0;
	}

	string GetActionCompNameFuel()
	{
		return "refill";
	}

	float GetActionDistanceFuel()
	{
		return 1.5;
	}
	
	string GetActionCompNameOil()
	{
		//return "refill";
		return "carradiator";
	}

	float GetActionDistanceOil()
	{
		return 2.0;
	}
	
	string GetActionCompNameBrakes()
	{
		return "carradiator";
	}

	float GetActionDistanceBrakes()
	{
		return 2.0;
	}
	
	void InitializeActions()
	{
		m_InputActionMap = m_CarTypeActionsMap.Get( this.Type() );
		if (!m_InputActionMap)
		{
			TInputActionMap iam = new TInputActionMap;
			m_InputActionMap = iam;
			SetActions();
			m_CarTypeActionsMap.Insert(this.Type(), m_InputActionMap);
		}
	}
	
	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		if (!m_ActionsInitialize)
		{
			m_ActionsInitialize = true;
			InitializeActions();
		}
		
		actions = m_InputActionMap.Get(action_input_type);
	}
	
	void SetActions()
	{
		//AddAction(ActionAnimateCarSelection); not needed now
		AddAction(ActionOpenCarDoorsOutside);
		AddAction(ActionCloseCarDoorsOutside);
		AddAction(ActionGetInTransport);
		AddAction(ActionSwitchLights);
	}
	
	void AddAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);

		if (!action)
		{
			Debug.LogError("Action " + actionName + " dosn't exist!");
			return;
		}		
		
		typename ai = action.GetInputType();
		if (!ai)
		{
			m_ActionsInitialize = false;
			return;
		}
		ref array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if (!action_array)
		{
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		
		if ( LogManager.IsActionLogEnable() )
		{
			Debug.ActionLog(action.ToString() + " -> " + ai, this.ToString() , "n/a", "Add action" );
		}
		action_array.Insert(action);
	}
	
	void RemoveAction(typename actionName)
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		ActionBase action = player.GetActionManager().GetAction(actionName);
		typename ai = action.GetInputType();
		ref array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if (action_array)
		{
			action_array.RemoveItem(action);
		}
	}
	
	override bool IsInventoryVisible()
	{
		return ( GetGame().GetPlayer() && ( !GetGame().GetPlayer().GetCommand_Vehicle() || GetGame().GetPlayer().GetCommand_Vehicle().GetTransport() == this ) );
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel,newLevel,zone);
		
		if ( newLevel ==  GameConstants.STATE_RUINED )
		{
			bool dummy;
			switch ( zone )
			{
				case "WindowLR":
				case "WindowRR":
					if (m_Initialized)
					{
						PlaySound("offroad_hit_window_small_SoundSet", m_WindowSmall, dummy);
					}
				break;
				
				case "WindowFront":
				case "WindowBack":
				case "WindowFrontLeft":
				case "WindowFrontRight":
					if (m_Initialized)
					{
						PlaySound("offroad_hit_window_large_SoundSet", m_WindowLarge, dummy);
					}
				break;
				case "Engine":
					#ifndef SERVER
					CreateCarDestroyedEffect();
					#endif
				break;
			}
		}
	}
	
	override void EEOnCECreate()
	{

		float maxVolume = GetFluidCapacity( CarFluid.FUEL );
		float amount = Math.RandomFloat(0.0, maxVolume * 0.35 );

		Fill( CarFluid.FUEL, amount );
	}
	
	void ForceUpdateLightsStart()
	{
		if (!m_ForceUpdateLights)
		{
			m_ForceUpdateLights = true;
			SetSynchDirty();
		}
	}
	
	void ForceUpdateLightsEnd()
	{
		if (m_ForceUpdateLights)
		{
			m_ForceUpdateLights = false;
			SetSynchDirty();
		}
	}
	
	//Get the engine start battery consumption
	float GetBatteryConsumption()
	{
		return m_BatteryConsume;
	}
	
	float GetBatteryRuntimeConsumption()
	{
		return m_BatteryContinuousConsume;
	}
	
	float GetBatteryRechargeRate()
	{
		return -m_BatteryRecharge;
	}
	
	ItemBase GetBattery()
	{
		if ( IsVitalCarBattery() ) 
			return ItemBase.Cast( FindAttachmentBySlotName("CarBattery") );
		else if ( IsVitalTruckBattery() ) 
			return ItemBase.Cast( FindAttachmentBySlotName("TruckBattery") );
		
		return null;
	}
};
