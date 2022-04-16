class Hammer extends Inventory_Base
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionRepairPart);
		AddAction(ActionBuildPartSwitch);
		AddAction(ActionMineRock1H);
		AddAction(ActionBuildPart);
	}
}