class PortableGasStove extends ItemBase
{
	StoveLight	m_Light;
	
	protected const string FLAME_BUTANE_ON 			= "dz\\gear\\cooking\\data\\flame_butane_ca.paa";
	protected const string FLAME_BUTANE_OFF 		= "";
	typename ATTACHMENT_COOKING_POT 				= Pot;
	typename ATTACHMENT_FRYING_PAN 					= FryingPan;
	typename ATTACHMENT_CAULDRON	 				= Cauldron;
	
	//cooking
	protected const float PARAM_COOKING_TEMP_THRESHOLD			= 100;		//temperature threshold for starting coooking process (degree Celsius)
	protected const float PARAM_COOKING_EQUIP_TEMP_INCREASE		= 10;		//how much will temperature increase when attached on burning fireplace (degree Celsius)
	protected const float PARAM_COOKING_EQUIP_MAX_TEMP			= 250;		//maximum temperature of attached cooking equipment (degree Celsius)
	protected const float PARAM_COOKING_TIME_INC_COEF			= 0.5;		//cooking time increase coeficient, can be used when balancing how fast a food can be cooked
	
	private 		float m_TimeFactor;
	//
	ref Cooking m_CookingProcess;
	ItemBase m_CookingEquipment;
	
	//sound
	const string SOUND_BURNING 		= "portablegasstove_burn_SoundSet";
	const string SOUND_TURN_ON		= "portablegasstove_turn_on_SoundSet";
	const string SOUND_TURN_OFF		= "portablegasstove_turn_off_SoundSet";
	
	protected EffectSound m_SoundBurningLoop;
	protected EffectSound m_SoundTurnOn;
	protected EffectSound m_SoundTurnOff;
	
	//cooking equipment
	ItemBase GetCookingEquipment()
	{
		return m_CookingEquipment;
	}
	
	void SetCookingEquipment( ItemBase equipment )
	{
		m_CookingEquipment = equipment;
	}
	
	void ClearCookingEquipment()
	{
		SetCookingEquipment( NULL );
	}

	//Destroy
	void DestroyFireplace()
	{
		//delete object
		GetGame().ObjectDelete( this );
	}	
	
	//--- ATTACHMENTS
	override void EEItemAttached( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );
		
