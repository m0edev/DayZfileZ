enum EAlarmClockState
{
	UNSET,
	SET,
	RINGING,
	//-----------
	COUNT
}

class AlarmClock_ColorBase: Inventory_Base
{
	float 				m_AlarmTime01;
	float				m_RingingDuration;
	int 				m_State = EAlarmClockState.UNSET;
	int 				m_StatePrev = EAlarmClockState.UNSET;
	ref Timer 			m_TimerUpdate;
	static ref NoiseParams 	m_NoisePar;
	static NoiseSystem		m_NoiseSystem;
	EffectSound 		m_RingingSoundLoop;
	EffectSound 		m_TurnOnSound;
	EffectSound 		m_DestoryedSound;
	EffectSound 		m_HitSound;
	
	const string		RINGING_SOUND 				= "AlarmClock_Ring_Loop_SoundSet";
	const string		TURN_TOGGLE_SOUND 			= "AlarmClock_Turn_Off_SoundSet";
	const string		DESTROYED_SOUND 			= "AlarmClock_Destroyed_SoundSet";
	const string		HIT_SOUND 					= "AlarmClock_Hit_SoundSet";

	const float 		UPDATE_TICK_RATE 			= 1; // Clock update tick frequency
	const float 		RINGING_DURATION_MAX 		= 60;//in secs, at or past this value, the clock stops ringing
	
	
	void AlarmClock_ColorBase()
	{
		RegisterNetSyncVariableFloat("m_AlarmTime01");
		RegisterNetSyncVariableInt("m_State",0, EAlarmClockState.COUNT - 1);
		
		if ( GetGame().IsServer() )
		{
			m_NoiseSystem = GetGame().GetNoiseSystem();
			if ( m_NoiseSystem && !m_NoisePar )
			{
				// Create and load noise parameters
				m_NoisePar = new NoiseParams;
				m_NoisePar.LoadFromPath("cfgVehicles " + GetType() + " NoiseAlarmClock");
			}
		}
	}
	
	void ~AlarmClock_ColorBase()
	{
		OnRingingStopClient();
	}
	
	override void GetDebugButtonNames(out string button1, out string button2, out string button3, out string button4)
	{
		button1 = "SetAlarmAhead1Min";
	}
	
	override void OnDebugButtonPressServer(int button_index)
	{
		SetAlarmInXMins(1);
	}
	
	float GetRingingDurationMax()
	{
		return RINGING_DURATION_MAX;
	}
	
	override string GetDebugText()
	{
		string debug_output;
		
		if( GetGame().IsDedicatedServer())
		{
			debug_output = "alarm in: " + GetAlarmInMin().ToString() + " mins" + "\n";
			debug_output +=  "current state: " +  typename.EnumToString(EAlarmClockState, m_State);
		}
		else
		{
			debug_output = "this is client";
		}
		return debug_output;
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		TurnOff();
	}
	
	override void EEHitByRemote(int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos)
	{
		super.EEHitByRemote(damageType, source, component, dmgZone, ammo, modelPos);
		PlaySoundSet( m_HitSound, HIT_SOUND, 0, 0 );
	}
	
	override void OnDamageDestroyed(int oldLevel)
	{
		super.OnDamageDestroyed(oldLevel);
		if(GetGame().IsClient() && oldLevel != -1)
			PlaySoundSet( m_DestoryedSound, DESTROYED_SOUND, 0, 0 );
	}
	
	
	protected void SetState(EAlarmClockState state)
	{
		m_State = state;
		SetSynchDirty();
	}

	protected void Disarm()
	{
		SetState(EAlarmClockState.UNSET);
	}
	
	protected void Arm()
	{
		SetupTimerServer();
		SetState(EAlarmClockState.SET);
	}
	
