class KeybindingsMenu extends UIScriptedMenu
{
	protected TabberUI							m_Tabber;
	protected ref array<ref KeybindingsGroup>	m_Tabs;
	
	protected TextWidget						m_Version;
	
	protected ButtonWidget						m_Apply;
	protected ButtonWidget						m_Back;
	protected ButtonWidget						m_Undo;
	protected ButtonWidget						m_Defaults;
	
	protected int								m_CurrentSettingKeyIndex = -1;
	protected int								m_CurrentSettingAlternateKeyIndex = -1;
	protected ref array<int>					m_SetKeybinds;
	
	override Widget Init()
	{
		layoutRoot			= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/pc/keybinding_menu.layout", null );
		
		m_Version			= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		m_Apply				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "apply" ) );
		m_Back				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "back" ) );
		m_Undo				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "reset" ) );
		m_Defaults			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "defaults" ) );
		
		m_Tabs				= new array<ref KeybindingsGroup>;
		layoutRoot.FindAnyWidget( "Tabber" ).GetScript( m_Tabber );
		
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );

		#ifdef PLATFORM_PS4
			string back = "circle";
			if( GetGame().GetInput().GetEnterButton() == GamepadButton.A )
			{
				back = "circle";
			}
			else
			{
				back = "cross";
			}
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:" + back );
		#endif
		
		Input input	= GetGame().GetInput();
		int group_count = input.GetActionGroupsCount();
		
		for( int i = 0; i < group_count; i++ )
		{
			AddGroup( i, input );
		}
		
		m_Tabber.SelectTabControl( 0 );
		m_Tabber.SelectTabPanel( 0 );
		g_Game.SetKeyboardHandle( this );
		
//		m_Apply.Enable( false );
		ColorDisabled( m_Apply );
		m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
//		m_Undo.Enable( false );
		ColorDisabled( m_Undo );
		m_Undo.SetFlags( WidgetFlags.IGNOREPOINTER );
