class Tripod extends ItemBase
{
	override bool HasProxyParts()
	{
		return true;
	}
	
	override bool CanDetachAttachment( EntityAI parent )
	{
		FireplaceBase fireplace = FireplaceBase.Cast(parent);
		if(fireplace)
		{
			if ( fireplace.GetCookingEquipment() != null )
				return false;
		}
		return true;
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionAttach);
		AddAction(ActionDetach);
	}
}