#ifdef DEVELOPER
//----------------------------------------------------------
//Class names need to match the preset name + _Preset suffix
//----------------------------------------------------------


//TEMPLATE:
/*

class Farmer_Preset extends PresetSpawnBase
{
	override void OnPresetSpawn(PlayerBase player)
	{
		//m_Items -> contains all items in player inventory
	}
}

*/

//---------------------------------------------

class FreshSpawn_Preset extends PresetSpawnBase
{
	override void OnPresetSpawn(PlayerBase player)
	{
		FindAndTakeToHands("Plum");
	}
}
//---------------------------------------------

class Farmer_Preset extends PresetSpawnBase
{
	override void OnPresetSpawn(PlayerBase player)
	{

	}
}
//---------------------------------------------






//---------------------------------------------
//------------------ BASE ---------------------
//---------------------------------------------

class PresetSpawnBase
{
	ref array<EntityAI> m_Items = new array<EntityAI>;
	HumanInventory m_Inventory;
	PlayerBase m_Player;

	//---------------------------------------------
	
	void Init(PlayerBase player)
	{
		m_Player = player;
		m_Inventory = player.GetHumanInventory();
		m_Inventory.EnumerateInventory(InventoryTraversalType.INORDER, m_Items);

		OnPresetSpawn(player);
	}
	
	//---------------------------------------------
	
	void OnPresetSpawn(PlayerBase player)
	{

	}
	//---------------------------------------------
	
	void TakeToHands(EntityAI item)
	{
		m_Player.TakeEntityToHandsImpl(InventoryMode.SERVER, item);
	}
	
	void FindAndTakeToHands(string classname)
	{
		foreach(EntityAI item:m_Items)
		{
			if (item.ClassName() == classname)
			{
				TakeToHands(item);
			}
		}
	}
	
}

#endif