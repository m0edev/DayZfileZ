//! base class of all commands exposed to script to provide common functionality over animations
class AnimCommandBase
{
	//! returns entity that this command is bount to
	proto native IEntity GetEntity();

	//! called when command starts
	void OnActivate() {}

	//! called when command ends
	void OnDeactivate() {}

	//! called before any animation is processed
	//! here change animation values, add animation commands
	void PreAnimUpdate(float pDt) {}

	//! functions usable only from OnActivate or PreAnimUpdate
	proto native void PreAnim_CallCommand(int pCommand, int pParamInt, float pParamFloat);
	proto native void PreAnim_SetFloat(int pVar, float pFlt);
	proto native void PreAnim_SetInt(int pVar, int pInt);
	proto native void PreAnim_SetBool(int pVar, bool pBool);
}