	void OnUpdate()
	{
		if ( IsAlarmOn() )
		{
			//due to variable server time flow(day-night time accel), it's not possible to simply set a timer for X secs without some convoluted math/code, so we need to check at regular intervals
			int alarm_hand_in_minutes = ConvertAlarmHand01ToMins12h(m_AlarmTime01);
		
			int pass, hour, minute;
			GetGame().GetWorld().GetDate(pass, pass, pass, hour, minute);
			
			int curr_time_in_minutes = ConvertTimeToMins12h(hour, minute);
			
			//Print(GetAlarmInMin());
			
			if ( alarm_hand_in_minutes == curr_time_in_minutes )
			{
				MakeRingingStart();
			}
		}
		
		if ( IsRinging())
		{
			m_RingingDuration += UPDATE_TICK_RATE;
			
			if (m_RingingDuration >= GetRingingDurationMax())
			{
				TurnOff();
			}
			else if ( m_NoiseSystem )
			{
				m_NoiseSystem.AddNoiseTarget( GetPosition(), UPDATE_TICK_RATE, m_NoisePar);
			}
		}
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionSetAlarmClock);
		AddAction(ActionTurnOnAlarmClock);
		AddAction(ActionTurnOffAlarmClock);
	}
	
	protected void SetupTimerServer()
	{
		m_TimerUpdate = new Timer;
		m_TimerUpdate.Run(UPDATE_TICK_RATE , this, "OnUpdate", null, true);

	}

	protected void OnRingingStartClient()
	{
		PlaySoundSetLoop( m_RingingSoundLoop, RINGING_SOUND, 0, 0 );
	}
	
	protected void OnRingingStopClient()
	{
		//StopSoundSet(m_RingingSoundLoop);
		
		SEffectManager.DestroyEffect(m_RingingSoundLoop);
	}

	protected void MakeRingingStart()
	{
		if(!m_TimerUpdate)
			SetupTimerServer();
		SetState(EAlarmClockState.RINGING);
	}
	
	protected void MakeRingingStop()
	{
		m_RingingDuration = 0;
		SetState(EAlarmClockState.UNSET);
	}

	protected void AnimateAlarmHand(float value)
	{
		SetAnimationPhaseNow("ClockAlarm", value);
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		AnimateAlarmHand(m_AlarmTime01);
		
		if (m_State != m_StatePrev)//state changed
		{
			if (m_StatePrev == EAlarmClockState.RINGING)
			{
				OnRingingStopClient();
			}
			else if (m_State == EAlarmClockState.RINGING)
			{
				OnRingingStartClient();
			}
			
			m_StatePrev = m_State;
		}
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;
		
		if (version < 126)
		{
			return true;
		}

		EAlarmClockState state;		
		
		if ( !ctx.Read( state ) )
		{
			return false;
		}

		float time;
		
		if ( !ctx.Read( time ) )
		{
			return false;
		}

		SetAlarmTimeServer(time);
		
		if ( state == EAlarmClockState.SET )
		{
			TurnOn();
		}
		else if (state == EAlarmClockState.RINGING )
		{
			MakeRingingStart();
		}
		
		return true;
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		ctx.Write(m_State);
		ctx.Write(m_AlarmTime01);
	}
	
	protected int GetAlarmInMin()
	{
		int alarm_hand_in_mins = ConvertAlarmHand01ToMins12h(m_AlarmTime01);
		
		int pass, hour, minute;
		GetGame().GetWorld().GetDate(pass, pass, pass, hour, minute);
		
		int curr_time_in_minutes = ConvertTimeToMins12h(hour, minute);
		int ring_in_mins = GetTimeDiffInMins12h(curr_time_in_minutes, alarm_hand_in_mins);
		return ring_in_mins;
	}
	
	
	static int ConvertAlarmHand01ToMins12h(float time01)
	{
		return Math.Lerp(0,12*60,time01);
	}
	
	static float ConvertMins12hToAlarmHand01(int mins)
	{
		return Math.InverseLerp(0,12*60,mins);
	}
	
	static int ConvertTimeToMins12h(int hour, int minute)
	{
		if (hour >= 12)
			hour -= 12;  
		return hour * 60 + minute;
	}
	
	static int GetTimeDiffInMins12h(int from_mins, int to_mins)
	{
		if (to_mins > from_mins)
		{
			return to_mins - from_mins;
		}
		else if (to_mins < from_mins)
		{
			return ((12 * 60) - from_mins) + to_mins;
		}
		else return 0;
	}
	
	void SetAlarmInXMins(int in_mins)
	{
		int pass, hour, minute;
		GetGame().GetWorld().GetDate(pass, pass, pass, hour, minute);
		int mins12h = ConvertTimeToMins12h(hour, minute) + in_mins;
		m_AlarmTime01 = ConvertMins12hToAlarmHand01(mins12h);
		Arm();
	}
	
	override void OnDebugSpawn()
	{
		TurnOn();
		MakeRingingStart();
	}
	
	//---------------------------------------------------------------------------------------------------------
	//---------------------------------------------- Public methods -------------------------------------------
	//---------------------------------------------------------------------------------------------------------
	
	bool IsRinging()
	{
		return (m_State == EAlarmClockState.RINGING);
	}
	
	bool IsAlarmOn()
	{
		return (m_State == EAlarmClockState.SET);
	}
	
	void TurnOn()
	{
		Arm();
	}
	
	void TurnOnClient()
	{
		PlaySoundSet( m_TurnOnSound, TURN_TOGGLE_SOUND, 0, 0 );
	}
	
	void TurnOffClient()
	{
		PlaySoundSet( m_TurnOnSound, TURN_TOGGLE_SOUND, 0, 0 );
	}
	
	
	void TurnOff()
	{
		if ( IsRinging() )
		{
			MakeRingingStop();
		}
		else
		{
			Disarm();
		}
		
		m_TimerUpdate = null;
	}

	void SetAlarmTimeServer(float time01)
	{
		m_AlarmTime01 = time01;
		SetSynchDirty();
	}
	
};
	
class AlarmClock_Red : AlarmClock_ColorBase {};
class AlarmClock_Blue : AlarmClock_ColorBase {};
class AlarmClock_Green : AlarmClock_ColorBase {};