enum EActions
{
	GET_TOTAL_WEIGHT,
	SET_MAX_QUANTITY,
	ADD_QUANTITY,
	REMOVE_QUANTITY,
	SET_QUANTITY_0,
	INJECT_STRING_RABBIT,
	INJECT_STRING_TIGER,
	ADD_HEALTH,
	REMOVE_HEALTH,
	WATCH_ITEM,
	USER_ACTION,
	ADD_TEMPERATURE,
	REMOVE_TEMPERATURE,
	ADD_WETNESS,
	REMOVE_WETNESS,
	LIQUIDTYPE_UP,
	LIQUIDTYPE_DOWN,
	PRINT_BULLETS,
	SPIN,
	FOOD_STAGE_PREV,
	FOOD_STAGE_NEXT,
	//<----add new individual types above this line
	
	
	DEBUG_AGENTS_RANGE_INJECT_START = 100,
	DEBUG_AGENTS_RANGE_INJECT_END = 199,
	
	DEBUG_AGENTS_RANGE_REMOVE_START = 200,
	DEBUG_AGENTS_RANGE_REMOVE_END = 299,
	
	RECIPES_RANGE_START = 1000,
	RECIPES_RANGE_END = 3000,
}