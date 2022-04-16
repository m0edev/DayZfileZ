class PluginItemDiagnosticEventHandler extends ScriptedWidgetEventHandler
{
	PluginItemDiagnostic m_Owner;

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if( ButtonWidget.Cast(w))
		{
			GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		}
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if( ButtonWidget.Cast(w))
		{
			GetGame().GetMission().PlayerControlEnable(true);
		}
		return true;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		return m_Owner.OnClick( w, x, y, button );
	}

}



class PluginItemDiagnostic extends PluginDeveloper
{
	ref Timer 						myTimer1;
	ref map<PlayerBase,ItemBase> 	m_ObserversMap = new map<PlayerBase,ItemBase>;
	ref array<string> 				m_Agents = new array<string>;
	ref map<string,float> 			m_Floats = new map<string,float>;
	ref map<string,float> 			m_VarsNumbersFinalServer = new map<string,float>;
	ref map<string,float> 			m_VarsFinalClient = new map<string,float>;
	ref array<ref Param>		m_Properties = new array<ref Param>;
	bool 									m_IsActive	= false;
	string									m_NoteClient;
	ref PluginItemDiagnosticEventHandler 	m_EventHandler;
	
	Widget 				m_DebugRootWidget;
	Widget 				m_FrameWidget;
	
	TextListboxWidget 	m_DebugAgentListWidget;
	TextWidget 			m_DebugOutputServer;
	TextWidget 			m_DebugOutputClient;
	TextListboxWidget 	m_DebugFloatsProperListWidget;
	TextListboxWidget 	m_DebugClientVarsWidget;
	TextWidget 			m_ClassNameWidget;
	ItemPreviewWidget 	m_ItemPreviewWidget;
	
	ButtonWidget		m_DebugButtonWidget1;
	ButtonWidget		m_DebugButtonWidget2;
	ButtonWidget		m_DebugButtonWidget3;
	ButtonWidget		m_DebugButtonWidget4;

	void PluginItemDiagnostic()
	{
		#ifndef NO_GUI
		InitializeWidgets();
		ShowWidgets(false);
		#endif
	}
	
	
	void ~PluginItemDiagnostic()
	{
		ClearProperties();
	}

	
	bool OnClick( Widget w, int x, int y, int button )
	{
		int index;
		
		if (w == m_DebugButtonWidget1)
		{
			index = 1;
		}
		else if (w == m_DebugButtonWidget2)
		{
			index = 2;
		}		
		else if (w == m_DebugButtonWidget3)
		{
			index = 3;
		}
		else if (w == m_DebugButtonWidget4)
		{
			index = 4;
		}
		
		#ifdef DEVELOPER
		if(_item)
			_item.OnDebugButtonPressClient(index);
		#endif
		
		GetGame().RPCSingleParam(null, ERPCs.DEV_RPC_ITEM_DIAG_BUTTON, new Param1<int>(index),true, GetGame().GetPlayer().GetIdentity());
		return true;
	}
	
	void RegisterDebugItem(ItemBase item, PlayerBase player)
	{
		if(!myTimer1)
		{
			myTimer1 = new Timer();
			myTimer1.Run(1, this, "Tick", NULL, true);
		}
		
		if(m_ObserversMap.Contains(player))
		{
			m_ObserversMap.Set(player,item);
		}
		else
		{
			m_ObserversMap.Insert(player,item);
		}
		#ifdef DEVELOPER
		_item = item;
		#endif
	}
	
	EntityAI GetWatchedItem(PlayerBase player)
	{
		return m_ObserversMap.Get(player);
	}
	
	
	bool IsActive()
	{
		return m_IsActive;
	}

	void OnScriptMenuOpened(bool opened/*1 - opened , 0 - closed*/)
	{
		if(m_DebugRootWidget && IsActive()) 
			m_DebugRootWidget.Show(!opened);
	}
	
	
	void ShowWidgets(bool show)
	{
		m_IsActive = show;
		if(m_DebugRootWidget) 
			m_DebugRootWidget.Show(show);
	}

	void ToggleDebugWindowEvent()
	{
		if(m_IsActive)
		{		
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			GetGame().RPCSingleParam(player, ERPCs.RPC_ITEM_DIAG_CLOSE,null, true);
			ShowWidgets(false);
			ClearWidgets();
			//m_IsActive = false;
		}
		else
		{
			ShowWidgets(true);
			//m_IsActive = true;
		}

	}

	void ClearWidgets()
	{
		m_DebugAgentListWidget.ClearItems();
		m_DebugOutputServer.SetText("");
		m_DebugOutputClient.SetText("");
		m_DebugFloatsProperListWidget.ClearItems();
		m_ItemPreviewWidget.SetItem(NULL);
		m_ClassNameWidget.SetText("");		
	}
	
	void Tick()
	{
		for(int i = 0; i < m_ObserversMap.Count();i++)
		{
			ItemBase item = m_ObserversMap.GetElement(i);
			PlayerBase player = m_ObserversMap.GetKey(i);

			if (item && player) 
			{
				GeneratePropertiesObject(item);
				SendRPC(item, player.GetIdentity());
			}
			else
			{
				Debug.LogError("PluginItemDiagnostic: dead debug record, removing record");
				m_ObserversMap.RemoveElement(i);
			}
		}
		
	}
	
