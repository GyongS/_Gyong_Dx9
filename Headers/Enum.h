#pragma once

namespace OBJ_ID
{
	enum ID
	{		
		TERRAIN,
		PORTAL,
		SHOP,
		TUTORIAL,
		CAGE,
		PLAYER_ATTACK,
		PRISONER_SWAP_EFFECT,
		STAMPING_WAVE,
		STAMPING_HIT,
		GHOST_EFFECT,
		SKUL_HEAD,
		DEFAULT_HEAD,
		PRISONER_HEAD,
		SAMURAI_HEAD,
		MONSTER,						
		MONSTER_ATTACK,
		MONSTER_SPAWN,
		PLAYER,
		STONE,		
		ICE_ALLOW,
		GRENADE,
		GRENADE_EFFECT,
		GRENADE_COL,
		BROADSWORD_EFFECT,
		BROADSWORD_COL,
		GIANTWOOD,	
		GIANTWOOD_BALL,
		GIANTWOOD_ATTACK,
		BOSS,
		BOSS_EARTHQUAKE,	
		EFFECT,
		ILSEOM,
		ILSEOM_HIT,
		FREEZE,
		DAMAGEFONT,
		UI,
		SWITCH_ICON,
		BOSS_UI,
		MONSTER_UI,
		GOLD,
		DARKQUARTZ,
		TILE,
		TILE_2,
		PARTICLE,
		MONSTER_ICON,
		MINIMAP,
		MINIMAP_MONSTER,
		MINIMAP_PORTAL,
		EVENT_UI,
		END
	};
}

namespace OBJ_TYPE
{
	enum TYPE
	{
		BROWNWOOD,
		GREENWOOD,
		GIANTWOOD,
		WARRIOR,
		END
	};
}

namespace TEX_ID
{
	enum ID
	{
		MULTI_TEX,
		SINGLE_TEX,
		END
	};
}

namespace MAPOBJ_ID
{
	enum ID
	{
		BUSH,
	    DECORATION,
		MUSHROOM,
		DEFAULTTILE,
		TREE,
		BACKGROUND,
		STARTPOS,
		END
	};
}

namespace TILE_TYPE
{
	enum TYPE
	{
		NONE,
		TILE,
		WALL,
		END
	};
}

namespace SCROLL_ID
{
	enum ID
	{
		SCROLL_X,
		SCROLL_Y
	};
}

namespace CAMERA_ID
{
	enum ID
	{
		CAMERA_X,
		CAMERA_Y
	};
}