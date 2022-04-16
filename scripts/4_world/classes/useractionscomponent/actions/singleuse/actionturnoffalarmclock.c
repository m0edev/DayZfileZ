class ActionTurnOffAlarmClockCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CASingleUseTurnOnPlugged;
	}
}

class ActionTurnOffAlarmClock: ActionTurnOffWhileInHands
{
	void ActionTurnOffAlarmClock()
	{
		m_CallbackClass = ActionTurnOffAlarmClockCB;
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_STOP_ALARM;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_STOP_ALARM;
	}

	override string GetText()
	{
		return "#turn_off";
	}
	
	override bool HasProneException()
	{
		return true;
	}
		
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		AlarmClock_ColorBase alarm = AlarmClock_ColorBase.Cast(item);
		return (alarm.IsAlarmOn() || alarm.IsRinging());
	}
		
	override void OnExecuteClient( ActionData action_data )
	{
		AlarmClock_ColorBase alarm = AlarmClock_ColorBase.Cast(action_data.m_MainItem);
		if (alarm)
 			alarm.TurnOffClient();
	}
	
	
	override void OnExecuteServer( ActionData action_data )
	{
		AlarmClock_ColorBase alarm = AlarmClock_ColorBase.Cast(action_data.m_MainItem);
		if (alarm)
			alarm.TurnOff();
	}
}
