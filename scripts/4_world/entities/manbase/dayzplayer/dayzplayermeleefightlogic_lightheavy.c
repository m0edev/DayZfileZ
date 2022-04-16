//! Light / Heavy punches

enum EFightLogicCooldownCategory
{
	EVADE = 0
}

class DayZPlayerMeleeFightLogic_LightHeavy
{
	//! cooldown timers settings
	const float EVADE_COOLDOWN			= 0.5;
	
	//
	const float CLOSE_TARGET_DISTANCE	= 1.5;
	
	//! dummy ammo types 
	const string DUMMY_LIGHT_AMMO		= "Dummy_Light";
	const string DUMMY_HEAVY_AMMO		= "Dummy_Heavy";

	protected DayZPlayerImplement					m_DZPlayer;
	protected ref DayZPlayerImplementMeleeCombat	m_MeleeCombat;
	protected EMeleeHitType							m_HitType;
	protected ref map <int, ref Timer>				m_CooldownTimers;
	protected Mission 								m_Mission;

	protected bool									m_IsInBlock;
	protected bool									m_IsEvading;
	protected bool 									m_IsFinisher;
	
	int 									m_DbgForcedFinisherType = -1;

	void DayZPlayerMeleeFightLogic_LightHeavy(DayZPlayerImplement player)
	{
		Init(player);
	}
	
	void Init(DayZPlayerImplement player)
	{
		m_DZPlayer 			= player;
		m_MeleeCombat		= m_DZPlayer.GetMeleeCombat();
		m_Mission			= GetGame().GetMission();

		m_IsInBlock 		= false;
		m_IsEvading	 		= false;
		m_IsFinisher 		= false;
		m_HitType			= EMeleeHitType.NONE;
		
		RegisterCooldowns();
	}

	void ~DayZPlayerMeleeFightLogic_LightHeavy() {}

	bool IsInBlock()
	{
		return m_IsInBlock;
	}
	
	void SetBlock( bool block )
	{
		m_IsInBlock = block;
	}
	
	bool IsEvading()
	{
		return m_IsEvading;
	}
	
	bool GetCurrentHitType()
	{
		return m_HitType;
	}

	protected void RegisterCooldowns()
	{
		m_CooldownTimers	= new map<int, ref Timer>;
		m_CooldownTimers.Insert(EFightLogicCooldownCategory.EVADE, new Timer(CALL_CATEGORY_SYSTEM)); // evades
	}

	protected EMeleeHitType GetAttackTypeFromInputs(HumanInputController pInputs)
	{
		if (pInputs.IsMeleeFastAttackModifier() && m_DZPlayer.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
			return EMeleeHitType.HEAVY;
		else
			return EMeleeHitType.LIGHT;
	}
	
	protected EMeleeHitType GetAttackTypeByWeaponAttachments(EntityAI pEntity)
	{
		if ( !m_DZPlayer.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY) )
			return EMeleeHitType.NONE;

		//! use stabbing if the bayonet/knife is attached to firearm
		if ( pEntity.HasBayonetAttached() )
			return EMeleeHitType.WPN_STAB;
		else if ( pEntity.HasButtstockAttached() )
			return EMeleeHitType.WPN_HIT_BUTTSTOCK;
		else
			return EMeleeHitType.WPN_HIT;
	}
	
	protected float GetAttackTypeByDistanceToTarget(EntityAI pTarget, EMeleeTargetType pTargetType = EMeleeTargetType.ALIGNABLE)
	{
		if ( pTargetType == EMeleeTargetType.NONALIGNABLE )
			return 1.0;

		//! target in short distance - in place attack
		float dist2 = Math.SqrFloat(CLOSE_TARGET_DISTANCE);
		if ( pTarget && (vector.DistanceSq(m_DZPlayer.GetPosition(), m_MeleeCombat.GetHitPos()) <= dist2 || vector.DistanceSq(m_DZPlayer.GetBonePositionWS(m_DZPlayer.GetBoneIndexByName("Head")), m_MeleeCombat.GetHitPos()) <= dist2) )
			return 1.0;
				
		//! target is far from the player - forward movement attack
		return 0.0;
	}

	bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{		
		InventoryItem itemInHands;
		HumanCommandMove hcm = m_DZPlayer.GetCommand_Move();

		itemInHands = InventoryItem.Cast(pEntityInHands);
		
		PlayerBase player = PlayerBase.Cast(m_DZPlayer);
				
		bool isFireWeapon = itemInHands && itemInHands.IsWeapon();
		
		//! Check if we need to damage
		HandleHitEvent(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
		
		//! Actually pressing a button to start a melee attack
		if ((pInputs.IsUseButtonDown() && !isFireWeapon) || (pInputs.IsMeleeWeaponAttack() && isFireWeapon) || (pContinueAttack && isFireWeapon))
		{
			//! do not perform attacks when blocking
			if (m_IsInBlock || m_IsEvading)
				return false;
			
			//! if the item in hands cannot be used as melee weapon
			if (itemInHands && !itemInHands.IsMeleeWeapon() && !isFireWeapon)
				return false;
			
			//If player has BrokenLegs she cannot use melee weapons
			/*PlayerBase m_PlayerBase = PlayerBase.Cast(m_DZPlayer);
			if (m_PlayerBase.m_BrokenLegState == eBrokenLegs.BROKEN_LEGS)
				return false;*/

			//! Currently not performing any attacks, so here we start the initial
			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
			{
				//! melee with firearm
				if (isFireWeapon)
				{
					return HandleInitialFirearmMelee(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
				}
				else
				{
					//! first attack in raised erc (light or heavy if modifier is used). Also added support for finishers
					if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
					{
						return HandleInitialMeleeErc(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
					}
					//! kick from raised pne
					else if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
					{
						return HandleProneKick(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
					}
					//! sprint attack in erc stance
					else if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT && m_DZPlayer.IsSprintFull())
					{
						return HandleSprintAttack(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
					}
				}
			}
			//! combo hits - when we are already in Melee command and clicking UseButton
			else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE2)
			{
				return HandleComboHit(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
			}
		}
		//! evade and blocking logic
		else if (!isFireWeapon && pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
		{
			//! evades in raised erc stance while moving
			if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
			{
				int roll = pInputs.IsMeleeLREvade();
				if (roll != 0)
				{
					//! not enough stamina to do evades
					if (!m_DZPlayer.CanConsumeStamina(EStaminaConsumers.MELEE_EVADE))
						return false;
					
					float angle;
					if (roll == 1)
					{
						angle = -90;	// left
					}
					else
					{
						angle = 90;		// right
					}
	
					// start melee evade
					m_IsEvading = true;
					SetCooldown(EVADE_COOLDOWN, EFightLogicCooldownCategory.EVADE);
					hcm.StartMeleeEvadeA(angle);
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_EVADE);
					
					//Inflict shock when sidestepping with broken legs
					if (player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
					{
						player.m_ShockHandler.SetShock(PlayerConstants.BROKEN_LEGS_LIGHT_MELEE_SHOCK);
						player.m_ShockHandler.CheckValue(true);
					}
				}
			}

			//! stand up when crouching and raised pressed
			if (pInputs.IsWeaponRaised() && pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
			{
				hcm.ForceStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT);
			}

			//! blocks in raised erc/pro stance
			//! (bare hand or with melee weapon only)
			if (!isFireWeapon && (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT || pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE))
			{
				float angle2;

				if (hcm.GetCurrentInputAngle(angle2) && (angle2 < -130.0 || angle2 > 130))
				{
					hcm.SetMeleeBlock(true);
					SetBlock(true);
				}
				else
				{
					hcm.SetMeleeBlock(false);
					SetBlock(false);
				}
			}
			else
			{
				hcm.SetMeleeBlock(false);
				SetBlock(false);
			}
		}

		return false;
	}
	
	//! Handle the event that our melee hit something
	protected bool HandleHitEvent(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		//! Check if we need to damage
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE2)
		{
			HumanCommandMelee2 hmc2a = m_DZPlayer.GetCommand_Melee2();
			if (hmc2a)
			{
				//! on anim event Hit received
				if (hmc2a.WasHit())
				{
					//! re-target (enemy can have moved out of range or disappeared)
					m_MeleeCombat.Update(itemInHands, m_HitType, true);
					//! evaluate hit - selection of cfg 'ammo' type
					EvaluateHit(itemInHands);
					
					PlayerBase player = PlayerBase.Cast(m_DZPlayer);
					
					//Get gloves
					ClothingBase gloves;
					
					//We kick so "gloves" will be the shoes
					if ( m_HitType == EMeleeHitType.KICK )
						gloves = ClothingBase.Cast(player.GetItemOnSlot("FEET"));
					else
						gloves = ClothingBase.Cast(player.GetItemOnSlot("GLOVES"));
					
					//If we hit something, inflict damage
					DamageHands(m_DZPlayer, gloves, itemInHands);
					
					//m_MeleeCombat.CheckMeleeItem();
					
					EnableControls();
					
					return true;
				}
			}
		}
		
		return false;
	}
	
	//! NOTE: Only singular (or first) hits, combo attacks are handled in combo
	protected bool HandleInitialFirearmMelee(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{	
		//! don't allow bash to interfere with actions like chambering or ejecting bullets
		Weapon_Base weapon = Weapon_Base.Cast(itemInHands);
		PlayerBase player = PlayerBase.Cast(m_DZPlayer);
		if (weapon.IsWaitingForActionFinish() || player.GetActionManager().GetRunningAction())
			return false;		
		
		HumanCommandMove hcm = m_DZPlayer.GetCommand_Move();
		
		//! perform firearm melee from raised erect or continue with attack after character stand up from crouch
		if (hcm.GetCurrentMovementSpeed() <= 2.05 && (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT || pContinueAttack) && !m_DZPlayer.IsFighting())
		{
			m_HitType = GetAttackTypeByWeaponAttachments(itemInHands);
			if ( m_HitType == EMeleeHitType.NONE )
				return false; //! exit if there is no suitable attack
			
			m_MeleeCombat.Update(itemInHands, m_HitType);

			EntityAI target;
			EMeleeTargetType targetType;
			GetTargetData(target, targetType);
			float attackByDistance = GetAttackTypeByDistanceToTarget(target, targetType);

			m_DZPlayer.StartCommand_Melee2(target, m_HitType == EMeleeHitType.WPN_STAB, attackByDistance, m_MeleeCombat.GetHitPos());
			
			// finisher attack hittype override
			int finisher_type = GetFinisherType(itemInHands, target);
			if (finisher_type != -1)
			{
				m_HitType = finisher_type;
				target.SetBeingBackstabbed();
			}
			m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
			DisableControls();

			pContinueAttack = false; // reset continueAttack flag

			return true;
		}				
		//! char stand up when performing firearm melee from crouch
		else if ( pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH )
		{
			if (hcm)
			{
				hcm.ForceStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT);
				pContinueAttack = true;
				return false;
			}
		}
		
		return false;
	}
	
	//! First attack in raised erc (light or heavy if modifier is used). Also added support for finishers
	protected bool HandleInitialMeleeErc(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		PlayerBase player = PlayerBase.Cast(m_DZPlayer);
		
		m_HitType = GetAttackTypeFromInputs(pInputs);		
		m_MeleeCombat.Update(itemInHands, m_HitType);
		
		EntityAI target;
		EMeleeTargetType targetType;
		GetTargetData(target, targetType);
		float attackByDistance = GetAttackTypeByDistanceToTarget(target, targetType);		
		
		// finisher attack hittype override
		int finisher_type = GetFinisherType(itemInHands,target);
		if (finisher_type != -1)
		{
			target.SetBeingBackstabbed();
			int animation_type = DetermineFinisherAnimation(finisher_type);
			
			if ( animation_type > -1 )
			{
				m_DZPlayer.StartCommand_Melee2(target, animation_type, attackByDistance, m_MeleeCombat.GetHitPos());
			}
			else
			{
				m_DZPlayer.StartCommand_Melee2(target, m_HitType == EMeleeHitType.HEAVY, attackByDistance, m_MeleeCombat.GetHitPos());
			}
			
			m_HitType = finisher_type;
		}
		else
		{
			m_DZPlayer.StartCommand_Melee2(target, m_HitType == EMeleeHitType.HEAVY, attackByDistance, m_MeleeCombat.GetHitPos());
		}
		
		if (m_HitType == EMeleeHitType.HEAVY)
		{
			m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
			if (player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			{	
				player.m_ShockHandler.SetShock(PlayerConstants.BROKEN_LEGS_HEAVY_MELEE_SHOCK);
				player.m_ShockHandler.CheckValue(true);
				//m_PlayerBase.DealShock(PlayerConstants.BROKEN_LEGS_HEAVY_MELEE_SHOCK);
			}
		}
		else
		{
			m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
			if (player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			{
				player.m_ShockHandler.SetShock(PlayerConstants.BROKEN_LEGS_LIGHT_MELEE_SHOCK);
				player.m_ShockHandler.CheckValue(true);
				//m_PlayerBase.DealShock(PlayerConstants.BROKEN_LEGS_LIGHT_MELEE_SHOCK);
			}
		}	
		
		DisableControls();
		return true;
	}
	
	//! kick from raised pne
	protected bool HandleProneKick(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		HumanCommandWeapons hcw = m_DZPlayer.GetCommandModifier_Weapons();

		PlayerBase player = PlayerBase.Cast(m_DZPlayer);

		float hcw_angle = hcw.GetBaseAimingAngleLR();
		//! check if player is on back
		//! (situation where the player is raised in prone and on back is not in anim graph)
		if ( ( hcw_angle < -90 || hcw_angle > 90 ) && player.GetBrokenLegs() != eBrokenLegs.BROKEN_LEGS )
		{
			// targetting
			m_HitType = EMeleeHitType.KICK;
			m_MeleeCombat.Update(itemInHands, m_HitType);

			EntityAI target;
			EMeleeTargetType targetType;
			GetTargetData(target, targetType);
			float attackByDistance = GetAttackTypeByDistanceToTarget(target, targetType);

			// command
			m_DZPlayer.StartCommand_Melee2(target, false, attackByDistance, m_MeleeCombat.GetHitPos());
			m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
			DisableControls();
			
			return true;
		}
		
		return false;
	}
	
	//! sprint attack in erc stance
	protected bool HandleSprintAttack(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		//! targetting
		m_HitType = EMeleeHitType.SPRINT;
		m_MeleeCombat.Update(itemInHands, m_HitType);

		EntityAI target;
		EMeleeTargetType targetType;
		GetTargetData(target, targetType);
		float attackByDistance = GetAttackTypeByDistanceToTarget(target, targetType);

		//! command
		m_DZPlayer.StartCommand_Melee2(target, false, attackByDistance, m_MeleeCombat.GetHitPos());
		m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
		DisableControls();
		return true;
	}
	
	//! combo hits - when we are already in Melee command and clicking UseButton
	protected bool HandleComboHit(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		//! no suitable attack - skip that
		if ( m_HitType == EMeleeHitType.NONE )
			return false;
		
		//! No combos for the finisher command
		if (m_IsFinisher)
			return false;

		HumanCommandMelee2 hmc2 = m_DZPlayer.GetCommand_Melee2();

		if (hmc2 && hmc2.IsInComboRange())
		{
			//! targetting
			// m_HitType = GetAttackTypeFromInputs(pInputs); // I have a feeling this could be useful here
			m_MeleeCombat.Update(itemInHands, m_HitType);
			
			EntityAI target;
			EMeleeTargetType targetType;
			GetTargetData(target, targetType);
			float attackByDistance = GetAttackTypeByDistanceToTarget(target, targetType);
			
			PlayerBase player = PlayerBase.Cast(m_DZPlayer);
		
			//! select if the next attack will light or heavy (based on input/attachment modifier)
			if ( itemInHands && itemInHands.IsWeapon() )
			{
				m_HitType = GetAttackTypeByWeaponAttachments(itemInHands);			
			}
			else
			{
				m_HitType = GetAttackTypeFromInputs(pInputs);
			}
			
			//! continue 'combo' - left hand attacks
			bool isHeavy = (m_HitType == EMeleeHitType.HEAVY || m_HitType == EMeleeHitType.WPN_STAB);			
			hmc2.ContinueCombo(isHeavy, attackByDistance, target, m_MeleeCombat.GetHitPos());				
			DisableControls();
			
			//! broken legs shock
			if (player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			{
				float shock = PlayerConstants.BROKEN_LEGS_HEAVY_MELEE_SHOCK;
				
				if (!isHeavy)
					shock = PlayerConstants.BROKEN_LEGS_LIGHT_MELEE_SHOCK;
				
				player.m_ShockHandler.SetShock(PlayerConstants.BROKEN_LEGS_HEAVY_MELEE_SHOCK);
				player.m_ShockHandler.CheckValue(true);
			}

			//! stamina depletion per attack
			switch ( m_HitType )
			{
				case EMeleeHitType.HEAVY:
				case EMeleeHitType.SPRINT:
				case EMeleeHitType.WPN_STAB:
				case EMeleeHitType.WPN_HIT_BUTTSTOCK:
				case EMeleeHitType.WPN_HIT:
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
				break;
				default:
					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
				break;
			}

			return true;
		}
		else
		{
			return true; // ?
		}
	}

	protected void EvaluateHit(InventoryItem weapon)
	{
		EntityAI target = m_MeleeCombat.GetTargetEntity();
	
		if ( target )
		{
			if ( target.IsInherited(DayZPlayer) )
			{
				EvaluateHit_Player(weapon, target);
			}
			else if ( target.IsInherited(DayZInfected) )
			{
				EvaluateHit_Infected(weapon, target);
			}
			else if ( target.GetMeleeTargetType() == EMeleeTargetType.NONALIGNABLE )
			{
				EvaluateHit_NonAlignableObjects(weapon, target);
			}
			else
			{
				EvaluateHit_Common(weapon, target);
			}
			
			m_MeleeCombat.CheckMeleeItem();
		}
	}

	protected void EvaluateHit_Player(InventoryItem weapon, Object target)
	{
		int hitZoneIdx = m_MeleeCombat.GetHitZoneIdx();
		int weaponMode = m_MeleeCombat.GetWeaponMode();
		vector hitPosWS;
		bool forcedDummy = false;

		PlayerBase targetPlayer = PlayerBase.Cast(target);

		//! Melee Hit/Impact modifiers
		if ( targetPlayer )
		{
			vector targetPos = targetPlayer.GetPosition();
			vector agressorPos = m_DZPlayer.GetPosition();
			
			// We get the angle from which an infected hit the blocking player
			float hitAngle = Math.RAD2DEG * Math.AbsFloat(Math3D.AngleFromPosition(targetPos, MiscGameplayFunctions.GetHeadingVector(targetPlayer), agressorPos));
							
			//! if the oponnent is in Melee Block shift the damage down
			if ( targetPlayer.GetMeleeFightLogic() && targetPlayer.GetMeleeFightLogic().IsInBlock() && hitAngle <= GameConstants.PVP_MAX_BLOCKABLE_ANGLE )
			{
				if ( weaponMode > 0 )
				{
					weaponMode--; // Heavy -> Light shift
				}
				else
				{
					forcedDummy = true; // dummy hits, cannot shift lower than 0
				}
			}
		}

		EvaluateHit_Common(weapon, target, forcedDummy);
	}
	
	protected void EvaluateHit_Infected(InventoryItem weapon, Object target)
	{
		//If the finisher attack haven't been performed, then use normal hit
		if (!EvaluateFinisherAttack(weapon, target))
			EvaluateHit_Common(weapon, target);
	}
	
	//! Check and evaluate stealth kill, if applicable
	protected bool EvaluateFinisherAttack(InventoryItem weapon, Object target)
	{
		if (GetGame().IsServer())
		{
			if (m_IsFinisher)
			{
				target.ProcessDirectDamage(DT_CLOSE_COMBAT, weapon, "", DetermineFinisherAmmo(m_HitType), "0 0 0", 1.0);
				
				//Component can't be easily used, since the function uses DAMAGE ZONE. Investigating later.
				/*string name = target.GetActionComponentName(m_MeleeCombat.GetHitZoneIdx(),"fire");
				target.ProcessDirectDamage(DT_CLOSE_COMBAT, weapon, name, "FinisherHit", "0 0 0", 1.0);*/
				
				return true;
			}
		}
		//Finisher attack not performed
		return false;
	}
	
	protected void EvaluateHit_Common(InventoryItem weapon, Object target, bool forcedDummy=false)
	{
		int hitZoneIdx = m_MeleeCombat.GetHitZoneIdx();
		int weaponMode = m_MeleeCombat.GetWeaponMode();
		vector hitPosWS;
		string ammo;

		EntityAI targetEntity = EntityAI.Cast(target);

		//! check if we need to use dummy hit
		if (!DummyHitSelector(m_HitType, ammo) && !forcedDummy)
		{
			//! normal hit with applied damage to targeted component
			if (hitZoneIdx >= 0)
			{
				hitPosWS = targetEntity.ModelToWorld(targetEntity.GetDefaultHitPosition());
				
				if (WeaponDestroyedCheck(weapon,ammo))
					DamageSystem.CloseCombatDamage(m_DZPlayer, target, hitZoneIdx, ammo, hitPosWS);
				else
					m_DZPlayer.ProcessMeleeHit(weapon, weaponMode, target, hitZoneIdx, hitPosWS);
			}
		}
		else
		{
			//! play hit animation for dummy hits
			if ( GetGame().IsServer() && targetEntity )
			{
				hitPosWS = targetEntity.ModelToWorld(targetEntity.GetDefaultHitPosition()); //! override hit pos by pos defined in type
				targetEntity.EEHitBy(null, 0, m_DZPlayer, hitZoneIdx, "", ammo, hitPosWS, 1.0);
			}
		}
	}

	protected void EvaluateHit_NonAlignableObjects(InventoryItem weapon, Object target)
	{
		int hitZoneIdx = m_MeleeCombat.GetHitZoneIdx();
		vector hitPosWS;
		string ammo;
		
		if (hitZoneIdx >= 0)
		{
			if (WeaponDestroyedCheck(weapon,ammo))
				DamageSystem.CloseCombatDamage(m_DZPlayer, target, hitZoneIdx, ammo, m_MeleeCombat.GetHitPos());
			else
				m_DZPlayer.ProcessMeleeHit(weapon, m_MeleeCombat.GetWeaponMode(), target, hitZoneIdx, m_MeleeCombat.GetHitPos());
		}

		return;
	}
	
	bool WeaponDestroyedCheck(InventoryItem weapon, out string ammo)
	{
		if (!weapon)
			return false;
		
		Weapon_Base firearm = Weapon_Base.Cast(weapon);
		ItemBase bayonet = ItemBase.Cast(weapon.GetInventory().FindAttachment(weapon.GetBayonetAttachmentIdx()));
		
		if ( firearm && bayonet && bayonet.IsRuined() )
		{
			ammo = bayonet.GetRuinedMeleeAmmoType();
			return true;
		}
		else if (weapon.IsRuined())
		{
			ammo = weapon.GetRuinedMeleeAmmoType();
			return true;
		}
		else
		{
			return false;
		}
	}

	protected void GetTargetData(out EntityAI target, out EMeleeTargetType targetType)
	{
		target = m_MeleeCombat.GetTargetEntity();
		targetType = EMeleeTargetType.ALIGNABLE; //! default
		
		// If there is no hitzone defined, then we won't be able to hit the target anyways..
		if (m_MeleeCombat.GetHitZoneIdx() < 0)
		{
			target = null;
		}
		
		if ( target )
		{
			targetType = target.GetMeleeTargetType();
		}

		//! nullify target for nonalignable objects (big objects)
		if ( targetType == EMeleeTargetType.NONALIGNABLE )
		{
			target = null;
		}
	}

	//! evaluation of hit player vs. player
	protected bool DummyHitSelector(EMeleeHitType hitType, out string ammoType)
	{
		switch (hitType)
		{
		//! in case of kick (on back or push from erc) change the ammo type to dummy
		case EMeleeHitType.KICK:
			ammoType = DUMMY_HEAVY_AMMO;
			return true;
		break;
		}
		
		ammoType = DUMMY_LIGHT_AMMO;
		
		return false;
	}
	
	protected bool IsBehindEntity(int angle, DayZPlayer source, Object target)
	{
		//! behind entity check
		vector targetDirection = target.GetDirection();
		ZombieBase target_zombie;
		if ( Class.CastTo(target_zombie,target) )
		{
			targetDirection = Vector(target_zombie.GetOrientationSynced(),0,0);
			targetDirection = targetDirection.AnglesToVector();
		}
		
		vector toSourceDirection = (source.GetPosition() - target.GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		int hitDir = Math.Acos(cosFi) * Math.RAD2DEG;
		
		if ( cross[1] < 0 )
			hitDir = -hitDir;
		
		if ( hitDir <= (-180 + angle) || hitDir >= (180 - angle) )
		{
			return true;
		}
		
		return false;
	}

	protected void SetCooldown(float time, EFightLogicCooldownCategory cooldownCategory)
	{
		//! stops currently running cooldown timer (for specific category)
		if ( m_CooldownTimers.Get(cooldownCategory).IsRunning() )
		{
			m_CooldownTimers.Get(cooldownCategory).Stop();
		}
		
		//! param for ResetCooldown
		Param1<int> param = new Param1<int>(cooldownCategory);
		m_CooldownTimers.Get(cooldownCategory).Run(time, this, "ResetCooldown", param);
	}
	
	protected void ResetCooldown(EFightLogicCooldownCategory cooldownCategory)
	{
		switch (cooldownCategory)
		{
		case EFightLogicCooldownCategory.EVADE:
			m_IsEvading = false;
		break;
		}
	}

	protected void EnableControls()
	{
		/*
		if (m_Mission)
		{
			m_Mission.PlayerControlEnable(true);
		}
		*/
	}

	protected void DisableControls()
	{
		/*
		if (m_Mission)
		{
			m_Mission.PlayerControlDisable(INPUT_EXCLUDE_MOUSE_ALL);
		}
		*/
	}
	
	protected void DamageHands(DayZPlayer DZPlayer, ClothingBase gloves, InventoryItem itemInHands)
	{
		EntityAI target = m_MeleeCombat.GetTargetEntity();
		
		//We did not hit anything
		if ( itemInHands || !target || !DZPlayer )
			return;
		
		//Check if server side
		if ( GetGame().IsServer() )
		{
			int randNum;
			
			//If gloves, damage gloves
			if ( gloves && gloves.GetHealthLevel() < GameConstants.STATE_RUINED )
			{
				gloves.DecreaseHealth("", "", 1);
			}
			else
			{
				//Do not add bleeding if hitting player, zombie or animal
				if ( PlayerBase.Cast(target) || DayZCreatureAI.Cast( target ) )
					return;
				
				PlayerBase player;
				BleedingSourcesManagerServer bleedingManager;
				
				//We don't inflict bleeding to hands when kicking
				if ( m_HitType != EMeleeHitType.KICK )
				{
					//If no gloves, inflict hand damage and bleeding
					//DZPlayer.DecreaseHealth("hands", "", 10); IN CASE WE WANT TO DO STUFF WITH HAND HEALTH
					
					//Random bleeding source
					randNum = Math.RandomIntInclusive(1, 15);
					switch ( randNum )
					{
						case 1:
							player = PlayerBase.Cast( DZPlayer );
							if ( player )
							{
								bleedingManager = player.GetBleedingManagerServer();
								if ( bleedingManager )
								{
									bleedingManager.AttemptAddBleedingSourceBySelection("RightForeArmRoll");
								}
							}
						break;
						
						case 2:
							player = PlayerBase.Cast( DZPlayer );
							if ( player )
							{
								bleedingManager = player.GetBleedingManagerServer();
								if ( bleedingManager )
								{
									bleedingManager.AttemptAddBleedingSourceBySelection("LeftForeArmRoll");
								}
							}
						break;
						
						default:
							//Do nothing here
						break;
					}
				}
				else
				{
					//Random bleeding source
					randNum = Math.RandomIntInclusive(1, 15);
					//We only add bleeding to left foot as character kicks with left foot
					switch ( randNum )
					{
						case 1:
							player = PlayerBase.Cast( DZPlayer );
							if ( player )
							{
								bleedingManager = player.GetBleedingManagerServer();
								if ( bleedingManager )
								{
									bleedingManager.AttemptAddBleedingSourceBySelection("LeftToeBase");
								}
							}
						break;
						
						case 2:
							player = PlayerBase.Cast( DZPlayer );
							if ( player )
							{
								bleedingManager = player.GetBleedingManagerServer();
								if ( bleedingManager )
								{
									bleedingManager.AttemptAddBleedingSourceBySelection("LeftFoot");
								}
							}
						break;
						
						default:
							//Do nothing here
						break;
					}
				}
			}
		}
	}
	
	/**
	\brief General condition for finisher attacks
		\param weapon \p Weapon used in the attack
		\param target \p Target entity
		\return \p int - type of finisher (-1 == no finisher)
	*/
	int GetFinisherType(InventoryItem weapon, EntityAI target)
	{
		m_IsFinisher = false;
		//! perform only for finisher suitable weapons
		if ( target && target.CanBeBackstabbed() && weapon && (weapon.IsMeleeFinisher() || m_HitType == EMeleeHitType.WPN_STAB) && !weapon.IsRuined() )
		{
			bool play_generic_finisher = false;
			ZombieBase target_zombie = ZombieBase.Cast(target);
			PlayerBase target_player = PlayerBase.Cast(target);
			if ( target_zombie && m_DbgForcedFinisherType == -1 )
			{
				//! check if attacker is in right pos and angle against victim
				if ( !IsBehindEntity(60, m_DZPlayer, target) )
				{
					return -1;
				}
				
				int mindState = target_zombie.GetMindStateSynced();
				
				//Check if the infected is aware of the player
				if (mindState >= DayZInfectedConstants.MINDSTATE_DISTURBED)
				{
					return -1;
				}
			}
			
			m_IsFinisher = true;
			
			//prone check
			if (target_zombie)
			{
				play_generic_finisher = target_zombie.IsCrawling();
			}
			else if (target_player)
			{
				play_generic_finisher = target_player.IsInProne();
			}
			else
			{
				play_generic_finisher = true; //unhandled target, play generic animation..
			}
			
			//firearm
			if ( weapon.IsWeapon() )
			{
				return EMeleeHitType.WPN_STAB_FINISHER;
			}
			else if ( play_generic_finisher ) //problematic animation, default animation used instead
			{
				return EMeleeHitType.FINISHER_GENERIC;
			}
			else
			{
				return DetermineSpecificFinisherType(ItemBase.Cast(weapon),m_MeleeCombat.GetHitZoneIdx());
			}
		}
		return -1;
	}
	
	//! DEPRECATED
	bool IsHitTypeFinisher(int type)
	{
		return false;
	}
	
	//! Picks a specific finisher animation. Not used for mounted bayonet stabs
	int DetermineFinisherAnimation(int finisher_type)
	{
		int animation = -1;
		
		switch (finisher_type)
		{
			case EMeleeHitType.FINISHER_LIVERSTAB:
				animation = HumanCommandMelee2.HIT_TYPE_FINISHER;
			break;
		}
		
		return animation;
	}
	
	//! Picks a specific finisher ammo fot the hit type. This gets synchronized and guides further behaviour of the target.
	string DetermineFinisherAmmo(int finisher_type)
	{
		string ret = "";
		
		switch (finisher_type)
		{
			default:
				ret = "FinisherHit";
			break;
		}
		return ret;
	}
	
	int DetermineSpecificFinisherType(ItemBase weapon, int component)
	{
		if (m_DbgForcedFinisherType > -1)
		{
			return EMeleeHitType.FINISHER_LIVERSTAB;
		}
		
		if (!weapon || !weapon.GetValidFinishers() || weapon.GetValidFinishers().Count() == 0)
		{
			return EMeleeHitType.FINISHER_LIVERSTAB; //default
		}
		
		PlayerBase player = PlayerBase.Cast(m_DZPlayer);
		int idx = Math.Round(Math.Lerp(0,weapon.GetValidFinishers().Count() - 1,player.GetRandomGeneratorSyncManager().GetRandom01(RandomGeneratorSyncUsage.RGSGeneric)));;
		return weapon.GetValidFinishers()[idx];
	}
}