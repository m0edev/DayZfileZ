class Screwdriver extends ToolBase
{
	void Screwdriver()
	{
		m_MineDisarmRate = 80;
	}
	
	override bool IsMeleeFinisher()
	{
		return true;
	}
	
	override array<int> GetValidFinishers()
	{
		return {EMeleeHitType.FINISHER_LIVERSTAB};
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionBurnSewTarget);
		AddAction(ActionUnrestrainTarget);
		AddAction(ActionBurnSewSelf);
		AddAction(ActionMineTreeBark);
		AddAction(ActionSkinning);
		AddAction(ActionLockAttachment);
		AddAction(ActionDisarmMine);
	}
}