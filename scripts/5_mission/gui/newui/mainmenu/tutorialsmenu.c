class TutorialsMenu extends UIScriptedMenu
{
	
	protected string 					m_BackButtonTextID;
	
	protected Widget					m_InfoTextLeft;
	protected Widget					m_InfoTextRight;
	protected ButtonWidget				m_Back;
	
	protected ImageWidget 				m_ControlsLayoutImage;
	protected const int 				TABS_COUNT = 4;
	protected ImageWidget 				m_tab_images[TABS_COUNT];
	protected TabberUI					m_TabScript;
	protected ref TutorialKeybinds		m_KeybindsTab;
	
	//============================================
	// Init
	//============================================
	override Widget Init()
	{
		#ifdef PLATFORM_CONSOLE
		layoutRoot		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/tutorials/xbox/tutorials.layout" );
		#else
		layoutRoot	= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/tutorials/pc/tutorials.layout" );
		#endif
		
		m_InfoTextLeft	= layoutRoot.FindAnyWidget( "InfoTextLeft" );
		m_InfoTextRight	= layoutRoot.FindAnyWidget( "InfoTextRight" );
		
		m_Back			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "back" ) );
		
		layoutRoot.FindAnyWidget("Tabber").GetScript( m_TabScript );
		m_TabScript.m_OnTabSwitch.Insert( DrawConnectingLines );
		
		#ifdef PLATFORM_CONSOLE
		if( GetGame().GetInput().IsEnabledMouseAndKeyboard() )
		{
			m_KeybindsTab	= new TutorialKeybinds( layoutRoot.FindAnyWidget( "Tab_6" ), this );
			m_TabScript.EnableTabControl( 6, true );
		}
		#endif
		
		m_tab_images[0] = ImageWidget.Cast( layoutRoot.FindAnyWidget("MovementTabBackdropImageWidget") );
		m_tab_images[1] = ImageWidget.Cast( layoutRoot.FindAnyWidget("WeaponsAndActionsBackdropImageWidget") );
		m_tab_images[2] = ImageWidget.Cast( layoutRoot.FindAnyWidget("InventoryTabBackdropImageWidget") );
		m_tab_images[3] = ImageWidget.Cast( layoutRoot.FindAnyWidget("MenusTabBackdropImageWidget") );
		
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
			ImageWidget toolbar_b2 = layoutRoot.FindAnyWidget( "BackIcon0" );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:" + back );
			toolbar_b2.LoadImageFile( 0, "set:playstation_buttons image:" + back );
		#endif
		
		PPERequesterBank.GetRequester(PPERequester_TutorialMenu).Start(new Param1<float>(0.6));
		DrawConnectingLines( 0 );
		return layoutRoot;
	}
	
	void TutorialsMenu()
	{
	}
	
	void ~TutorialsMenu()
	{
		m_TabScript.m_OnTabSwitch.Remove( DrawConnectingLines );
		PPERequesterBank.GetRequester(PPERequester_TutorialMenu).Stop();
	}

	void Back()
	{
		GetGame().GetUIManager().Back();
	}
	
	void DrawConnectingLines( int index )
	{
		if( index == 6 )
		{
			m_InfoTextLeft.Show( false );
			m_InfoTextRight.Show( false );
		}
		else
		{
			m_InfoTextLeft.Show( true );
			m_InfoTextRight.Show( true );
			ref array<ref JsonControlMappingInfo> control_mapping_info = new array<ref JsonControlMappingInfo>;
			ref array<ref array <ref JsonControlMappingInfo>> tab_array = new array<ref array <ref JsonControlMappingInfo>>;
			
			map<string, ref array<int>> button_marker_groups_unflitred = new map<string, ref array<int>>;
			map<string, ref array<int>> button_marker_groups = new map<string, ref array<int>>;
			
			float text_widget_pos_x, text_widget_pos_y;
			float text_widget_width, text_widget_height;
			float dot_pos_x, dot_pos_y;
			float dot_width, dot_height;
			float draw_pos_x, draw_pos_y;
			
			CanvasWidget canvas_widget = CanvasWidget.Cast( layoutRoot.FindAnyWidget("CanvasWidget_" + index) );
			canvas_widget.Clear();
			control_mapping_info  = GetControlMappingInfo();
			
			for( int i = 0; i < m_TabScript.GetTabCount(); i++ )
			{
				tab_array.Insert( new array<ref JsonControlMappingInfo> );
				for( int j = 0; j < 30; j++ )
				{
					tab_array[i].Insert( NULL );
				}
			}
			
			// insert json info to array by index, so it is sorted
			for ( i = 0; i < control_mapping_info.Count(); i++ )
			{
				JsonControlMappingInfo info = control_mapping_info.Get( i );
				tab_array[info.m_TabID][info.m_TextWidgetID] = info;
			}
			
			// create group of buttons which are connected together with line
			for( int l = 0; l < control_mapping_info.Count(); l++ )
			{
				JsonControlMappingInfo info1 = control_mapping_info[l];
				string button_name = info1.m_ButtonName;
				int text_widget_id = info1.m_TextWidgetID;
				if( info1.m_TabID != index )
				{
					continue;
				}
				if( !button_marker_groups_unflitred.Contains( button_name ) )
				{
					button_marker_groups_unflitred.Insert( button_name, new ref array<int> );
					button_marker_groups_unflitred.Get( button_name ).Insert( text_widget_id );
				}
				else
				{
					button_marker_groups_unflitred.Get( button_name ).Insert( text_widget_id );
				}
			}
			
			// we want groups which are bigger than 1
			for( l = 0; l < button_marker_groups_unflitred.Count(); l++ )
			{
				if( button_marker_groups_unflitred.GetElement( l ).Count() > 1 )
				{
					string key = button_marker_groups_unflitred.GetKey( l );
					button_marker_groups.Insert( button_marker_groups_unflitred.GetKey( l ), button_marker_groups_unflitred.Get( key ) );
				}
			}
			
			// hide all button markers
			Widget xbox_controls_image = layoutRoot.FindAnyWidget( "Markers_" + index );
			
			Widget panel_widget;
			Widget button_marker_widget;
			
			for( l = 0; l < tab_array[index].Count(); l++ )
			{
				panel_widget = layoutRoot.FindAnyWidget( "PanelWidget" + l );
				if( tab_array[index][l] != NULL )
				{
					TextWidget text_widget = TextWidget.Cast( panel_widget.FindAnyWidget( "TextWidget" + l ) );
					button_marker_widget = layoutRoot.FindAnyWidget( "button_marker_" + tab_array[index][l].m_ButtonName );
					text_widget.SetText( tab_array[index][l].m_InfoText );
					panel_widget.Show( true );
					panel_widget.Update();
					
					if( !button_marker_groups.Contains(tab_array[index][l].m_ButtonName ))
					{
						panel_widget.GetScreenPos( text_widget_pos_x, text_widget_pos_y );
						panel_widget.GetScreenSize( text_widget_width,text_widget_height );
						
						button_marker_widget.GetScreenPos( dot_pos_x, dot_pos_y );
						button_marker_widget.GetScreenSize( dot_width, dot_height );
						
						draw_pos_y = text_widget_pos_y + text_widget_height / 2;
						
						if( l < 15 )
						{
							draw_pos_x = text_widget_pos_x + text_widget_width - 1;
						}
						else
						{
							draw_pos_x = text_widget_pos_x;
						}
						
						canvas_widget.DrawLine( draw_pos_x, draw_pos_y, dot_pos_x+dot_width/2, draw_pos_y, 2, ARGBF( 0.6, 1, 1, 1 ) );
						canvas_widget.DrawLine( dot_pos_x+dot_width/2, draw_pos_y, dot_pos_x+dot_width/2, dot_pos_y+dot_height/2, 2, ARGBF( 0.6, 1, 1, 1 ) );	
					}
				}
				else
				{
					panel_widget.Show( false );
				}
				panel_widget.Update();
			}
			
			// draw connecting lines 
			for( l = 0; l <  button_marker_groups.Count(); l++ )
			{
				text_widget_pos_x = 0;
				text_widget_pos_y = 0;
				text_widget_width = 0;
				text_widget_height = 0;
				float group_point_x = 0, group_point_y = 0;
				float first_x = 0, first_y = 0;
				
				ref array<int> element = button_marker_groups.GetElement( l );
				string key_name = button_marker_groups.GetKey( l );
				button_marker_widget = layoutRoot.FindAnyWidget( "button_marker_" + key_name );
				
				for( int g = 0; g < element.Count(); g++ )
				{
					panel_widget = layoutRoot.FindAnyWidget( "PanelWidget" + element[g] );
					
					panel_widget.GetScreenPos( text_widget_pos_x, text_widget_pos_y );
					panel_widget.GetScreenSize( text_widget_width, text_widget_height );
					
					if( g == 0 )
					{
						if( element[0] < 15 )
						{
							first_x = text_widget_pos_x + text_widget_width +50;
						}
						else
						{
							first_x = text_widget_pos_x - 50;
						}
						first_y = text_widget_pos_y + text_widget_height/2;
						
					}
					
					group_point_x += text_widget_pos_x;
					group_point_y += text_widget_pos_y;
					
					if( element[0] < 15 )
					{
						canvas_widget.DrawLine( text_widget_pos_x + text_widget_width - 1, text_widget_pos_y + text_widget_height/2, text_widget_pos_x + text_widget_width +50, text_widget_pos_y + text_widget_height/2, 2, ARGBF( 0.6, 1, 1, 1 ) );
					}
					else
					{
						canvas_widget.DrawLine( text_widget_pos_x, text_widget_pos_y + text_widget_height/2, text_widget_pos_x - 50, text_widget_pos_y + text_widget_height/2, 2, ARGBF( 0.6, 1, 1, 1 ) );
					}
				}
				
				if( element[0] < 15 )
				{
					group_point_x = group_point_x/element.Count() + text_widget_width + 50;
				}
				else
				{
					group_point_x = group_point_x/element.Count() - 50;
				}
				
				group_point_y = group_point_y/element.Count() + text_widget_height/2;
				
				button_marker_widget.GetScreenPos( dot_pos_x, dot_pos_y );
				button_marker_widget.GetScreenSize( dot_width, dot_height );
				
				canvas_widget.DrawLine( group_point_x, group_point_y, dot_pos_x+dot_width/2, group_point_y, 2, ARGBF( 0.6, 1, 1, 1 ) );
				canvas_widget.DrawLine( dot_pos_x+dot_width/2, group_point_y, dot_pos_x+dot_width/2, dot_pos_y, 2, ARGBF( 0.6, 1, 1, 1 ) );
				
				if( element[0] < 15 )
				{
					canvas_widget.DrawLine( first_x, first_y, text_widget_pos_x + text_widget_width +50, text_widget_pos_y + text_widget_height/2, 2, ARGBF( 0.6, 1, 1, 1 ) );
				}
				else
				{
					canvas_widget.DrawLine( first_x, first_y, text_widget_pos_x - 50, text_widget_pos_y + text_widget_height/2, 2, ARGBF( 0.6, 1, 1, 1 ) );
				}
			}
		}
	}
	
	protected array<ref JsonControlMappingInfo> GetControlMappingInfo()
	{
		array<ref JsonControlMappingInfo> control_mapping_info = new array<ref JsonControlMappingInfo>;
		string file_path;
		
		#ifdef PLATFORM_WINDOWS
			file_path =	"Scripts/data/PageDataTutorials.json";
		#endif
		#ifdef PLATFORM_XBOX
			if( GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer() )
				file_path =	"Scripts/data/PageDataTutorials.json";
			else
				file_path =	"Xbox/PageDataTutorials.json";
		#endif
		#ifdef PLATFORM_PS4
			if( GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer() )
				file_path =	"Scripts/data/PageDataTutorials.json";
			else
				file_path =	"Ps4/PageDataTutorials.json";
		#endif
		FileHandle file_handle = OpenFile(file_path, FileMode.READ);
		JsonSerializer js = new JsonSerializer();
		
		string js_error = "";
		string line_content = "";
		string content = "";
		if ( file_handle )
		{
			while ( FGets( file_handle,  line_content ) >= 0 )
			{
				content += line_content;
			}
			CloseFile( file_handle );
			
			if ( js.ReadFromString( control_mapping_info, content, js_error ) )
			{
				return control_mapping_info;
			}
			else
			{
				Print("JSON ERROR => [ControlMappingInfo.json]: "+ js_error);
				DumpStack();
			}
		}
		
		return control_mapping_info;
	}
	
	override void OnShow()
	{
		super.OnShow();
		#ifdef PLATFORM_CONSOLE
			layoutRoot.FindAnyWidget( "play_panel_root" ).Show( GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer() );
			layoutRoot.FindAnyWidget( "toolbar_bg" ).Show( !GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer() );
		#endif
	}
	
	override void Update( float timeslice )
	{
		if( GetGame().GetInput().LocalPress("UAUITabLeft",false) )
		{
			m_TabScript.PreviousTab();
		}
		
		//RIGHT BUMPER - TAB RIGHT
		if( GetGame().GetInput().LocalPress("UAUITabRight",false) )
		{
			m_TabScript.NextTab();
		}
		
		if( GetGame().GetInput().LocalPress("UAUIBack",false) )
		{
			Back();
		}
	}
	
		/// Initial texts load for the footer buttons
	protected void LoadFooterButtonTexts()
	{
		TextWidget uiBackText 	= TextWidget.Cast(layoutRoot.FindAnyWidget( "BackText" ));		
		
		if (uiBackText)
		{
			uiBackText.SetText(m_BackButtonTextID);
		}
	}
	/// Set correct bottom button texts based on platform (ps4 vs xbox texts)
	protected void LoadTextStrings()
	{
		#ifdef PLATFORM_PS4
			m_BackButtonTextID = "ps4_ingame_menu_back";
		#else 
			m_BackButtonTextID = "STR_rootFrame_toolbar_bg_ConsoleToolbar_Back_BackText0";	
		#endif
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_Back )
			{
				Back();
				return true;
			}
		}
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorHighlight( w );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorNormal( w );
			return true;
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorHighlight( w );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorNormal( w );
			return true;
		}
		return false;
	}
	
	bool IsFocusable( Widget w )
	{
		return ( w && w == m_Back );
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorHighlight( Widget w )
	{
		if( !w )
			return;
		
		//SetFocus( w );
		
		int color_pnl = ARGB(255, 0, 0, 0);
		int color_lbl = ARGB(255, 255, 0, 0);
		int color_img = ARGB(255, 200, 0, 0);
		
		#ifdef PLATFORM_CONSOLE
			color_pnl = ARGB(255, 200, 0, 0);
			color_lbl = ARGB(255, 255, 255, 255);
		#endif
		
		ButtonSetColor(w, color_pnl);
		ButtonSetTextColor(w, color_lbl);
		ImagenSetColor(w, color_img);
	}
	
	void ColorNormal( Widget w )
	{
		if( !w )
			return;
		
		int color_pnl = ARGB(0, 0, 0, 0);
		int color_lbl = ARGB(255, 255, 255, 255);
		int color_img = ARGB(255, 255, 255, 255);
		
		ButtonSetColor(w, color_pnl);
		ButtonSetTextColor(w, color_lbl);
		ImagenSetColor(w, color_img);
	}
	
	void ButtonSetText( Widget w, string text )
	{
		if( !w )
			return;
				
		TextWidget label = TextWidget.Cast(w.FindWidget( w.GetName() + "_label" ) );
		
		if( label )
		{
			label.SetText( text );
		}
		
	}
	
	void ButtonSetColor( Widget w, int color )
	{
		if( !w )
			return;
		
		Widget panel = w.FindWidget( w.GetName() + "_panel" );
		
		if( panel )
		{
			panel.SetColor( color );
		}
	}
	
	void ImagenSetColor( Widget w, int color )
	{
		if( !w )
			return;
		
		Widget panel = w.FindWidget( w.GetName() + "_image" );
		
		if( panel )
		{
			panel.SetColor( color );
		}
	}
	
	void ButtonSetTextColor( Widget w, int color )
	{
		if( !w )
			return;

		TextWidget label	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_label" ) );
		TextWidget text		= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_text_1" ) );
				
		if( label )
		{
			label.SetColor( color );
		}
		
		if( text )
		{
			text.SetColor( color );
		}
		
		if( text2 )
		{
			text2.SetColor( color );
		}
	}
}