//		m_Defaults.Enable( true );
		ColorWhite( m_Defaults, null );
		m_Defaults.ClearFlags( WidgetFlags.IGNOREPOINTER );
		
		return layoutRoot;
	}
	
	void AddGroup( int index, Input input )
	{
		string group_name;
		input.GetActionGroupName( index, group_name );
		m_Tabber.AddTab( group_name );
		m_Tabs.Insert( new KeybindingsGroup( index, input, m_Tabber.GetTab( index ), this ) );
	}
	
	KeybindingsGroup GetCurrentTab()
	{
		return m_Tabs.Get( m_Tabber.GetSelectedIndex() );
	}
	
	void ClearKeybind( int key_index )
	{
//		m_Apply.Enable( true );
		ColorWhite( m_Apply, null );
		m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
//		m_Undo.Enable( true );
		ColorWhite( m_Undo, null );
		m_Undo.ClearFlags( WidgetFlags.IGNOREPOINTER );
	}
	
	void ClearAlternativeKeybind( int key_index )
	{
//		m_Apply.Enable( true );
		ColorWhite( m_Apply, null );
		m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
//		m_Undo.Enable( true );
		ColorWhite( m_Undo, null );
		m_Undo.ClearFlags( WidgetFlags.IGNOREPOINTER );
	}
	
	void StartEnteringKeybind( int key_index )
	{
		m_CurrentSettingAlternateKeyIndex	= -1;
		m_CurrentSettingKeyIndex			= key_index;
	}
	
	void CancelEnteringKeybind()
	{
		GetCurrentTab().CancelEnteringKeybind();
		m_CurrentSettingKeyIndex = -1;
	}
	
	void StartEnteringAlternateKeybind( int key_index )
	{
		m_CurrentSettingKeyIndex			= -1;
		m_CurrentSettingAlternateKeyIndex	= key_index;
	}
	
	void CancelEnteringAlternateKeybind()
	{
		GetCurrentTab().CancelEnteringAlternateKeybind();
		m_CurrentSettingAlternateKeyIndex = -1;
	}
	
	void ConfirmKeybindEntry( TIntArray new_keys )
	{
		m_CurrentSettingKeyIndex = -1;
//		m_Apply.Enable( true );
		ColorWhite( m_Apply, null );
		m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
//		m_Undo.Enable( true );
		ColorWhite( m_Undo, null );
		m_Undo.ClearFlags( WidgetFlags.IGNOREPOINTER );
	}
	
	void ConfirmAlternateKeybindEntry( TIntArray new_keys )
	{
		m_CurrentSettingAlternateKeyIndex = -1;
//		m_Apply.Enable( true );
		ColorWhite( m_Apply, null );
		m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
//		m_Undo.Enable( true );
		ColorWhite( m_Undo, null );
		m_Undo.ClearFlags( WidgetFlags.IGNOREPOINTER );
	}
	
	override void Update(float timeslice)
	{
		if( GetGame().GetInput().LocalPress("UAUIBack",false) )
		{
			Back();
		}
		
		if( GetCurrentTab() )
		{
			GetCurrentTab().Update( timeslice );
		}
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_Apply )
			{
				Apply();
				return true;
			}
			else if( w == m_Back )
			{
				Back();
				return true;
			}
			else if( w == m_Undo )
			{
				Reset();
				return true;
			}
			else if( w == m_Defaults )
			{
				SetToDefaults();
				return true;
			}
		}
		return false;
	}
	
	void Apply()
	{
//		m_Apply.Enable( false );
		ColorDisabled( m_Apply );
		m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
//		m_Undo.Enable( false );
		ColorDisabled( m_Undo );
		m_Undo.SetFlags( WidgetFlags.IGNOREPOINTER );
//		m_Defaults.Enable( true );
		ColorWhite( m_Defaults, null );
		m_Defaults.ClearFlags( WidgetFlags.IGNOREPOINTER );
		
		foreach( KeybindingsGroup group : m_Tabs )
		{
			group.Apply();
		}
		
		// save input configuration
		GetUApi().Export();
		//Print("----- Export -----");
	}
	
	void Back()
	{
		if( m_CurrentSettingKeyIndex != -1 )
		{
			CancelEnteringKeybind();
			return;
		}
		
		if( m_CurrentSettingAlternateKeyIndex != -1 )
		{
			CancelEnteringAlternateKeybind();
			return;
		}
		
		bool changed = false;
		foreach( KeybindingsGroup group : m_Tabs )
		{
			changed = changed || group.IsChanged();
		}

		if( changed )
			g_Game.GetUIManager().ShowDialog("#main_menu_configure", "#main_menu_configure_desc", 1337, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
		else
		{
			GetGame().GetUIManager().Back();
		}
	}
	
	//! Undoes the unsaved changes and reverts to previous state. Does not reset to defaults!
	void Reset()
	{
//		m_Apply.Enable( false );
		ColorDisabled( m_Apply );
		m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
//		m_Undo.Enable( false );
		ColorDisabled( m_Undo );
		m_Undo.SetFlags( WidgetFlags.IGNOREPOINTER );
		
		foreach( KeybindingsGroup group : m_Tabs )
		{
			group.ResetEx();
		}
	}
	
	void SetToDefaults()
	{
//		m_Apply.Enable( false );
		ColorDisabled( m_Apply );
		m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
//		m_Undo.Enable( false );
		ColorDisabled( m_Undo );
		m_Undo.SetFlags( WidgetFlags.IGNOREPOINTER );
//		m_Defaults.Enable( false );
		ColorDisabled( m_Defaults );
		m_Defaults.SetFlags( WidgetFlags.IGNOREPOINTER );
		
		GetUApi().Revert();
		
		//DeferredDefaultsInit();
		//GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(DeferredDefaultsInit);
		//GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DeferredDefaultsInit,2000);
		//GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLaterByName(this,"DeferredDefaultsInit",2000);
		
		int mode = -1;
		if ( GetGame().GetMission().IsControlDisabled() )
		{
			mode = GetGame().GetMission().GetControlDisabledMode();
		}
		
		//GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().GetMission().PlayerControlDisable,4000,false,mode);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().PlayerControlDisable,mode);
		
		foreach( KeybindingsGroup group : m_Tabs )
		{
			group.ResetEx(true);
		}
	}
	
	void DeferredDefaultsInit() //TODO: please stop crashing..
	{
		int mode = -1;
		if ( GetGame().GetMission().IsControlDisabled() )
		{
			mode = GetGame().GetMission().GetControlDisabledMode();
		}
		GetGame().GetMission().PlayerControlDisable(mode);
		//GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().PlayerControlDisable,mode);
		//GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().GetMission().PlayerControlDisable,5000,false,mode);
		
		/*foreach( KeybindingsGroup group : m_Tabs )
		{
			group.ResetEx(true);
		}*/
	}
	
	override bool OnModalResult( Widget w, int x, int y, int code, int result )
	{
		if( code == 1337 )
		{
			if( result == 2 )
			{
				Reset();
				GetGame().GetUIManager().Back();
			}
			return true;
		}
		return false;
	}
	
	override void Refresh()
	{
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( w && IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( w && IsFocusable( w ) )
		{
			if ( (w.GetFlags() & WidgetFlags.DISABLED) || (w.GetFlags() & WidgetFlags.IGNOREPOINTER) )
			{
				ColorDisabled(w);
			}
			else
			{
				ColorWhite( w, enterW );
			}
			return true;
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( w && IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( w && IsFocusable( w ) )
		{
			if ( (w.GetFlags() & WidgetFlags.DISABLED) || (w.GetFlags() & WidgetFlags.IGNOREPOINTER) )
			{
				ColorDisabled(w);
			}
			else
			{
				ColorWhite( w, null );
			}
			return true;
		}
		return false;
	}
	
	bool IsFocusable( Widget w )
	{
		if( w )
		{
			return ( w == m_Apply || w == m_Back || w == m_Undo || w == m_Defaults );
		}
		return false;
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
		SetFocus( w );
		
		ButtonWidget button = ButtonWidget.Cast( w );
		if( button && button != m_Apply )
		{
			button.SetTextColor( ARGB( 255, 200, 0, 0 ) );
		}
	}
	
	void ColorWhite( Widget w, Widget enterW )
	{
		#ifdef PLATFORM_WINDOWS
		SetFocus( null );
		#endif
		
		ButtonWidget button = ButtonWidget.Cast( w );
		/*if( button && button != m_Apply )
		{
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
		}*/
		if ( button )
		{
			if ( button.GetFlags() & WidgetFlags.DISABLED )
			{
				button.SetTextColor(ColorManager.COLOR_DISABLED_TEXT);
			}
			else
			{
				button.SetTextColor(ColorManager.COLOR_NORMAL_TEXT);
				//Print("DbgWidgets | ColorWhite: " + w.GetName());
				//DumpStack();
			}
		}
	}
	
	void ColorDisabled( Widget w )
	{
		#ifdef PLATFORM_WINDOWS
		SetFocus( null );
		#endif
		
		ButtonWidget button = ButtonWidget.Cast(w);
		if ( button )
		{
			button.SetTextColor(ColorManager.COLOR_DISABLED_TEXT);
		}
	}
}