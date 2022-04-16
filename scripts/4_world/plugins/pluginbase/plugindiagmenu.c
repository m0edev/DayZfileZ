enum DebugActionType
{
	GENERIC_ACTIONS = 1,
	PLAYER_AGENTS = 2,
	UNLIMITED_AMMO = 4,
};


class PluginDiagMenu extends PluginBase
{
#ifdef DEVELOPER
	ref Timer m_Timer;
	bool m_EnableModifiers 			= true;
	bool m_EnableUnlimitedAmmo;
	bool m_DisableBloodLoss			= false;
	int m_IsInvincible 				= false;
	bool m_ShowCraftingDebugActions = false;
	bool m_LogPlayerStats 			= false;
	bool m_SoftSkillsState 			= false;
	bool m_SoftSkillsDebug 			= false;
	bool m_GunParticlesState		= false;
	bool m_SoftSkillsModel 			= false;
	bool m_BloodyHands	 			= false;
	bool m_PermanentCrossHair		= false;
	bool m_ToggleHud				= false;
	bool m_ShowingWeaponDebug		= false;
	bool m_AimNoiseEnabled			= true;
	int m_DisplayPlayerInfo			= false;
	bool m_ProceduralRecoilEnabled 	= true;
	bool m_EnableQuickRestrain 		= false;
	bool m_EnableQuickFishing		= false;
	bool m_StaminaDisabled			= false;
	bool m_EnvironmentStats			= false;
	bool m_DrawCheckerboard			= false;
	bool m_PresenceNotifierDebug	= false;
	bool m_ShowBleedingSources		= false;
	bool m_XboxCursor				= true;
	bool m_DoActionLogs				= false;
	bool m_DoWeaponLogs				= false;
	bool m_DoInventoryMoveLogs		= false;
	bool m_DoInventoryReservationLogs	= false;
	bool m_DoInventoryHFSMLogs		= false;
	bool m_DoSymptomLogs			= false;
	bool m_AllowInventoryAccess		= false;
	bool m_FixItems					= false;
	bool m_HitIndicationDebugEnabled		= false;
	float m_SpecialtyLevel			= 0;
	float m_LifespanLevel			= 0;
	int  m_DayzPlayerDebugMenu		= -1;
	int m_BleedingSourceRequested;
	int m_HairLevelSelected 		= 0;
	int m_TotalHairLevelsAdjusted;
	int m_Burst = 0;
	static bool ENABLE_BREATH_VAPOR;
	
	//string m_HairSelections 		= "Clipping_GhillieHood, Clipping_grathelm, Clipping_ConstructionHelmet, Clipping_Hockey_hekmet, Clipping_Maska, Clipping_ProtecSkateHelmet2, Clipping_BandanaFace, Clipping_NioshFaceMask, Clipping_NBC_Hood, Clipping_MotoHelmet, Clipping_FireHelmet, Clipping_ushanka, Clipping_TankerHelmet, Clipping_SantasBeard, Clipping_Surgical_mask, Clipping_PumpkinHelmet, Clipping_Balaclava_3holes, Clipping_Balaclava, Clipping_GP5GasMask, Clipping_BoonieHat, Clipping_prison_cap, Clipping_MilitaryBeret_xx, Clipping_Policecap, Clipping_OfficerHat, Clipping_Hat_leather, Clipping_CowboyHat, Clipping_BandanaHead, Clipping_SantasHat, Clipping_FlatCap, Clipping_MxHelmet, Clipping_baseballcap, Clipping_BeanieHat, Clipping_MedicalScrubs_Hat, Clipping_RadarCap, Clipping_ZmijovkaCap, Clipping_HeadTorch, Clipping_pilotka, Clipping_MxHelmet, Clipping_HelmetMich, Clipping_Ssh68Helmet, Clipping_Mich2001, Clipping_Welding_Mask, Clipping_VintageHockeyMask, Clipping_mouth_rags, Clipping_Gasmask";
	ref map<int,bool> m_HairHidingStateMap;
	ref TStringArray m_HairSelectionArray;
	Shape m_VehicleFreeAreaBox;
	
