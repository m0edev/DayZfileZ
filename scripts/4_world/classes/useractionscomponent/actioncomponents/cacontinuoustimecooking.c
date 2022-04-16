class CAContinuousTimeCooking : CAContinuousBase
{
	protected float 			m_LocalTimeElpased;
	protected float				m_TimeElapsed;
	protected float 			m_TimeElapsedRepeat;
	protected ref Param1<float>	m_SpentUnits;
	
	protected float 			m_TimeToCook;
	protected float 			m_AdjustedTimeToCook;
	protected Cooking			m_CookingProcess;
	protected Edible_Base		m_ItemToCook;
	protected float 			m_CookingUpdateTime;
	protected float 			m_AdjustedCookingUpdateTime;
	
	void CAContinuousTimeCooking( float cooking_update_time )
	{
		m_CookingUpdateTime = cooking_update_time;
	}
	
	override void Setup( ActionData action_data )
	{
		m_TimeElapsedRepeat = 0;
		
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		
		//fireplace & cooking process
		FireplaceBase fireplace = FireplaceBase.Cast( action_data.m_Target.GetObject() );
		m_CookingProcess = fireplace.GetCookingProcess();
		m_ItemToCook = m_CookingProcess.GetFoodOnStick( action_data.m_MainItem );
		m_TimeToCook = m_CookingProcess.GetTimeToCook( m_ItemToCook, CookingMethodType.BAKING );
		
		//update elapsed time
		m_TimeElapsed = m_ItemToCook.GetFoodStage().GetCookingTime();
		
		//SoftSkill implementation to hasten Cooking
		m_AdjustedCookingUpdateTime = action_data.m_Player.GetSoftSkillsManager().SubtractSpecialtyBonus( m_CookingUpdateTime, m_Action.GetSpecialtyWeight(), true);
		m_AdjustedTimeToCook = action_data.m_Player.GetSoftSkillsManager().SubtractSpecialtyBonus( m_TimeToCook, m_Action.GetSpecialtyWeight(), true);
	}
	
	override int Execute( ActionData action_data )
	{
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		int cooking_state_update = 0;
		float cooking_time = m_ItemToCook.GetFoodStage().GetCookingTime();
		if ( m_TimeToCook > cooking_time )
		{
			m_TimeElapsed += action_data.m_Player.GetDeltaT();
			m_TimeElapsedRepeat += ( action_data.m_Player.GetDeltaT() );
			
			if ( m_TimeElapsedRepeat >= m_AdjustedCookingUpdateTime )
			{
				cooking_state_update = m_CookingProcess.CookOnStick( m_ItemToCook, m_CookingUpdateTime );
				//Print("cooking_state_update: "+ cooking_state_update);
				//refresh action when food changed state
				if ( cooking_state_update == 1 )
				{
					Setup(action_data);
					
					if ( GetGame().IsServer() )
					{
						MiscGameplayFunctions.DealAbsoluteDmg(action_data.m_MainItem, 30);
						action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_Action.GetSpecialtyWeight() );
					}
					else
					{
						if ( m_ItemToCook.IsMeat() && m_ItemToCook.GetFoodStage().GetFoodStageType() == FoodStageType.BAKED )
						{
							GetGame().GetAnalyticsClient().OnActionCookedSteak();
						}
					}
				}
			
				m_TimeElapsedRepeat = 0;
			}				
					
			return UA_PROCESSING;
		}
		else
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = m_TimeElapsed;
				SetACData(m_SpentUnits);
			}
			OnCompletePogress(action_data);
			return UA_FINISHED;
		}
	}
	
	override float GetProgress()
	{			
		if ( m_AdjustedTimeToCook != 0 )
		{
			return m_TimeElapsed/m_AdjustedTimeToCook;
		}

		return 1; 
	}
};