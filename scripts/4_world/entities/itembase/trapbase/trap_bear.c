class BearTrap extends TrapBase
{
	// Raycasts start positions:
	// Positions are local to model. Vertical offset prevents ground collision.
	static const vector m_RaycastSources[5] = {
		"0.0 0.1 0.0",	// center
		"0.2 0.1 0.2",	// north east
		"-.2 0.1 0.2",	// north west
		"0.2 0.1 -0.2",	// south east
		"-0.2 0.1 -0.2"	// south west
	};
	
	void BearTrap()
	{
		m_DamagePlayers 			= 5; 				// How much damage player gets when caught
		m_DamageOthers 				= 5;         		// How much damage other entities(CreatureAI) gets when caught
		m_DefectRate 				= 0;
		m_InitWaitTime 				= 0; 				// After this time after deployment, the trap is activated
		m_AnimationPhaseGrounded 	= "placing";
		m_AnimationPhaseSet 		= "BearTrap_Set";
		m_AnimationPhaseTriggered 	= "placing";
	}
	
	override void CreateTrigger()
	{
		m_TrapTrigger	= TrapTrigger.Cast(GetGame().CreateObject("TrapTrigger", GetPosition(), false));
		vector mins		= "-0.1 -0.05 -0.1";
		vector maxs 	= "0.1 0.2 0.1";

		m_TrapTrigger.SetOrientation(GetOrientation());
		m_TrapTrigger.SetExtents(mins, maxs);	
		m_TrapTrigger.SetParentObject(this);
	}
	
	override void OnSteppedOn(EntityAI victim)
	{
		if (GetGame().IsServer() && victim)
		{
			int i;

			//! CarScript specific reaction on BearTrap
			if (victim.IsInherited(CarScript))
			{
				vector trapPosXZ = GetPosition();
				trapPosXZ[1] = 0;

				GameInventory inv = victim.GetInventory();
				for (i = 0; i < inv.AttachmentCount(); i++)
				{
					EntityAI ent = inv.GetAttachmentFromIndex(i);
					if (ent && ent.IsInherited(CarWheel))
					{
						vector entPosXZ = ent.GetPosition();
						entPosXZ[1] = 0;
						if (vector.Distance(trapPosXZ, entPosXZ) < 1)
						{
							OnServerSteppedOn(ent, "");
							return;
						}
					}
				}
			}
			else
			{
				for (i = 0; i < 5; ++i)
				{
					vector raycast_start_pos 			= ModelToWorld(m_RaycastSources[i]);
					vector raycast_end_pos 				= "0 0.5 0" + raycast_start_pos;
					
					RaycastRVParams rayInput 			= new RaycastRVParams(raycast_start_pos, raycast_end_pos, this);
					rayInput.flags 						= CollisionFlags.ALLOBJECTS;
					rayInput.type 						= ObjIntersectFire;
					rayInput.radius 					= 0.05;
					array<ref RaycastRVResult> results 	= new array<ref RaycastRVResult>;
			
					if (DayZPhysics.RaycastRVProxy(rayInput, results))
					{
						RaycastRVResult res;
						for (int j = 0; j < results.Count(); j++)
						{
							Object contact_obj = results[j].obj;
							if (contact_obj)
							{
								OnServerSteppedOn(contact_obj, contact_obj.GetDamageZoneNameByComponentIndex(results[j].component));
								break;
							}
						}
						
						return;
					}
				}
					
				// Damage random leg since we don't know what part of player's body was caught in the trap.
				string damageZoneRand = "LeftLeg";
				if (Math.RandomIntInclusive(0, 1) == 1)
				{
					damageZoneRand = "RightLeg";
				}
	
				OnServerSteppedOn(victim, damageZoneRand);
			}
		}
		else if (!GetGame().IsDedicatedServer())
		{
			if (victim)
			{
				victim.SpawnDamageDealtEffect();
				PlaySoundBiteLeg();
			}
			else if (victim.IsInherited(DayZCreatureAI))
			{
				PlaySoundBiteLeg();
			}
			else
			{
				PlaySoundBiteEmpty();
			}
		}
	}
	
	void OnServerSteppedOn(Object obj, string damageZone)
	{
		if (obj.IsInherited(CarWheel))
		{
			obj.ProcessDirectDamage(DT_CLOSE_COMBAT, this, damageZone, "BearTrapHit_CarWheel", "0 0 0", 1);
			return;
		}

		//! PlayerBase specific handling
		if (obj.IsInherited(PlayerBase))
		{
			CauseVictimToStartLimping(obj, damageZone);
		}
		
		//! DayZInfected specific handling
		if (obj.IsInherited(DayZInfected))
		{
			DayZInfected victim = DayZInfected.Cast(obj);
			if (victim &&  Math.RandomIntInclusive(0, 1) == 1)
			{
				victim.StartCommand_Crawl(0); // switch to crawl anim
			}
		}
		
		obj.ProcessDirectDamage(DT_CLOSE_COMBAT, this, damageZone, "BearTrapHit", "0 0 0", 1);
	}
	
	// Causes the player to start limping. This is temporary and should at some point be replaced by broken legs
	void CauseVictimToStartLimping(Object obj, string damagedZone)
	{
		PlayerBase victim = PlayerBase.Cast(obj);
		if (victim)
		{
			victim.DamageAllLegs(victim.GetMaxHealth()); //deal 100% damage to break legs
		}
	}
		
	void PlaySoundBiteLeg()
	{
		EffectSound sound = SEffectManager.PlaySound("beartrapCloseDamage_SoundSet", GetPosition(), 0, 0, false);
		sound.SetAutodestroy(true);
	}
	
	void PlaySoundBiteEmpty()
	{
		EffectSound sound = SEffectManager.PlaySound("beartrapClose_SoundSet", GetPosition(), 0, 0, false);
		sound.SetAutodestroy(true);
	}
	
	void PlaySoundOpen()
	{
		EffectSound sound = SEffectManager.PlaySound("beartrapOpen_SoundSet", GetPosition(), 0, 0, false);
		sound.SetAutodestroy(true);
	}

	override void OnActivate()
	{
		if (!GetGame().IsDedicatedServer() && GetGame().GetPlayer())
		{
			PlaySoundOpen();
		}
	}
	
	//================================================================
	// ADVANCED PLACEMENT
	//================================================================
	
	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);
		
		if (GetGame().IsServer())
		{
			PlayerBase player_PB = PlayerBase.Cast(player);
			StartActivate(player_PB);
			
			m_TrapTrigger.SetPosition(position);
			m_TrapTrigger.SetOrientation(orientation);
		}	
	}
	
	override bool IsDeployable()
	{
		return true;
	}
	
	override string GetLoopDeploySoundset()
	{
		return "beartrap_deploy_SoundSet";
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionClapBearTrapWithThisItem);
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
	}
}