	override void OnInit()
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() ) return; //(only client/local)
		
		m_DoActionLogs = IsCLIParam("doActionLog");
		m_DoActionLogs = IsCLIParam("doInventoryLog");
		m_DoActionLogs = IsCLIParam("doActionLog");
		m_DoWeaponLogs = IsCLIParam("doWeaponLog");
		
		//----------------------
		m_HairHidingStateMap = new map<int,bool>;
		m_HairSelectionArray = new TStringArray;
		m_VehicleFreeAreaBox = Debug.DrawBox(Vector(0,0,0), Vector(0,0,0), 0xffffffff);
		
		g_Game.ConfigGetTextArray("cfgVehicles Head_Default simpleHiddenSelections",m_HairSelectionArray);
		m_TotalHairLevelsAdjusted = m_HairSelectionArray.Count() - 1;
		for (int i = 0; i < m_HairSelectionArray.Count(); i++)
		{
			m_HairHidingStateMap.Insert(i,1); //all considered "shown" on init
		}
		
		//----------------------
		m_Timer = new Timer();

		m_Timer.Run(0.5, this, "Update", NULL, true);
		
		//---------------------------------------------------------------
		// LEVEL 0
		//---------------------------------------------------------------
 		DiagMenu.RegisterMenu(DiagMenuIDs.DM_SCRIPTS_MENU,"Script", "");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_BLEEDING_MENU,"Bleeding", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SHOW_BLEEDING_SOURCES, "", "Show Bleeding Sources", "Bleeding");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_DISABLE_BLOOD_LOSS, "", "Disable Blood Loss", "Bleeding");
				DiagMenu.RegisterRange(DiagMenuIDs.DM_ACTIVATE_SOURCE, "", "Activate Source #", "Bleeding", "1, 32, 0, 1");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_ACTIVATE_ALL_BS, "", "Activate All Sources", "Bleeding");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_BS_RELOAD, "", "Client Reload", "Bleeding");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_CHEATS_MENU,"Cheats", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_CHEATS_MODIFIERS_ENABLE, "lalt+2", "Tick Modifiers", "Cheats");
				DiagMenu.SetValue(DiagMenuIDs.DM_CHEATS_MODIFIERS_ENABLE, true);
				DiagMenu.RegisterItem( DiagMenuIDs.DM_CHEATS_INVINCIBILITY, "lalt+1", "Invincibility (IDDQD)", "Cheats", "Disabled,Partial,Full" );
				DiagMenu.RegisterBool(DiagMenuIDs.DM_CHEATS_PLAYER_INSTAKILL, "", "Kill Player", "Cheats");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_CHEATS_STAMINA_DISABLE, "", "Disable stamina", "Cheats");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_INVENTORY_ACCESS, "", "Inventory Access", "Cheats");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_RESET, "", "Reset Player", "Cheats");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_RESET_MAX, "lalt+3", "Reset Player Max", "Cheats");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_FIX_ITEMS, "", "Fix Inventory Items", "Cheats");
			
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_PLAYER_CRAFTING_MENU,"Crafting", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_CRAFTING_GENERATE, "", "Generate Cache", "Crafting");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_CRAFTING_DEBUG_ACTIONS_ENABLE, "", "Debug Insta Crafting", "Crafting");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_CRAFTING_DUMP, "", "Dump recipes to file", "Crafting");
			
			
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_PLAYER_STATES_MENU,"Player States", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_SYMPTOMS_SHOW, "lalt+6", "Show States", "Player States");

			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_TRANSFER_VALUES_MENU, "TransferValues", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_TRANSFER_VALUES_SHOW, "", "ShowValues", "TransferValues");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_PLAYER_AGENTS_MENU, "Player Agents", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_AGENTS_INJECTS_SHOW, "lalt+5", "Allow Inject Actions", "Player Agents");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_SOFT_SKILLS_MENU, "Soft Skills", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool( DiagMenuIDs.DM_SOFT_SKILLS_SHOW_DEBUG, "", "Show Debug", "Soft Skills" );
				DiagMenu.RegisterBool( DiagMenuIDs.DM_SOFT_SKILLS_TOGGLE_STATE, "", "Enable SoftSkills", "Soft Skills" );
				DiagMenu.RegisterBool( DiagMenuIDs.DM_SOFT_SKILLS_TOGGLE_MODEL, "", "Enable linear model", "Soft Skills" );
				DiagMenu.RegisterRange( DiagMenuIDs.DM_SOFT_SKILLS_SPECIALTY_VALUE, "", "Set specialty value", "Soft Skills", "-1, 1, 0, 0.01" );
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_LIFESPAN_MENU, "Lifespan", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool( DiagMenuIDs.DM_BLOODY_HANDS, "", "Bloody hands", "Lifespan" );
				DiagMenu.RegisterRange( DiagMenuIDs.DM_LIFESPAN_PLAYTIME_UPDATE, "", "Playtime in seconds", "Lifespan", "0, 3600, 0, 300" );
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_MISC_SIMULATE, "Simulate script", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SIMULATE_INFINITE_LOOP, "", "Simulate infinite loop", "Simulate script");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SIMULATE_NULL_POINTER, "", "Simulate null pointer", "Simulate script");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SIMULATE_DIVISION_BY_ZERO, "", "Simulate division by 0", "Simulate script");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SIMULATE_ERROR_FUNCTION, "", "Simulate Error() function", "Simulate script");	
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_MISC_MENU, "Misc", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_ITEM_DEBUG_ACTIONS_SHOW, "lalt+4", "Item Debug Actions", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_BULLET_IMPACT, "lalt+7", "BulletImpact", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SHOCK_IMPACT, "lalt+8", "ShockHitEffect", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_TARGETABLE_BY_AI, "", "Toggle Targetable By AI", "Misc");
				DiagMenu.SetValue(DiagMenuIDs.DM_TARGETABLE_BY_AI, true);
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_STATS_LOG_ENABLE, "", "Log Player Stats", "Misc");
				DiagMenu.RegisterMenu(DiagMenuIDs.DM_ACTION_TARGETS_MENU, "Action Targets", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_XBOX_CURSOR, "", "XboxCursor", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PERMANENT_CROSSHAIR, "", "Enable permanent crosshair", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SHOW_VEHICLE_GETOUT_BOX, "", "Debug transport freespace", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_TOGGLE_HUD, "", "Toggle HUD on/off", "Misc", true);
				DiagMenu.RegisterRange(DiagMenuIDs.DM_DISPLAY_PLAYER_INFO, "", "Display Player Info", "Misc", "0,2,0,1");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_ENVIRONMENT_DEBUG_ENABLE, "", "Show Environment stats", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_DRAW_CHECKERBOARD, "", "Draw Checkerboard on screen", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PRESENCE_NOTIFIER_DBG, "", "Show Presence to AI dbg", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_GO_UNCONSCIOUS, "", "Go Unconscious", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_GO_UNCONSCIOUS_DELAYED, "", "Uncons. in 10sec", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_QUICK_RESTRAIN, "ralt+0", "Quick Restrain", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_QUICK_FISHING, "", "Quick Fishing", "Misc");
				DiagMenu.RegisterMenu(DiagMenuIDs.DM_HAIR_MENU, "Hair Hiding", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_DISABLE_PERSONAL_LIGHT, "", "Disable Personal Light", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_CAM_SHAKE, "", "Simulate Cam Shake", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_HOLOGRAM, "lctrl+h", "Hologram placing debug", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SHOW_FIREHEAT_RADIUS, "", "Show Fire Radius", "Misc"); 
				DiagMenu.RegisterBool(DiagMenuIDs.DM_BREATH_VAPOR_LVL, "", "Breath Vapor", "Misc");
				DiagMenu.SetValue(DiagMenuIDs.DM_BREATH_VAPOR_LVL, true);
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SHOW_PLUG_ARROWS, "", "Show Energy Manager Plug Arrows", "Misc");
				DiagMenu.RegisterMenu(DiagMenuIDs.DM_HIT_INDICATION_MENU, "Hit Indication", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SUPPRESSNEXTFRAMEINPUTS, "", "Suppress Next Frame Inputs", "Misc");
				DiagMenu.SetValue(DiagMenuIDs.DM_SUPPRESSNEXTFRAMEINPUTS, false);
				DiagMenu.RegisterBool(DiagMenuIDs.DM_FREEZE_ENTITY, "lalt+x", "Freeze entity", "Misc");
					//---------------------------------------------------------------
					// LEVEL 3
					//---------------------------------------------------------------
					DiagMenu.RegisterBool(DiagMenuIDs.DM_ACTION_TARGETS_NEW, "", "New AT Selection", "Action Targets", true);
					DiagMenu.RegisterBool(DiagMenuIDs.DM_ACTION_TARGETS_DEBUG, "", "Show Debug", "Action Targets");
					DiagMenu.RegisterBool(DiagMenuIDs.DM_ACTION_TARGETS_SELPOS_DEBUG, "", "Show selection pos debug", "Action Targets");
					DiagMenu.RegisterBool(DiagMenuIDs.DM_HAIR_DISPLAY_DEBUG, "", "Display Debug", "Hair Hiding");
					DiagMenu.RegisterRange(DiagMenuIDs.DM_HAIR_LEVEL, "", "Hair Level#", "Hair Hiding","0,"+m_TotalHairLevelsAdjusted+",0,1");
					DiagMenu.RegisterBool(DiagMenuIDs.DM_HAIR_LEVEL_HIDE, "", "Toggle Hair Level", "Hair Hiding");
					DiagMenu.RegisterBool(DiagMenuIDs.DM_HAIR_HIDE_ALL, "", "Hide/Show All", "Hair Hiding");
					//hit indication
					DiagMenu.RegisterBool(DiagMenuIDs.DM_HIT_INDICATION_ENABLE_DEBUG, "", "Enable Debug", "Hit Indication");
					DiagMenu.SetValue(DiagMenuIDs.DM_HIT_INDICATION_ENABLE_DEBUG,false);
					DiagMenu.RegisterBool(DiagMenuIDs.DM_HIT_INDICATION_SPAWN_HIT, "", "Spawn Hit", "Hit Indication");
					DiagMenu.RegisterRange(DiagMenuIDs.DM_HIT_INDICATION_SPAWN_HIT_DIRECTION, "", "Spawn Hit Direction", "Hit Indication", "0, 359, 0, 1");
					DiagMenu.RegisterItem(DiagMenuIDs.DM_HIT_INDICATION_MODE, "", "Mode", "Hit Indication", "Disabled,Static,Dynamic");
					DiagMenu.SetValue(DiagMenuIDs.DM_HIT_INDICATION_MODE,1);
					DiagMenu.RegisterItem(DiagMenuIDs.DM_HIT_INDICATION_TYPE, "", "Type", "Hit Indication", "Splash,Spike,Arrow");
					DiagMenu.RegisterRange(DiagMenuIDs.DM_HIT_INDICATION_DURATION, "", "Hit Duration Max", "Hit Indication", "0.1, 5.0, 1.0, 0.1");
					DiagMenu.RegisterRange(DiagMenuIDs.DM_HIT_INDICATION_BREAKPOINT, "", "Hit Breakpoint", "Hit Indication", "0.0, 1.0, 0.8, 0.05");
					DiagMenu.RegisterRange(DiagMenuIDs.DM_HIT_INDICATION_DISTANCE, "", "Distance Adjust", "Hit Indication", "0.0, 1.0, 0.0, 0.01");
					DiagMenu.RegisterRange(DiagMenuIDs.DM_HIT_INDICATION_ROTATION, "", "Rotation Override", "Hit Indication", "0, 360, 0, 15");
					DiagMenu.RegisterRange(DiagMenuIDs.DM_HIT_INDICATION_SCATTER, "", "Direction Scatter", "Hit Indication", "0, 90, 0, 1");
					DiagMenu.RegisterBool(DiagMenuIDs.DM_HIT_INDICATION_DISABLE_PPE, "", "Disable Hit PPE", "Hit Indication");
					DiagMenu.SetValue(DiagMenuIDs.DM_HIT_INDICATION_DISABLE_PPE,false);
					
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_MELEE_MENU, "Melee", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_DEBUG_ENABLE, "", "Enable Melee Debug", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_CONTINUOUS, "", "Continuous update", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_SHOW_TARGETS, "", "Show targets", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_DRAW_TARGETS, "", "Draw targets", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_DRAW_RANGE, "", "Draw range", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_DRAW_BLOCK_RANGE_AI, "", "Draw block range PVE", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_DRAW_BLOCK_RANGE_PVP, "", "Draw block range PVP", "Melee");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_WEAPON_DEBUG_MENU, "Weapon", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool( DiagMenuIDs.DM_GUN_PARTICLES, "", "Enable gun particles", "Weapon", true );
				DiagMenu.RegisterBool(DiagMenuIDs.DM_WEAPON_DEBUG_ENABLE, "", "Show Debug", "Weapon");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_WEAPON_AIM_NOISE, "", "Weapon Sway", "Weapon");
				DiagMenu.SetValue(DiagMenuIDs.DM_WEAPON_AIM_NOISE, true);
				DiagMenu.RegisterBool(DiagMenuIDs.DM_WEAPON_ALLOW_RECOIL, "", "Procedural Recoil", "Weapon");
				DiagMenu.SetValue(DiagMenuIDs.DM_WEAPON_ALLOW_RECOIL, true);
				DiagMenu.RegisterBool(DiagMenuIDs.DM_WEAPON_UNLIMITED, "lalt+9", "Unlimited Ammo", "Weapon");
				DiagMenu.RegisterItem(DiagMenuIDs.DM_WEAPON_BURST, "lctrl+0", "Burst Version", "Weapon", "Holt, Press");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_TRIGGER_MENU, "Triggers", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SHOW_AREADMG_TRIGGER, "", "Show Triggers", "Triggers");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_SHOW_PLAYER_TOUCHTRIGGER, "", "Show Player Touch", "Triggers");
				#ifdef TRIGGER_DEBUG_BASIC
				DiagMenu.SetValue(DiagMenuIDs.DM_SHOW_AREADMG_TRIGGER, true);
				DiagMenu.SetValue(DiagMenuIDs.DM_SHOW_PLAYER_TOUCHTRIGGER, true);
				#endif
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_LOGS_MENU, "Logs", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_LOG_ACTIONS, "", "Log Actions", "Logs");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_LOG_SYMPTOM, "", "Log Symptoms", "Logs");
				DiagMenu.RegisterMenu(DiagMenuIDs.DM_LOGS_MENU_INVENTORY, "InventoryLog", "Logs");
					//---------------------------------------------------------------
					// LEVEL 3
					//---------------------------------------------------------------
					DiagMenu.RegisterBool(DiagMenuIDs.DM_LOG_INVENTORY_MOVE, "", "Log Items move", "InventoryLog");
					DiagMenu.RegisterBool(DiagMenuIDs.DM_LOG_INVENTORY_RESERVATION, "", "Log Reservations", "InventoryLog");
					DiagMenu.RegisterBool(DiagMenuIDs.DM_LOG_INVENTORY_HFSM, "", "Log HandFSM", "InventoryLog");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_FINISHERS_MENU, "Finishers", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterItem(DiagMenuIDs.DM_FINISHERS_FINISHER_FORCED, "", "Forced Finisher: ", "Finishers", "None,Liver,Neck");
				DiagMenu.SetValue(DiagMenuIDs.DM_FINISHERS_FINISHER_FORCED,0);
	}
	
	void Update(float deltaT)
	{
		//---------------------------------------------
		// every system can handle their own actions locally(within the system itself),
		// this section is for systems which don't have client side representation,
		// or don't have their own tick, or for whatever other reason you prefer to put the logic here
		// PUT ONLY METHOD CALLS HERE, NO CODE !!
		// PUT ONLY METHOD CALLS HERE, NO CODE !!
		//---------------------------------------------
			
		CheckModifiers();//should modifiers be ticking
		CheckKillPlayer();//should i kill myself ?
		CheckCraftingDebugActions();
		CheckItemDebugActions();
		CheckAgentInjectActions();
		CheckGenerateRecipeCache();
		CheckDayzPlayerMenu();
		CheckSoftSkillsDebug();
		CheckSoftSkillsState();
		CheckSoftSkillsModel();
		CheckGunParticles();
		CheckSpecialtyLevel();
		CheckBloodyHands();
		CheckLifeSpanPlaytimeUpdate();
		CheckPermanentCrossHairUpdate();
		CheckToggleHud();
		CheckShowWeaponDebug();
		CheckInvincibility();
		CheckStamina();
		CheckAimNoise();
		CheckProceduralRecoil();
		CheckUnlimitedAmmo();
		CheckCraftingDump();
		CheckEnvironmentStats();
		CheckDisplayMenu();
		CheckDrawCheckerboard();
		CheckBulletImpact();
		CheckShockImpact();
		CheckTargetableByAI();
		CheckPresenceNotifierDebug();
		CheckGoUnconscious();
		CheckGoUnconsciousDelayed();
		CheckSimulateNULLPointer();
		CheckSimulateDivisionByZero();
		CheckSimulateInfiniteLoop();
		CheckSimulateErrorFunction();
		CheckShowBleedingSources();
		CheckDisableBloodLoss();
		CheckActivateAllBS();
		CheckReloadBS();
		CheckActivateBleedingSource();
		CheckQuickRestrain();
		CheckQuickFishing();
		CheckHairLevel();
		CheckHairHide();
		CheckPersonalLight();
		CheckCamShake();
		CheckVehicleGetOutBox();
		CheckDoActionLogs();
		CheckDoWeaponLogs();
		CheckDoInventoryLogs();
		CheckDoSymptomLogs();
		CheckPlayerReset();
		CheckInventoryAccess();
		CheckFixItems();
		CheckPlugArrows();
		CheckBurst();
		CheckBreathVapor();
		CheckHitDirection();
		CheckFinisherOverride();
		CheckInputSuppression();
		CheckFreezeEntity();
	}
	//---------------------------------------------
	void CheckBreathVapor()
	{
		ENABLE_BREATH_VAPOR = DiagMenu.GetBool(DiagMenuIDs.DM_BREATH_VAPOR_LVL);
	}	
	//---------------------------------------------
	void CheckPlayerReset()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_RESET) || DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_RESET_MAX))
		{
		
			if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_RESET) )
			{
				CachedObjectsParams.PARAM1_BOOL.param1 = false;
				
			}
			
			if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_RESET_MAX) )
			{
				CachedObjectsParams.PARAM1_BOOL.param1 = true;
				
			}
			
			
			if (GetGame() && GetGame().GetPlayer()) 
			{
				GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_PLAYER_RESET, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
				DiagMenu.SetValue(DiagMenuIDs.DM_PLAYER_RESET, false);
				DiagMenu.SetValue(DiagMenuIDs.DM_PLAYER_RESET_MAX, false);
			}
		}
	}	
	void CheckInventoryAccess()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_INVENTORY_ACCESS) )
		{
			if (!m_AllowInventoryAccess)
			{
				SendInventoryAccessRPC(true);
				PlayerBase.DEBUG_INVENTORY_ACCESS = true;
				m_AllowInventoryAccess = true;
			}
		}
		else
		{
			if (m_AllowInventoryAccess)
			{
				PlayerBase.DEBUG_INVENTORY_ACCESS = false;
				m_AllowInventoryAccess = false;
			}
		}
	}	
	void CheckFixItems()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_FIX_ITEMS) )
		{
			if (GetGame() && GetGame().GetPlayer()) 
				GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_ITEMS_FIX, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
			DiagMenu.SetValue(DiagMenuIDs.DM_FIX_ITEMS, false);
		}
	}
	
	//---------------------------------------------
	void CheckModifiers()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_CHEATS_MODIFIERS_ENABLE) )
		{
			if (!m_EnableModifiers)
			{
				SendModifiersRPC(true);
				m_EnableModifiers = true;
			}
		}
		else
		{
			if (m_EnableModifiers)
			{
				SendModifiersRPC(false);
				m_EnableModifiers = false;
			}
		}
	}
	//---------------------------------------------
	void CheckUnlimitedAmmo()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_WEAPON_UNLIMITED) )
		{
			if (!m_EnableUnlimitedAmmo)
			{
				SendUnlimitedRPC(true);
				m_EnableUnlimitedAmmo = true;
				ItemBase.SetDebugActionsMask( ItemBase.GetDebugActionsMask() | DebugActionType.UNLIMITED_AMMO );
			}
		}
		else
		{
			if (m_EnableUnlimitedAmmo)
			{
				SendUnlimitedRPC(false);
				m_EnableUnlimitedAmmo = false;
				ItemBase.SetDebugActionsMask( ItemBase.GetDebugActionsMask() & (~DebugActionType.UNLIMITED_AMMO) );
			}
		}
	}	
	//---------------------------------------------
	void CheckDisableBloodLoss()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_DISABLE_BLOOD_LOSS) )
		{
			if (!m_DisableBloodLoss)
			{
				SendDisableBloodLossRPC(true);
				m_DisableBloodLoss = true;
			}
		}
		else
		{
			if (m_DisableBloodLoss)
			{
				SendDisableBloodLossRPC(false);
				m_DisableBloodLoss = false;
			}
		}
	}
	//---------------------------------------------
	void CheckActivateAllBS()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_ACTIVATE_ALL_BS) )
		{
			SendActivateAllBSRPC();
			DiagMenu.SetValue(DiagMenuIDs.DM_ACTIVATE_ALL_BS, false);//to prevent constant RPC calls, switch back to false
		}
	}	
	//---------------------------------------------
	void CheckReloadBS()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_BS_RELOAD) )
		{
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			if (player && player.GetBleedingManagerRemote())
			{
				player.GetBleedingManagerRemote().Reload();
			}
			DiagMenu.SetValue(DiagMenuIDs.DM_BS_RELOAD, false);//to prevent constant RPC calls, switch back to false
		}
	}
	//---------------------------------------------
	void CheckGenerateRecipeCache()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_CRAFTING_GENERATE) )
		{
			PluginRecipesManager plugin_recipes_manager;
			Class.CastTo(plugin_recipes_manager, GetPlugin(PluginRecipesManager));
			plugin_recipes_manager.CallbackGenerateCache();
			DiagMenu.SetValue(DiagMenuIDs.DM_PLAYER_CRAFTING_GENERATE, false);
		}
	}
	//---------------------------------------------
	void CheckShowBleedingSources()
	{
		PlayerBase player;
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_SHOW_BLEEDING_SOURCES) )
		{
			if ( !m_ShowBleedingSources )
			{
				player = PlayerBase.Cast(GetGame().GetPlayer());
				player.GetBleedingManagerRemote().SetDiag(true);
				m_ShowBleedingSources = true;
			}
		}
		else
		{
			if ( m_ShowBleedingSources )
			{
				player = PlayerBase.Cast(GetGame().GetPlayer());
				player.GetBleedingManagerRemote().SetDiag(false);
				m_ShowBleedingSources = false;
			}
		}
	}
	//---------------------------------------------
	void CheckKillPlayer()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_CHEATS_PLAYER_INSTAKILL) )
		{
			SendKillPlayerRPC();
			DiagMenu.SetValue(DiagMenuIDs.DM_CHEATS_PLAYER_INSTAKILL, false);//to prevent constant RPC calls, switch back to false
		}
	}
	//---------------------------------------------
	void CheckGoUnconscious()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_GO_UNCONSCIOUS) )
		{
			SendGoUnconsciousRPC(false);
			DiagMenu.SetValue(DiagMenuIDs.DM_GO_UNCONSCIOUS, false);//to prevent constant RPC calls, switch back to false
		}
	}
	//---------------------------------------------
	void CheckGoUnconsciousDelayed()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_GO_UNCONSCIOUS_DELAYED) )
		{
			SendGoUnconsciousRPC(true);
			DiagMenu.SetValue(DiagMenuIDs.DM_GO_UNCONSCIOUS_DELAYED, false);//to prevent constant RPC calls, switch back to false
		}
	}
	
	void CheckHairLevel()
	{
		int value = DiagMenu.GetRangeValue(DiagMenuIDs.DM_HAIR_LEVEL);
		if (value != m_HairLevelSelected)
		{
			m_HairLevelSelected = value;
			//SendSetHairLevelRPC();
			DiagMenu.SetRangeValue(DiagMenuIDs.DM_HAIR_LEVEL, value);//to prevent constant RPC calls, switch back to false
		}
	}
	
	void CheckHairHide()
	{
		int value = DiagMenu.GetBool(DiagMenuIDs.DM_HAIR_LEVEL_HIDE);
		bool hide_all = DiagMenu.GetBool(DiagMenuIDs.DM_HAIR_HIDE_ALL);
		if (hide_all)
		{
			SendSetHairLevelHideRPC(-1,value);
			DiagMenu.SetValue(DiagMenuIDs.DM_HAIR_HIDE_ALL, false);//to prevent constant RPC calls, switch back to false
		}
		else if (value)
		{
			SendSetHairLevelHideRPC(m_HairLevelSelected,value);
			DiagMenu.SetValue(DiagMenuIDs.DM_HAIR_LEVEL_HIDE, false);//to prevent constant RPC calls, switch back to false
		}
	}
	
	void CheckPersonalLight()
	{
		int value = DiagMenu.GetBool(DiagMenuIDs.DM_DISABLE_PERSONAL_LIGHT);
		PlayerBaseClient.m_PersonalLightDisabledByDebug = value;
		PlayerBaseClient.UpdatePersonalLight();
	}
	
	//---------------------------------------------
	void CheckItemDebugActions()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_ITEM_DEBUG_ACTIONS_SHOW) )
		{
			if ( (ItemBase.GetDebugActionsMask() & DebugActionType.GENERIC_ACTIONS) == 0 )
			{
				int mask = (ItemBase.GetDebugActionsMask() | DebugActionType.GENERIC_ACTIONS);
				ItemBase.SetDebugActionsMask(mask);
				SendDebugActionsRPC();
			}
		}
		else
		{
			if ( (ItemBase.GetDebugActionsMask() & DebugActionType.GENERIC_ACTIONS) )
			{
				int mask2 = (ItemBase.GetDebugActionsMask() & (~DebugActionType.GENERIC_ACTIONS));
				ItemBase.SetDebugActionsMask(mask2);
				SendDebugActionsRPC();
			}
		}
	}

	void CheckBulletImpact()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_BULLET_IMPACT) )
		{
			PlayerBase pl_player = PlayerBase.Cast(GetGame().GetPlayer() );
			CachedObjectsParams.PARAM2_FLOAT_FLOAT.param1 = 32;
			CachedObjectsParams.PARAM2_FLOAT_FLOAT.param2 = 0.3;
			pl_player.SpawnDamageDealtEffect2(CachedObjectsParams.PARAM2_FLOAT_FLOAT);
			DiagMenu.SetValue(DiagMenuIDs.DM_BULLET_IMPACT, false);
		}
		else
		{
			
		}
	}
	
	float tmp = 0.25;
	void CheckShockImpact()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_SHOCK_IMPACT) )
		{
			PlayerBase pl_player = PlayerBase.Cast(GetGame().GetPlayer() );
			if (tmp >= 1.0) 	tmp = 0.25;
			pl_player.SpawnShockEffect(tmp);
			tmp += 0.25;
			DiagMenu.SetValue(DiagMenuIDs.DM_SHOCK_IMPACT, false);
		}
	}
	
	void CheckHitDirection()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		bool enabled = DiagMenu.GetBool(DiagMenuIDs.DM_HIT_INDICATION_ENABLE_DEBUG);
		if (enabled != m_HitIndicationDebugEnabled)
		{
			if (!enabled)
			{
				HitDirectionEffectBase.CheckValues();
				if (player)
				{
					player.SetHitPPEEnabled(CfgGameplayHandler.GetHitIndicationPPEEnabled());
				}
			}
			//Print("DbgHitDirection | PluginDiagMenu | CheckHitDirection | enabled: " + enabled);
			m_HitIndicationDebugEnabled = enabled;
		}
		
		if ( m_HitIndicationDebugEnabled )
		{
			HitDirectionEffectBase.m_Mode = DiagMenu.GetValue(DiagMenuIDs.DM_HIT_INDICATION_MODE);
			HitDirectionEffectBase.m_ID = DiagMenu.GetValue(DiagMenuIDs.DM_HIT_INDICATION_TYPE);
			HitDirectionEffectBase.m_DurationMax = DiagMenu.GetRangeValue(DiagMenuIDs.DM_HIT_INDICATION_DURATION);
			HitDirectionEffectBase.m_BreakPointBase = DiagMenu.GetRangeValue(DiagMenuIDs.DM_HIT_INDICATION_BREAKPOINT);
			HitDirectionEffectBase.m_DistanceAdjust = DiagMenu.GetRangeValue(DiagMenuIDs.DM_HIT_INDICATION_DISTANCE);
			HitDirectionEffectBase.m_RotationOverride = DiagMenu.GetRangeValue(DiagMenuIDs.DM_HIT_INDICATION_ROTATION);
			HitDirectionEffectBase.m_Scatter = DiagMenu.GetRangeValue(DiagMenuIDs.DM_HIT_INDICATION_SCATTER);
		}
		
		if (player)
		{
			if ( DiagMenu.GetBool(DiagMenuIDs.DM_HIT_INDICATION_SPAWN_HIT) )
			{
				if ( m_HitIndicationDebugEnabled )
				{
					GetGame().GetMission().GetHud().SpawnHitDirEffect(player,DiagMenu.GetRangeValue(DiagMenuIDs.DM_HIT_INDICATION_SPAWN_HIT_DIRECTION),1.0);
				}
				DiagMenu.SetValue(DiagMenuIDs.DM_HIT_INDICATION_SPAWN_HIT,false);
			}
			
			if ( m_HitIndicationDebugEnabled )
			{
				player.SetHitPPEEnabled(!DiagMenu.GetBool(DiagMenuIDs.DM_HIT_INDICATION_DISABLE_PPE));
			}
		}
	}
	
	void CheckFinisherOverride()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		int value = DiagMenu.GetValue(DiagMenuIDs.DM_FINISHERS_FINISHER_FORCED) - 1;
		if ( player && player.GetMeleeFightLogic() && player.GetMeleeFightLogic().m_DbgForcedFinisherType != value )
		{
			player.GetMeleeFightLogic().m_DbgForcedFinisherType = value;
			SendForceFinisherType(value);
		}
	}
	
	void CheckTargetableByAI()
	{
		PlayerBase pl_player = PlayerBase.Cast(GetGame().GetPlayer());
		if (pl_player)
		{
			bool value = DiagMenu.GetBool(DiagMenuIDs.DM_TARGETABLE_BY_AI);
			if ( value != pl_player.m_CanBeTargetedDebug )
			{
				pl_player.m_CanBeTargetedDebug = value;
				SendTargetabilityRPC(value);
			}
		}
	}
	
	void CheckInputSuppression()
	{
		Mission mission = GetGame().GetMission();
		if (mission)
		{
			bool value = DiagMenu.GetBool(DiagMenuIDs.DM_SUPPRESSNEXTFRAMEINPUTS);
			if ( value != mission.GetInputSuppression() )
			{
				mission.SetInputSuppression(value);
			}
		}
	}
	
	
	void CheckFreezeEntity()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_FREEZE_ENTITY) )
		{
			EntityAI entity;
			if(PluginSceneManager.GetInstance() && PluginSceneManager.GetInstance().IsOpened())
			{
				// Scene Editor
				SceneObject scene_obj = PluginSceneManager.GetInstance().GetSelectedSceneObject();

				if (scene_obj)
				{
					entity = scene_obj.GetObject();
				}
			}
			else
			{
				// FreeDebugCamera
				FreeDebugCamera camera = FreeDebugCamera.GetInstance();
				entity = EntityAI.Cast(camera.GetCrosshairObject());
			}
			
			if (entity)
				entity.DisableSimulation(!entity.GetIsSimulationDisabled());
			DiagMenu.SetValue(DiagMenuIDs.DM_FREEZE_ENTITY, false);
		}
	}
	
	bool GetXboxCursor()
	{
		return DiagMenu.GetBool(DiagMenuIDs.DM_XBOX_CURSOR);
	}
	
	//---------------------------------------------
	void CheckAgentInjectActions()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_AGENTS_INJECTS_SHOW) )
		{
			if ( (ItemBase.GetDebugActionsMask() & DebugActionType.PLAYER_AGENTS) == 0 )
			{
				int mask = (ItemBase.GetDebugActionsMask() | DebugActionType.PLAYER_AGENTS);
				ItemBase.SetDebugActionsMask(mask);	
				SendDebugActionsRPC();
			}
		}
		else
		{
			if ( ItemBase.GetDebugActionsMask() & DebugActionType.PLAYER_AGENTS )
			{
				int mask2 = (ItemBase.GetDebugActionsMask() & (~DebugActionType.PLAYER_AGENTS));
				ItemBase.SetDebugActionsMask(mask2);
				SendDebugActionsRPC();
			}
		}
	}
	//---------------------------------------------
	/*
	void CheckAgentInjectActions()
	{
		if ( DiagMenu.GetBool(DM_PLAYER_AGENTS_INJECTS_SHOW) )
		{
			if ( !ItemBase.IsShowInjectActions() )
			{
				ItemBase.SetShowInjectActions(true);		
			}
		}
		else
		{
			if ( ItemBase.IsShowInjectActions() )
			{
				ItemBase.SetShowInjectActions(false);		
			}
		}
	}
	*/
	
	//---------------------------------------------
	void CheckCraftingDump()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_CRAFTING_DUMP) )
		{
			PluginRecipesManager plugin_recipes_manager;
			Class.CastTo(plugin_recipes_manager, GetPlugin(PluginRecipesManager));
			plugin_recipes_manager.GenerateHumanReadableRecipeList();
			DiagMenu.SetValue(DiagMenuIDs.DM_PLAYER_CRAFTING_DUMP, false);
		}
	}
	//---------------------------------------------
	
	
	//---------------------------------------------
	void CheckCraftingDebugActions()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_CRAFTING_DEBUG_ACTIONS_ENABLE) )
		{
			if (!m_ShowCraftingDebugActions)
			{
				SendDebugCraftingRPC(true);
				PluginRecipesManager plugin_recipes_manager;
				Class.CastTo(plugin_recipes_manager, GetPlugin(PluginRecipesManager));
				plugin_recipes_manager.SetEnableDebugCrafting(true);
				m_ShowCraftingDebugActions = true;
			}
		}
		else
		{
			if (m_ShowCraftingDebugActions)
			{
				SendDebugCraftingRPC(false);
				PluginRecipesManager plugin_recipes_manager2;
				Class.CastTo(plugin_recipes_manager2, GetPlugin(PluginRecipesManager));
				plugin_recipes_manager2.SetEnableDebugCrafting(false);
				m_ShowCraftingDebugActions = false;
			}
		}
	}
	//---------------------------------------------
	void CheckInvincibility()
	{
		if(DiagMenu.GetValue(DiagMenuIDs.DM_CHEATS_INVINCIBILITY) == 1)
		{
			if (m_IsInvincible != 1)	
			{
				SendInvincibilityRPC(1);
				m_IsInvincible = 1;
			}
		}
		else if(DiagMenu.GetValue(DiagMenuIDs.DM_CHEATS_INVINCIBILITY) == 2)
		{
			if (m_IsInvincible != 2)	
			{
				SendInvincibilityRPC(2);
				m_IsInvincible = 2;
			}
		}
		else
		{
			if(m_IsInvincible != 0)
			{
				SendInvincibilityRPC(0);
				m_IsInvincible = 0;
			}
		}
		
	}
	//---------------------------------------------
	void CheckBurst()
	{
		int value = DiagMenu.GetValue(DiagMenuIDs.DM_WEAPON_BURST);
		if( m_Burst != value )
		{
			m_Burst = value;
			SendBurstRPC(value);
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			player.GetWeaponManager().SetBurstOption(m_Burst);
		}
	} 

	//---------------------------------------------
	void CheckStamina()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_CHEATS_STAMINA_DISABLE) )
		{
			if (!m_StaminaDisabled)	
			{
				m_StaminaDisabled = true;
			}
		}
		else
		{
			if (m_StaminaDisabled)	
			{
				m_StaminaDisabled = false;
			}
		}
	}

	//---------------------------------------------
	void CheckLogPlayerStats()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_STATS_LOG_ENABLE) )
		{
			if (!m_LogPlayerStats)	
			{
				SendLogPlayerStatsRPC(true);
				m_LogPlayerStats = true;
			}
		}
		else
		{
			if (m_LogPlayerStats)	
			{
				SendLogPlayerStatsRPC(false);
				m_LogPlayerStats = false;
			}
		}
	}

	//---------------------------------------------
	void CheckSoftSkillsDebug()
	{
		PlayerBase player;
		SoftSkillManagerDebug m_DebugWindow;

		if ( DiagMenu.GetBool( DiagMenuIDs.DM_SOFT_SKILLS_SHOW_DEBUG ) )
		{
			if (!m_SoftSkillsDebug)	
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.GetSoftSkillsManager().CreateDebugWindow( true );
				SoftSkillsShowDebugRPC(true);
				m_SoftSkillsDebug = true;
			}
		}
		else
		{
			if (m_SoftSkillsDebug)	
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.GetSoftSkillsManager().CreateDebugWindow( false );
				SoftSkillsShowDebugRPC(false);
				m_SoftSkillsDebug = false;
			}
		}
	}

	//---------------------------------------------
	void CheckSoftSkillsState()
	{
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_SOFT_SKILLS_TOGGLE_STATE ) )
		{
			if (!m_SoftSkillsState)	
			{
				SoftSkillsToggleStateRPC(true);
				m_SoftSkillsState = true;
			}
		}
		else
		{
			if (m_SoftSkillsState)	
			{
				SoftSkillsToggleStateRPC(false);
				m_SoftSkillsState = false;
			}
		}
	}
		
	//---------------------------------------------
	void CheckGunParticles()
	{
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_GUN_PARTICLES ) )
		{
			if (!m_GunParticlesState)
			{
				GunParticlesToggleStateRPC(false);
				m_GunParticlesState = true;
			}
		}
		else
		{
			if (m_GunParticlesState)
			{
				GunParticlesToggleStateRPC(true);
				m_GunParticlesState = false;
			}
		}
	}

	//---------------------------------------------
	void CheckSoftSkillsModel()
	{
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_SOFT_SKILLS_TOGGLE_MODEL ) )
		{
			if (!m_SoftSkillsModel)	
			{
				SoftSkillsToggleModelRPC(true);
				m_SoftSkillsModel = true;
			}
		}
		else
		{
			if (m_SoftSkillsModel)	
			{
				SoftSkillsToggleModelRPC(false);
				m_SoftSkillsModel = false;
			}
		}
	}

	//---------------------------------------------
	void CheckSpecialtyLevel()
	{
		float specialty_value = DiagMenu.GetRangeValue( DiagMenuIDs.DM_SOFT_SKILLS_SPECIALTY_VALUE );

		if ( m_SpecialtyLevel != specialty_value )
		{
			SoftSkillsSetSpecialtyRPC( specialty_value );
			m_SpecialtyLevel = specialty_value;
		}
	}
	
	//---------------------------------------------
	void CheckActivateBleedingSource()
	{
		int bleeding_source = DiagMenu.GetRangeValue( DiagMenuIDs.DM_ACTIVATE_SOURCE );
		
		if ( bleeding_source != m_BleedingSourceRequested )
		{
			SendActivateBleedingSource(bleeding_source);
			m_BleedingSourceRequested = bleeding_source;
		}
		
	}

	//---------------------------------------------
	void CheckBloodyHands()
	{
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_BLOODY_HANDS ) )
		{
			if (!m_BloodyHands)	
			{
				LifespanBloodyHandsRPC(true);
				m_BloodyHands = true;
			}
		}
		else
		{
			if (m_BloodyHands)	
			{
				LifespanBloodyHandsRPC(false);
				m_BloodyHands = false;
			}
		}
	}

	//---------------------------------------------
	void CheckLifeSpanPlaytimeUpdate()
	{
		float lifespan_level = DiagMenu.GetRangeValue( DiagMenuIDs.DM_LIFESPAN_PLAYTIME_UPDATE );

		if ( m_LifespanLevel != lifespan_level )
		{
			LifespanPlaytimeUpdateRPC( lifespan_level );
			m_LifespanLevel = lifespan_level;
		}
	}
	//---------------------------------------------	
	void CheckPermanentCrossHairUpdate()
	{
		PluginPermanentCrossHair crosshair;
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_PERMANENT_CROSSHAIR ) )
		{
			if (!m_PermanentCrossHair)	
			{
				Class.CastTo(crosshair, GetPlugin( PluginPermanentCrossHair ));
				crosshair.SwitchPermanentCrossHair(true);
				m_PermanentCrossHair = true;
			}
		}
		else
		{
			if (m_PermanentCrossHair)	
			{
				Class.CastTo(crosshair, GetPlugin( PluginPermanentCrossHair ));
				crosshair.SwitchPermanentCrossHair(false);
				m_PermanentCrossHair = false;
			}
		}
	}
	
	//---------------------------------------------		
	void CheckVehicleGetOutBox()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_SHOW_VEHICLE_GETOUT_BOX ) )
		{
			if (m_VehicleFreeAreaBox)
			{
				m_VehicleFreeAreaBox.Destroy();
				m_VehicleFreeAreaBox = null;
			}
			
			HumanCommandVehicle hcv = GetGame().GetPlayer().GetCommand_Vehicle();
			if (!hcv)
				return;
			m_VehicleFreeAreaBox = hcv.GetTransport().DebugFreeAreaAtDoor( hcv.GetVehicleSeat() );
		}
		else if (m_VehicleFreeAreaBox)
		{
			m_VehicleFreeAreaBox.Destroy();
			m_VehicleFreeAreaBox = null;
		}
	}
	
	//---------------------------------------------	
	void CheckToggleHud()
	{
		Hud hud;
			
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_TOGGLE_HUD ) )
		{
			if (!m_ToggleHud)	
			{
				hud = GetGame().GetMission().GetHud();
				hud.Show( m_ToggleHud );
				GetGame().GetMission().HideCrosshairVisibility();
				m_ToggleHud = true;
			}
		}
		else
		{
			if (m_ToggleHud)	
			{
				hud = GetGame().GetMission().GetHud();
				hud.Show( m_ToggleHud );
				GetGame().GetMission().RefreshCrosshairVisibility();
				m_ToggleHud = false;
			}
		}
	}

	//---------------------------------------------	
	void CheckShowWeaponDebug()
	{
		PlayerBase player;
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_WEAPON_DEBUG_ENABLE ) )
		{
			if (!m_ShowingWeaponDebug)	
			{

				Class.CastTo(player, GetGame().GetPlayer());
				player.ShowWeaponDebug(true);
				m_ShowingWeaponDebug = true;
			}
		}
		else
		{
			if (m_ShowingWeaponDebug)	
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.ShowWeaponDebug(false);
				m_ShowingWeaponDebug = false;
			}
		}
	}
	//---------------------------------------------	
	void CheckProceduralRecoil()
	{
		DayZPlayerImplement player;
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_WEAPON_ALLOW_RECOIL ) )
		{
			if (!m_ProceduralRecoilEnabled)
			{
				//Class.CastTo(player, GetGame().GetPlayer());
				//player.GetAimingModel().SetProceduralRecoilEnabled(true);
				SendProceduralRecoilRPC(true);
				m_ProceduralRecoilEnabled = true;
			}
		}
		else
		{
			if (m_ProceduralRecoilEnabled)	
			{
				//Class.CastTo(player, GetGame().GetPlayer());
				//player.GetAimingModel().SetProceduralRecoilEnabled(false);
				SendProceduralRecoilRPC(false);
				m_ProceduralRecoilEnabled = false;
			}
		}
	}
	//---------------------------------------------	
	void CheckQuickRestrain()
	{
		
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_QUICK_RESTRAIN ) )
		{
			if (!m_EnableQuickRestrain)
			{
				SendEnableQuickRestrainRPC(true);
				m_EnableQuickRestrain = true;
			}
		}
		else
		{
			if (m_EnableQuickRestrain)	
			{
				SendEnableQuickRestrainRPC(false);
				m_EnableQuickRestrain = false;
			}
		}
	}
	
	//---------------------------------------------	
	void CheckQuickFishing()
	{
		
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_QUICK_FISHING ) )
		{
			if (!m_EnableQuickFishing)
			{
				SendEnableQuickFishingRPC(true);
				m_EnableQuickFishing = true;
			}
		}
		else
		{
			if (m_EnableQuickFishing)	
			{
				SendEnableQuickFishingRPC(false);
				m_EnableQuickFishing = false;
			}
		}
	}
	
	//---------------------------------------------	
	
	void CheckCamShake()
	{
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_CAM_SHAKE ) )
		{
			GetGame().GetPlayer().GetCurrentCamera().SpawnCameraShake(1,1,15,4);
			DiagMenu.SetValue(DiagMenuIDs.DM_CAM_SHAKE, false);
		}
		

	}
	//---------------------------------------------	
	void CheckAimNoise()
	{
		DayZPlayerImplement player;
		if ( DiagMenu.GetBool( DiagMenuIDs.DM_WEAPON_AIM_NOISE ) )
		{
			if (!m_AimNoiseEnabled)
			{

				Class.CastTo(player, GetGame().GetPlayer());
				player.GetAimingModel().SetAimNoiseAllowed(true);
				m_AimNoiseEnabled = true;
			}
		}
		else
		{
			if (m_AimNoiseEnabled)	
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.GetAimingModel().SetAimNoiseAllowed(false);
				m_AimNoiseEnabled = false;
			}
		}
	}

	//---------------------------------------------
	void CheckDayzPlayerMenu()
	{
		int	actMenuValue = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_SHOWDEBUGPLUGIN);
		// Print("Debug Menu:" + actMenuValue.ToString());
		if (actMenuValue != m_DayzPlayerDebugMenu)
		{
			if (g_Plugins != NULL)
			{
				PluginDayzPlayerDebug pd;
				Class.CastTo(pd, g_Plugins.GetPluginByType(PluginDayzPlayerDebug));
				if (pd != NULL)
				{
					pd.ToggleDebugWindowSetMode(actMenuValue);
					//Print("DayzPlayerDebug - switched to:" + actMenuValue.ToString());
				}
			}
			m_DayzPlayerDebugMenu = actMenuValue;
		}

	}
	//---------------------------------------------
	
	void CheckDisplayMenu()
	{
		PluginRemotePlayerDebugClient plugin_remote_client;
		int value = DiagMenu.GetRangeValue( DiagMenuIDs.DM_DISPLAY_PLAYER_INFO );
		
		if ( value != 0 )
		{
			if ( m_DisplayPlayerInfo != value )
			{
				plugin_remote_client = PluginRemotePlayerDebugClient.Cast( GetPlugin(PluginRemotePlayerDebugClient) );
				plugin_remote_client.RequestPlayerInfo( PlayerBase.Cast(GetGame().GetPlayer()), value );
				
				m_DisplayPlayerInfo = value;
			}
		}
		else
		{
			if ( m_DisplayPlayerInfo != 0 )	
			{
				plugin_remote_client = PluginRemotePlayerDebugClient.Cast( GetPlugin(PluginRemotePlayerDebugClient) );
				plugin_remote_client.RequestPlayerInfo( PlayerBase.Cast(GetGame().GetPlayer()), 0 );
				
				m_DisplayPlayerInfo = 0;
			}
		}
	}

	void CheckEnvironmentStats()
	{
		if (DiagMenu.GetBool(DiagMenuIDs.DM_ENVIRONMENT_DEBUG_ENABLE))
		{
			if (!m_EnvironmentStats)
				m_EnvironmentStats = true;
		}
		else
		{
			if (m_EnvironmentStats)
				m_EnvironmentStats = false;
		}
	}
	
	void CheckDrawCheckerboard()
	{
		if (DiagMenu.GetBool(DiagMenuIDs.DM_DRAW_CHECKERBOARD))
		{
			if (!m_DrawCheckerboard) m_DrawCheckerboard = true;
		}
		else
		{
			if (m_DrawCheckerboard) m_DrawCheckerboard = false;
		}
	}

	void CheckPresenceNotifierDebug()
	{
		if (DiagMenu.GetBool(DiagMenuIDs.DM_PRESENCE_NOTIFIER_DBG))
		{
			if (!m_PresenceNotifierDebug) m_PresenceNotifierDebug = true;
		}
		else
		{
			if (m_PresenceNotifierDebug) m_PresenceNotifierDebug = false;
		}
	}
	
	//---------------------------------------------
	void CheckSimulateNULLPointer()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_SIMULATE_NULL_POINTER) )
		{
			SendSimulateNULLPointer();
			DiagMenu.SetValue(DiagMenuIDs.DM_SIMULATE_NULL_POINTER, false);//to prevent constant RPC calls, switch back to false
		}
	}
	//---------------------------------------------
	void CheckSimulateDivisionByZero()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_SIMULATE_DIVISION_BY_ZERO) )
		{
			SendSimulateDivisionByZero();
			DiagMenu.SetValue(DiagMenuIDs.DM_SIMULATE_DIVISION_BY_ZERO, false);//to prevent constant RPC calls, switch back to false
		}
	}
	//---------------------------------------------
	void CheckSimulateInfiniteLoop()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_SIMULATE_INFINITE_LOOP) )
		{
			SendSimulateInfiniteLoop();
			DiagMenu.SetValue(DiagMenuIDs.DM_SIMULATE_INFINITE_LOOP, false);//to prevent constant RPC calls, switch back to false
		}
	}
	//---------------------------------------------
	void CheckSimulateErrorFunction()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_SIMULATE_ERROR_FUNCTION) )
		{
			SendSimulateErrorFunction();
			DiagMenu.SetValue(DiagMenuIDs.DM_SIMULATE_ERROR_FUNCTION, false);//to prevent constant RPC calls, switch back to false
		}
	}
	//---------------------------------------------
	void CheckDoActionLogs()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_LOG_ACTIONS) )
		{
			if ( !m_DoActionLogs )
			{
				SendSetActionLogs(true);
				LogManager.ActionLogEnable(true);
				m_DoActionLogs = true;
			}
		}
		else
		{
			if ( m_DoActionLogs )
			{
				SendSetActionLogs(false);
				LogManager.ActionLogEnable(false);
				m_DoActionLogs = false;
			}
		}
	}
	//---------------------------------------------
	void CheckDoWeaponLogs()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_LOG_WEAPON) )
		{
			if ( !m_DoActionLogs )
			{
				SendSetWeaponLogs(true);
				LogManager.WeaponLogEnable(true);
				m_DoWeaponLogs = true;
			}
		}
		else
		{
			if ( m_DoActionLogs )
			{
				SendSetWeaponLogs(false);
				LogManager.WeaponLogEnable(false);
				m_DoWeaponLogs = false;
			}
		}
	}
	//---------------------------------------------
	void CheckDoInventoryLogs()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_LOG_INVENTORY_MOVE) )
		{
			if ( !m_DoInventoryMoveLogs )
			{
				SendSetInventoryMoveLogs(true);
				LogManager.InventoryMoveLogEnable(true);
				m_DoInventoryMoveLogs = true;
			}
		}
		else
		{
			if ( m_DoInventoryMoveLogs )
			{
				SendSetInventoryMoveLogs(false);
				LogManager.InventoryMoveLogEnable(false);
				m_DoInventoryMoveLogs = false;
			}
		}
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_LOG_INVENTORY_RESERVATION) )
		{
			if ( !m_DoInventoryReservationLogs )
			{
				SendSetInventoryReservationLogs(true);
				LogManager.InventoryReservationLogEnable(true);
				m_DoInventoryReservationLogs = true;
			}
		}
		else
		{
			if ( m_DoInventoryReservationLogs )
			{
				SendSetInventoryReservationLogs(false);
				LogManager.InventoryReservationLogEnable(false);
				m_DoInventoryReservationLogs = false;
			}
		}
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_LOG_INVENTORY_MOVE) )
		{
			if ( !m_DoInventoryHFSMLogs )
			{
				SendSetInventoryHFSMLogs(true);
				LogManager.InventoryHFSMLogEnable(true);
				m_DoInventoryHFSMLogs = true;
			}
		}
		else
		{
			if ( m_DoInventoryHFSMLogs )
			{
				SendSetInventoryHFSMLogs(false);
				LogManager.InventoryHFSMLogEnable(false);
				m_DoInventoryHFSMLogs = false;
			}
		}
	}
	//---------------------------------------------
	void CheckDoSymptomLogs()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_LOG_SYMPTOM) )
		{
			if ( !m_DoSymptomLogs )
			{
				SendSetSymptomLogs(true);
				LogManager.SymptomLogEnable(true);
				m_DoSymptomLogs = true;
			}
		}
		else
		{
			if ( m_DoSymptomLogs )
			{
				SendSetSymptomLogs(false);
				LogManager.SymptomLogEnable(false);
				m_DoSymptomLogs = false;
			}
		}
	}
	
	#ifdef DEVELOPER
	bool m_Active = false;
	void CheckPlugArrows()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_SHOW_PLUG_ARROWS) )
		{
			if ( m_Active == false )
			{
				for ( int i = 0; i < GetGame().m_EnergyManagerArray.Count(); i++)
				{
					if ( GetGame().m_EnergyManagerArray[i] )
						GetGame().m_EnergyManagerArray[i].SetDebugPlugs( DiagMenu.GetBool(DiagMenuIDs.DM_SHOW_PLUG_ARROWS) );
				}
				m_Active = true;
			}
		}
		else
		{
			for ( int j = 0; j < GetGame().m_EnergyManagerArray.Count(); j++)
			{
				if ( GetGame().m_EnergyManagerArray[j] )
					GetGame().m_EnergyManagerArray[j].SetDebugPlugs( DiagMenu.GetBool(DiagMenuIDs.DM_SHOW_PLUG_ARROWS) );
			}
			m_Active = false;
		}
	}
	#endif
	
	//---------------------------------------------
	void SendDebugActionsRPC()
	{
		CachedObjectsParams.PARAM1_INT.param1 = ItemBase.GetDebugActionsMask();
		if ( GetGame() && GetGame().GetPlayer() ) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_ITEM_DEBUG_ACTIONS, CachedObjectsParams.PARAM1_INT, true, GetGame().GetPlayer().GetIdentity() );
 	}
	
	//---------------------------------------------
	void SendEnableQuickRestrainRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player) player.SetQuickRestrain(enable);
		if ( GetGame() && GetGame().GetPlayer() ) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_ENABLE_QUICK_RESTRAIN, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	
	//---------------------------------------------
	void SendEnableQuickFishingRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player) player.SetQuickFishing(enable);
		if ( GetGame() && GetGame().GetPlayer() ) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_ENABLE_QUICK_FISHING, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	
	//---------------------------------------------
	void SendProceduralRecoilRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player) player.GetAimingModel().SetProceduralRecoilEnabled(enable);
		if ( GetGame() && GetGame().GetPlayer() ) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_WEAPON_PROC_RECOIL, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	
	//---------------------------------------------
	void SendActivateAllBSRPC()
	{
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_ACTIVATE_ALL_BS, NULL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendKillPlayerRPC()
	{
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_KILL_PLAYER, NULL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendInvincibilityRPC(int level)
	{
		CachedObjectsParams.PARAM1_INT.param1 = level;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_ENABLE_INVINCIBILITY, CachedObjectsParams.PARAM1_INT, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendTargetabilityRPC(bool value)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = value;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_TOGGLE_TARGETABLE, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendBurstRPC(int opt)
	{
		CachedObjectsParams.PARAM1_INT.param1 = opt;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_BURST_OPT, CachedObjectsParams.PARAM1_INT, true, GetGame().GetPlayer().GetIdentity() );
 	}		
	//---------------------------------------------
	void SendLogPlayerStatsRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_LOG_PLAYER_STATS, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendModifiersRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_DISABLE_MODIFIERS, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendInventoryAccessRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_INVENTORY_ACCESS, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	
	//---------------------------------------------
	void SendUnlimitedRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_UNLIMITED_AMMO, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendDisableBloodLossRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_DISABLE_BLOODLOOS, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendDebugCraftingRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_CRAFTING_DEBUG, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SoftSkillsShowDebugRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_SOFT_SKILLS_DEBUG_WINDOW, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SoftSkillsToggleStateRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_SOFT_SKILLS_TOGGLE_STATE, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void GunParticlesToggleStateRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_GUN_PARTICLES_TOGGLE, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SoftSkillsToggleModelRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_SOFT_SKILLS_TOGGLE_MODEL, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SoftSkillsSetSpecialtyRPC( float specialty_value )
	{
		CachedObjectsParams.PARAM1_FLOAT.param1 = specialty_value;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_SOFT_SKILLS_SET_SPECIALTY, CachedObjectsParams.PARAM1_FLOAT, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void LifespanBloodyHandsRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_LIFESPAN_BLOODY_HANDS, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void LifespanPlaytimeUpdateRPC( float playtime )
	{
		CachedObjectsParams.PARAM1_FLOAT.param1 = playtime;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_LIFESPAN_PLAYTIME_UPDATE, CachedObjectsParams.PARAM1_FLOAT, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendMeleeBlockStanceRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_MELEE_BLOCK_STANCE, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
	}
	//---------------------------------------------
	void SendMeleeFightRPC(bool enable)
	{
		CachedObjectsParams.PARAM1_BOOL.param1 = enable;
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_MELEE_FIGHT, CachedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
	}
	//---------------------------------------------
	void SendGoUnconsciousRPC(bool is_delayed)
	{
		Param1<bool> p1 = new Param1<bool>(is_delayed);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_GO_UNCONSCIOUS, p1, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendSimulateNULLPointer()
	{
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_SIMULATE_NULL_POINTER, NULL, true, GetGame().GetPlayer().GetIdentity() );
	}
	//---------------------------------------------
	void SendSimulateDivisionByZero()
	{
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_SIMULATE_DIVISION_BY_ZERO, NULL, true, GetGame().GetPlayer().GetIdentity() );
	}
	//---------------------------------------------
	void SendSimulateInfiniteLoop()
	{
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_SIMULATE_INFINITE_LOOP, NULL, true, GetGame().GetPlayer().GetIdentity() );
	}
	//---------------------------------------------
	void SendSimulateErrorFunction()
	{
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_SIMULATE_ERROR_FUNCTION, NULL, true, GetGame().GetPlayer().GetIdentity() );
	}
	
	void SendActivateBleedingSource(int bleeding_source)
	{
		Param1<int> p1 = new Param1<int>(bleeding_source);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_ACTIVATE_BS, p1, true, GetGame().GetPlayer().GetIdentity() );
	}
	//---------------------------------------------
	void SendSetHairLevelRPC()
	{
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_HAIR_LEVEL, NULL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	
	void SendSetHairLevelHideRPC(int level, bool value)
	{
		Param2<int,bool> p2 = new Param2<int,bool>(level,value);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_HAIR_LEVEL_HIDE, p2, true, GetGame().GetPlayer().GetIdentity() );
 	}
	
	void SendSetActionLogs(bool enable)
	{
		Param1<bool> p1 = new Param1<bool>(enable);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_DO_ACTION_LOGS, p1, true, GetGame().GetPlayer().GetIdentity() );
	}
	
	void SendSetWeaponLogs(bool enable)
	{
		Param1<bool> p1 = new Param1<bool>(enable);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_DO_WEAPON_LOGS, p1, true, GetGame().GetPlayer().GetIdentity() );
	}
	
	void SendSetSymptomLogs(bool enable)
	{
		Param1<bool> p1 = new Param1<bool>(enable);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_DO_SYMPTOMS_LOGS, p1, true, GetGame().GetPlayer().GetIdentity() );
	}
	
	void SendSetInventoryMoveLogs(bool enable)
	{
		Param1<bool> p1 = new Param1<bool>(enable);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_DO_INV_MOVE_LOGS, p1, true, GetGame().GetPlayer().GetIdentity() );
	}
	
	void SendSetInventoryReservationLogs(bool enable)
	{
		Param1<bool> p1 = new Param1<bool>(enable);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_DO_INV_RESERVATION_LOGS, p1, true, GetGame().GetPlayer().GetIdentity() );
	}
	
	void SendSetInventoryHFSMLogs(bool enable)
	{
		Param1<bool> p1 = new Param1<bool>(enable);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_DO_INV_HFSM_LOGS, p1, true, GetGame().GetPlayer().GetIdentity() );
	}
	
	void SendForceFinisherType(int value)
	{
		Param1<int> p1 = new Param1<int>(value);
		if (GetGame() && GetGame().GetPlayer()) 
			GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_FORCE_FINISHER, p1, true, GetGame().GetPlayer().GetIdentity() );
	}
	
	//---------------------------------------------
	void OnRPC(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		if ( GetGame().IsMultiplayer()  &&  GetGame().IsServer() )
		{
			switch(rpc_type)
			{
				case ERPCs.DEV_SIMULATE_NULL_POINTER:
					PlayerBase NULL_player = NULL;
					NULL_player.SetHealth("","", -1);		
				break;
				
				case ERPCs.DEV_SIMULATE_DIVISION_BY_ZERO:
					int zero = 0;
					int division_by_zero = 1/zero;
				break;
				
				case ERPCs.DEV_SIMULATE_INFINITE_LOOP:
					while( true )
					{
						Print("simulated infinite loop");
					}
				break;
				
				case ERPCs.DEV_SIMULATE_ERROR_FUNCTION:
					Error("Simulated error");
				break;
			}
		}
		
		bool enable;
		
		switch(rpc_type)
		{
			case ERPCs.DEV_ACTIVATE_BS:
					ctx.Read(CachedObjectsParams.PARAM1_INT);
					player.GetBleedingManagerServer().DebugActivateBleedingSource(CachedObjectsParams.PARAM1_INT.param1);
			break;
			
			case ERPCs.DEV_RPC_UNLIMITED_AMMO:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				enable = CachedObjectsParams.PARAM1_BOOL.param1;
				if (enable)
				{			
					ItemBase.SetDebugActionsMask( ItemBase.GetDebugActionsMask() | DebugActionType.UNLIMITED_AMMO );
				}
				else
				{
					ItemBase.SetDebugActionsMask( ItemBase.GetDebugActionsMask() & (~DebugActionType.UNLIMITED_AMMO) );			
				}
			break;
			
			case ERPCs.RPC_DISABLE_MODIFIERS:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				enable = CachedObjectsParams.PARAM1_BOOL.param1;
				player.SetModifiers( enable );
			break;
			
			case ERPCs.DEV_RPC_DISABLE_BLOODLOOS:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				player.GetBleedingManagerServer().SetBloodLoss( CachedObjectsParams.PARAM1_BOOL.param1 );
			break;
			
			case ERPCs.RPC_KILL_PLAYER:
				player.SetHealth("","", -1);
			break;	
			
			case ERPCs.DEV_ACTIVATE_ALL_BS:
				player.GetBleedingManagerServer().ActivateAllBS();
			break;

			case ERPCs.RPC_ENABLE_INVINCIBILITY:
				ctx.Read(CachedObjectsParams.PARAM1_INT);
				int level = CachedObjectsParams.PARAM1_INT.param1;
				if(level == 0)
				{
					player.SetAllowDamage(true);
					player.SetCanBeDestroyed(true);
				}
				else if(level == 1)
				{
					player.SetAllowDamage(true);
					player.SetCanBeDestroyed(false);
				}
				else if(level == 2)
				{
					player.SetAllowDamage(false);
				}
			break;
			
			case ERPCs.DEV_RPC_TOGGLE_TARGETABLE:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				bool value = CachedObjectsParams.PARAM1_BOOL.param1;
				player.m_CanBeTargetedDebug = value;
			break;
			
			case ERPCs.RPC_BURST_OPT:
				ctx.Read(CachedObjectsParams.PARAM1_INT);
				int opt = CachedObjectsParams.PARAM1_INT.param1;
				player.GetWeaponManager().SetBurstOption(opt);
			break;

			case ERPCs.RPC_ITEM_DEBUG_ACTIONS:
				ctx.Read(CachedObjectsParams.PARAM1_INT);
				int mask = CachedObjectsParams.PARAM1_INT.param1;
				ItemBase.SetDebugActionsMask(mask);
			break;
			
			case  ERPCs.RPC_WEAPON_PROC_RECOIL:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				enable = CachedObjectsParams.PARAM1_BOOL.param1;
				player.GetAimingModel().SetProceduralRecoilEnabled(enable);
			break;
			
			case ERPCs.RPC_ENABLE_QUICK_RESTRAIN:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				enable = CachedObjectsParams.PARAM1_BOOL.param1;
				player.SetQuickRestrain(enable);
			break;
			
			case ERPCs.RPC_ENABLE_QUICK_FISHING:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				enable = CachedObjectsParams.PARAM1_BOOL.param1;
				player.SetQuickFishing(enable);
			break;

			case ERPCs.RPC_LOG_PLAYER_STATS:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				player.GetPlayerStats().SetAllowLogs(CachedObjectsParams.PARAM1_BOOL.param1);
			break;

			case ERPCs.RPC_SOFT_SKILLS_TOGGLE_STATE:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				player.GetSoftSkillsManager().SetSoftSkillsState(CachedObjectsParams.PARAM1_BOOL.param1);
			break;

			case ERPCs.RPC_SOFT_SKILLS_DEBUG_WINDOW:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				enable = CachedObjectsParams.PARAM1_BOOL.param1;
				
				if ( enable )
				{
					player.GetSoftSkillsManager().StartSynchTimer();
				}
				else
				{
					player.GetSoftSkillsManager().StopSynchTimer();
					player.GetSoftSkillsManager().ResetDebugWindow();
				}
				
			break;
		
			case ERPCs.RPC_GUN_PARTICLES_TOGGLE:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				PrtTest.m_GunParticlesState = CachedObjectsParams.PARAM1_BOOL.param1;
			break;

			case ERPCs.RPC_CRAFTING_DEBUG:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				PluginRecipesManager plugin_recipes_manager;
				Class.CastTo(plugin_recipes_manager, GetPlugin(PluginRecipesManager));
				plugin_recipes_manager.SetEnableDebugCrafting(CachedObjectsParams.PARAM1_BOOL.param1);
			break;

			case ERPCs.RPC_SOFT_SKILLS_TOGGLE_MODEL:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				player.GetSoftSkillsManager().SetLinearState(CachedObjectsParams.PARAM1_BOOL.param1) ;
			break;

			case ERPCs.RPC_SOFT_SKILLS_SET_SPECIALTY:
				ctx.Read( CachedObjectsParams.PARAM1_FLOAT );
				float specialty = CachedObjectsParams.PARAM1_FLOAT.param1;
				SoftSkillsManager soft_skill_manager = player.GetSoftSkillsManager();
				soft_skill_manager.SetSpecialtyLevel( specialty );
				soft_skill_manager.SynchSpecialtyLevel();
				player.GetStatSpecialty().Set( specialty );
			break;

			case ERPCs.RPC_LIFESPAN_BLOODY_HANDS:
				ctx.Read( CachedObjectsParams.PARAM1_BOOL );
				enable = CachedObjectsParams.PARAM1_BOOL.param1;
				PluginLifespan lifespan_bloody_hands;
				Class.CastTo(lifespan_bloody_hands, GetPlugin( PluginLifespan ));
				lifespan_bloody_hands.UpdateBloodyHandsVisibility( player, enable );
			break;

			case ERPCs.RPC_LIFESPAN_PLAYTIME_UPDATE:
				ctx.Read( CachedObjectsParams.PARAM1_FLOAT );
				//reset players playtime
				float playtime = player.StatGet("playtime");
				float opposite_playtime = playtime * ( -1 );
				player.StatUpdate("playtime", opposite_playtime );
				//set new playtime
				float playtime_update = CachedObjectsParams.PARAM1_FLOAT.param1;
				player.StatUpdate("playtime", playtime_update );
				player.SetLastShavedSeconds( 0 );
				//update lifespan
				PluginLifespan module_lifespan_update;
				Class.CastTo(module_lifespan_update, GetPlugin( PluginLifespan ));
				module_lifespan_update.UpdateLifespan( player, true );
				module_lifespan_update.ChangeFakePlaytime( player, playtime_update );
				
			break;
			case ERPCs.DEV_GO_UNCONSCIOUS:
				ctx.Read( CachedObjectsParams.PARAM1_BOOL );
				if (!CachedObjectsParams.PARAM1_BOOL.param1)
				{
					if (player.IsUnconscious())
					{
						player.SetHealth("","Shock",100);
					}
					else
					{
						player.m_UnconsciousEndTime = -60;
						player.SetHealth("","Shock",0);
					}
				}
				else
				{
					GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(GoUnconsciousDelayed, 10000, false, new Param1<PlayerBase>(player));
				}
			//DayZPlayerSyncJunctures.SendPlayerUnconsciousness(player, !player.IsUnconscious() );
			break;
			case ERPCs.DEV_HAIR_LEVEL_HIDE:
				ctx.Read( CachedObjectsParams.PARAM2_INT_INT ); //PARAM2_INT_INT.param2 is BOOL here
				player.SetHairLevelToHide(CachedObjectsParams.PARAM2_INT_INT.param1,CachedObjectsParams.PARAM2_INT_INT.param2,true);
				player.UpdateHairSelectionVisibility(true);
			break;
			
			case ERPCs.RPC_DO_ACTION_LOGS:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				LogManager.ActionLogEnable(CachedObjectsParams.PARAM1_BOOL.param1);
			break;		
				
			case ERPCs.RPC_DO_WEAPON_LOGS:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				LogManager.WeaponLogEnable(CachedObjectsParams.PARAM1_BOOL.param1);
			break;
			
			case ERPCs.RPC_DO_SYMPTOMS_LOGS:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				LogManager.SymptomLogEnable(CachedObjectsParams.PARAM1_BOOL.param1);
			break;
			
			case ERPCs.RPC_DO_INV_MOVE_LOGS:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				LogManager.InventoryMoveLogEnable(CachedObjectsParams.PARAM1_BOOL.param1);
			break;
			
			case ERPCs.RPC_DO_INV_RESERVATION_LOGS:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				LogManager.InventoryReservationLogEnable(CachedObjectsParams.PARAM1_BOOL.param1);
			break;
			
			case ERPCs.RPC_DO_INV_HFSM_LOGS:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				LogManager.InventoryHFSMLogEnable(CachedObjectsParams.PARAM1_BOOL.param1);
			break;			
			case ERPCs.DEV_RPC_ITEMS_FIX:
				ctx.Read(CachedObjectsParams.PARAM1_BOOL);
				player.FixAllInventoryItems();
			break;
			case ERPCs.DEV_RPC_FORCE_FINISHER:
				ctx.Read(CachedObjectsParams.PARAM1_INT);
				player.GetMeleeFightLogic().m_DbgForcedFinisherType = CachedObjectsParams.PARAM1_INT.param1;
			break;
		}
	}
	// Helper diag functions
	void GoUnconsciousDelayed(Param1<PlayerBase> p1)
	{
		PlayerBase player = p1.param1;
		if (player.IsUnconscious())
		{
			player.SetHealth("","Shock",100);
		}
		else
		{
			player.m_UnconsciousEndTime = -60;
			player.SetHealth("","Shock",0);
		}
	}
	
#endif
}
