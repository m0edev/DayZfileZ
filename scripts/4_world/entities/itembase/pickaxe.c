class Pickaxe extends ItemBase
{
	override bool CanMakeGardenplot()
	{
		return true;
	}
	
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionBuildPartSwitch);
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDigGardenPlot);
		AddAction(ActionDismantleGardenPlot);
		AddAction(ActionUnrestrainTarget);
		AddAction(ActionMineRock);
		AddAction(ActionDismantlePart);
		AddAction(ActionBuildPart);
		AddAction(ActionBuryBody);
		AddAction(ActionBuryAshes);
		AddAction(ActionDigWorms);
		AddAction(ActionSkinning);
		AddAction(ActionDigOutStash);
		AddAction(ActionDigInStash);
		AddAction(ActionCreateGreenhouseGardenPlot);
	}
}	