	void ClearProperties()
	{
		m_Properties.Clear();
	}


	void GeneratePropertiesObject(ItemBase item)
	{
		ClearProperties();
		GetLocalProperties(item, m_Properties);

	}
	
	void SendRPC(ItemBase item, PlayerIdentity identity)
	{
		GetGame().RPC(item,ERPCs.RPC_ITEM_DIAG,m_Properties,true,identity);
	}
	
	void StopWatchRequest(PlayerBase player)//called from player after an RPC call
	{
		if( m_ObserversMap.Contains(player) )
		{
			m_ObserversMap.Remove(player);
		}
	}

	void OnRPC(ItemBase item, ParamsReadContext ctx)
	{
		#ifdef DEVELOPER
		_item = item;
		#endif
		if( !IsActive() )
		{
			ShowWidgets(true);
		}
		
		ctx.Read(CachedObjectsParams.PARAM1_STRING);
		string debug_output_server = CachedObjectsParams.PARAM1_STRING.param1;
		ctx.Read(CachedObjectsParams.PARAM1_INT);//hardcoded to index 1
		int agents_count 	= CachedObjectsParams.PARAM1_INT.param1;

		ctx.Read(CachedObjectsParams.PARAM1_INT);//hardcoded to index 2
		int individual_items_count 	= CachedObjectsParams.PARAM1_INT.param1;
		
		FillAgentArray(ctx, agents_count);
		
		array<ref Param> vars_server = new array<ref Param>;
		FillServerFinalVars(individual_items_count, ctx, vars_server);
		
		
		array<ref Param> vars_client = new array<ref Param>;
		GetLocalProperties(item, vars_client, true);
		
		DisplayAll(item, vars_server, vars_client, debug_output_server );
		

		m_DebugRootWidget.Show(!GetDayZGame().IsInventoryOpen());

			
	}
	
	void FillServerFinalVars(int count, ParamsReadContext ctx, array<ref Param> params )
	{
		for(int i = 0; i < count; i++)
		{
			Param2<string,float> p2 = new Param2<string,float>("",0);
			
			ctx.Read(p2);
			/*
			string name = CachedObjectsParams.PARAM2_STRING_FLOAT.param1;
			float value = CachedObjectsParams.PARAM2_STRING_FLOAT.param2;
			PrintString(name+","+value.ToString());
			*/
			params.Insert(p2);
		}
		//PrintString("----------------------------------------------------");
			
	}
	
	void InitializeWidgets()
	{
		m_EventHandler = new PluginItemDiagnosticEventHandler;
		m_EventHandler.m_Owner = this;
		
		if(!m_DebugRootWidget) 
			m_DebugRootWidget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/debug/debug_item.layout");

		m_DebugRootWidget.SetHandler(m_EventHandler);

		m_FrameWidget = m_DebugRootWidget.FindAnyWidget("FrameWidget0");
		m_DebugAgentListWidget = TextListboxWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_Agents"));
		m_DebugOutputServer = TextWidget.Cast(m_DebugRootWidget.FindAnyWidget("DebugOutputServer"));
		m_DebugOutputClient = TextWidget.Cast(m_DebugRootWidget.FindAnyWidget("DebugOutputClient"));
		m_DebugFloatsProperListWidget = TextListboxWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_FloatsProper"));
		m_ItemPreviewWidget = ItemPreviewWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_ItemPreview"));
		m_ClassNameWidget = TextWidget.Cast(m_DebugRootWidget.FindAnyWidget("w_ClassName"));
		
		m_DebugButtonWidget1 = ButtonWidget.Cast(m_DebugRootWidget.FindAnyWidget("DebugButton1"));
		m_DebugButtonWidget2 = ButtonWidget.Cast(m_DebugRootWidget.FindAnyWidget("DebugButton2"));
		m_DebugButtonWidget3 = ButtonWidget.Cast(m_DebugRootWidget.FindAnyWidget("DebugButton3"));
		m_DebugButtonWidget4 = ButtonWidget.Cast(m_DebugRootWidget.FindAnyWidget("DebugButton4"));
	}

	void DisplayAll(ItemBase item, array<ref Param> vars_server, array<ref Param> vars_client, string debug_text_server)
	{
		m_ClassNameWidget.SetText( item.GetType() );
		m_ItemPreviewWidget.SetItem(item);
		UpdateAgentWidget();
		//UpdateFloatWidget();
		UpdateNumericalVarsWidget(vars_server, vars_client);
		m_DebugOutputServer.SetText(debug_text_server);
		m_DebugOutputClient.SetText(item.GetDebugText());
		
		string button1, button2, button3, button4;
		item.GetDebugButtonNames(button1, button2, button3, button4);
		m_DebugButtonWidget1.SetText(button1);
		m_DebugButtonWidget2.SetText(button2);
		m_DebugButtonWidget3.SetText(button3);
		m_DebugButtonWidget4.SetText(button4);
	}	