		//cookware
		if ( ( item.Type() == ATTACHMENT_CAULDRON ) || ( item.Type() == ATTACHMENT_COOKING_POT ) || ( item.Type() == ATTACHMENT_FRYING_PAN ) )
		{
			ItemBase item_base = ItemBase.Cast( item );
			SetCookingEquipment( item_base );
			
			RefreshFlameVisual(m_EM.IsSwitchedOn(), true);
		}
	}
	
	override void EEItemDetached( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached( item, slot_name );
		
		//cookware
		if ( ( item.Type() == ATTACHMENT_CAULDRON ) || ( item.Type() == ATTACHMENT_COOKING_POT ) || ( item.Type() == ATTACHMENT_FRYING_PAN ) )
		{
			//stop steam particle
			RemoveCookingAudioVisuals();

			//remove cooking equipment reference
			ClearCookingEquipment();
			
			RefreshFlameVisual(m_EM.IsSwitchedOn(), false);
		}
	}
	
	//--- POWER EVENTS
	override void OnSwitchOn()
	{
		super.OnSwitchOn();
		
		//sound (client only)
		SoundTurnOn();
	}

	override void OnSwitchOff()
	{
		super.OnSwitchOff();
		
		//sound (client only)
		SoundTurnOff();
	}
	
	override void OnWorkStart()
	{
		super.OnWorkStart();

		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() ) // client side
		{
			m_Light = StoveLight.Cast( ScriptedLightBase.CreateLight( StoveLight, "0 0 0") );
			m_Light.AttachOnMemoryPoint(this, "light");
		}
		
		//refresh visual
		RefreshFlameVisual(true, GetCookingEquipment() != null);
		
		//sound (client only)
		SoundBurningStart();
	}

	override void OnWorkStop()
	{
		if ( m_Light )
			m_Light.FadeOut();

		//refresh visual
		RefreshFlameVisual(false, false);
		
		//stop steam particle
		RemoveCookingAudioVisuals();
		
		//sound (client only)
		SoundBurningStop();
	}
	
	//on update 
	override void OnWork( float consumed_energy )
	{
		//manage cooking equipment
		if ( GetCookingEquipment() )
		{
			if ( GetGame() && GetGame().IsServer() )
			{
				float cook_equip_temp = GetCookingEquipment().GetTemperature();
				
				//start cooking
				if ( cook_equip_temp >= PARAM_COOKING_TEMP_THRESHOLD )
				{
					m_TimeFactor = consumed_energy;
					CookWithEquipment();
				}				
				
				//set temperature to cooking equipment
				cook_equip_temp = cook_equip_temp + (PARAM_COOKING_EQUIP_TEMP_INCREASE * consumed_energy);
				cook_equip_temp = Math.Clamp ( cook_equip_temp, 0, PARAM_COOKING_EQUIP_MAX_TEMP );
				GetCookingEquipment().SetTemperature( cook_equip_temp );
			}
		}
	}
	
	void CookWithEquipment()
	{
		if ( m_CookingProcess == NULL )
		{
			m_CookingProcess = new Cooking();
		}
		
		m_CookingProcess.CookWithEquipment ( GetCookingEquipment(), PARAM_COOKING_TIME_INC_COEF * m_TimeFactor );
	}

	protected void RefreshFlameVisual(bool working = false, bool hasAttachment = false)
	{
		if (!working)
		{
			SetObjectTexture( 0, FLAME_BUTANE_OFF );
			SetObjectTexture( 1, FLAME_BUTANE_OFF );

			return;
		}
		
		if ( !hasAttachment )
		{
			//! full size flame selection
			SetObjectTexture( 0, FLAME_BUTANE_ON );
			SetObjectTexture( 1, FLAME_BUTANE_OFF );
			
			return;
		}
		else
		{
			//! shrinked flame selection
			SetObjectTexture( 0, FLAME_BUTANE_OFF );
			SetObjectTexture( 1, FLAME_BUTANE_ON );
			
			return;
		}
	}

	//================================================================
	// PARTICLES
	//================================================================	
	//cooking equipment steam
	protected void RemoveCookingAudioVisuals()
	{
		if ( GetCookingEquipment() )
		{
			if ( ( GetCookingEquipment().Type() == ATTACHMENT_CAULDRON ) || ( GetCookingEquipment().Type() == ATTACHMENT_COOKING_POT ) )
			{
				Bottle_Base cooking_pot = Bottle_Base.Cast( GetCookingEquipment() );
				cooking_pot.RemoveAudioVisualsOnClient();
			}

			if ( GetCookingEquipment().Type() == ATTACHMENT_FRYING_PAN ) 
			{
				FryingPan frying_pan = FryingPan.Cast( GetCookingEquipment() );
				frying_pan.RemoveAudioVisualsOnClient();
			}
		}
	}
	
	//================================================================
	// SOUNDS
	//================================================================
	protected void SoundBurningStart()
	{
		PlaySoundSetLoop( m_SoundBurningLoop, SOUND_BURNING, 0.1, 0.3 );
	}
	
	protected void SoundBurningStop()
	{
		StopSoundSet( m_SoundBurningLoop );
	}	

	protected void SoundTurnOn()
	{
		PlaySoundSet( m_SoundTurnOn, SOUND_TURN_ON, 0.1, 0.1 );
	}
	
	protected void SoundTurnOff()
	{
		PlaySoundSet( m_SoundTurnOff, SOUND_TURN_OFF, 0.1, 0.1 );
	}		
		
	//================================================================
	// CONDITIONS
	//================================================================
	//this into/outo parent.Cargo
	override bool CanPutInCargo( EntityAI parent )
	{
		if ( !super.CanPutInCargo(parent) ) {return false;}
		if ( GetCompEM().IsSwitchedOn() )
		{
			return false;
		}
		
		return true;
	}

	override bool CanRemoveFromCargo( EntityAI parent )
	{
		return true;
	}
	
	//hands
	override bool CanPutIntoHands( EntityAI parent )
	{
		if ( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		if ( GetCompEM().IsSwitchedOn() )
		{
			return false;
		}
		
		return true;
	}	
	
	//================================================================
	// ITEM-TO-ITEM FIRE DISTRIBUTION
	//================================================================
	
	override bool IsIgnited()
	{
		return GetCompEM().IsWorking();
	}
	
	override bool CanIgniteItem(EntityAI ignite_target = NULL)
	{
		return GetCompEM().IsWorking();
	}
	
	override void SetActions()
	{
		super.SetActions();
		

		AddAction(ActionLightItemOnFire);
		AddAction(ActionTurnOnWhileOnGround);
		AddAction(ActionTurnOffWhileOnGround);
	}
	
	//Debug menu Spawn Ground Special
	override void OnDebugSpawn()
	{
		EntityAI entity;
		if ( Class.CastTo(entity, this) )
		{
			GetInventory().CreateInInventory( "LargeGasCanister" );
			GetInventory().CreateInInventory( "Pot" );

			SpawnEntityOnGroundPos("WaterBottle", entity.GetPosition() + Vector(0.2, 0, 0) );
		}
	}
}