	void FillAgentArray(ParamsReadContext ctx, int agents_count)
	{
		m_Agents.Clear();
		for(int i = 0; i < agents_count; i++)
		{
			ctx.Read(CachedObjectsParams.PARAM1_STRING);
			string agent = CachedObjectsParams.PARAM1_STRING.param1;
			m_Agents.Insert(agent);
		}
	}


	void UpdateAgentWidget()
	{
		m_DebugAgentListWidget.ClearItems();
		for(int i = 0; i < m_Agents.Count(); i++)
		{
			string agent = m_Agents.Get(i);
			m_DebugAgentListWidget.AddItem(agent,NULL,0);
		}
	}	

	
	void UpdateNumericalVarsWidget(array<ref Param> vars_server, array<ref Param> vars_client)
	{
		m_DebugFloatsProperListWidget.ClearItems();
		for(int i = 0; i < vars_server.Count(); i++)
		{
			Param param_server = vars_server.Get(i);
			Param2<string,float> p2_server = Param2<string,float>.Cast(param_server);
			
			Param param_client = vars_client.Get(i);
			Param2<string,float> p2_client = Param2<string,float>.Cast(param_client);
			
			string name = p2_server.param1;
			m_DebugFloatsProperListWidget.AddItem(name,NULL,0);
			m_DebugFloatsProperListWidget.SetItem(i,p2_server.param2.ToString(),NULL,1);
			m_DebugFloatsProperListWidget.SetItem(i,p2_client.param2.ToString(),NULL,2);
		}
	}	
	
	
	void PrintOut()
	{
		Debug.LogArrayString(m_Agents,"ItemDebug");
		Debug.Log("----------------------","ItemDebug");
	}
	
	//-------------------------
	// QUERY FOR DEBUG PROPERTIES
	//-------------------------
	void GetLocalProperties(ItemBase item, array<ref Param> properties, bool client = false)
	{
		if(!client)
		{
			// -------- DEBUG OUTPUT ------
			Param1<string> debug_output = new Param1<string>(item.GetDebugText());
			properties.Insert(debug_output);
			// -------- AGENTS ------------
			int num_of_agents = FillWithAgents(item, properties);
			Param1<int> agents_count = new Param1<int>(num_of_agents);
			properties.InsertAt(agents_count,1);// hard coded index 1 !!
		}
	
		// ------ INDIVIDUAL VARS------
		int number_of_items = 0;
		Param2<string, float> quantity = new Param2<string, float>("quantity", item.GetQuantity() );
		properties.Insert(quantity);
		number_of_items++;
		//-----------------------------
		Param2<string, float> liquid_type = new Param2<string, float>("liquid_type", item.GetLiquidType() );
		properties.Insert(liquid_type);
		number_of_items++;
		//-----------------------------
		Param2<string, float> wetness = new Param2<string, float>("wetness", item.GetWet() );
		properties.Insert(wetness);
		number_of_items++;
		//-----------------------------
		Param2<string, float> temperature = new Param2<string, float>("temperature", item.GetTemperature() );
		properties.Insert(temperature);
		number_of_items++;
		//-----------------------------
		Param2<string, float> energy = new Param2<string, float>("energy", item.GetEnergy() );
		properties.Insert(energy);
		number_of_items++;
		//-----------------------------
		Param2<string, float> health;
		if(!client)
		{
			health = new Param2<string, float>("health", item.GetHealth("", "") );
		}
		else
		{
			health = new Param2<string, float>("NO SYNC",0 );
		}
		properties.Insert(health);
		number_of_items++;
		//-----------------------------
		int r,g,b,a;
		item.GetColor(r,g,b,a);
		Param2<string, float> p2r = new Param2<string, float>("R",r);
		properties.Insert(p2r);
		number_of_items++;
		Param2<string, float> p2g = new Param2<string, float>("G",g);
		properties.Insert(p2g);
		number_of_items++;
		Param2<string, float> p2b = new Param2<string, float>("B",b);
		properties.Insert(p2b);
		number_of_items++;
		Param2<string, float> p2a = new Param2<string, float>("A",a);
		properties.Insert(p2a);
		number_of_items++;
		//-----------------------------
		if(!client)
		{
			Param1<int> item_count = new Param1<int>(number_of_items);
			properties.InsertAt(item_count,2);
		}
	}

	int FillWithAgents(ItemBase item, array<ref Param> properties)
	{
		int agents = item.GetAgents();
		if(agents == 0) return 0;
		int num_of_agents = 0;
		ref array<int> agents_aray = new array<int>;
		PluginTransmissionAgents.BuildAgentArray(agents, agents_aray);
		int agents_count = agents_aray.Count();
	
		for(int i = 0; i < agents_count; i++)
		{
			//PluginTransmissionAgents mta = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
			string agent_name = PluginTransmissionAgents.GetNameByID(agents_aray.Get(i));
			Param1<string> param1 = new Param1<string>(agent_name);
			properties.Insert(param1);
			num_of_agents++;
		}

		return num_of_agents;
	}
}
	

