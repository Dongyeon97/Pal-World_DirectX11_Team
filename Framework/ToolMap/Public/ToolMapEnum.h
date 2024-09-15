#pragma once

namespace ToolMap
{
	enum class EMapObjectCollider
	{
		AABB,
		OBB,
		SPHERE,
		FRUSTUM,
		ENUM_END
	};

	enum class EEventTrigger
	{
		EVENT1,
		EVENT2,
		ENUM_END
	};

	enum class ECameraType
	{
		CAMERAPLAYER,
		CAMERADUNGEON1,
		CAMERADUNGEON2,
		ENUM_END
	};

	enum class EMonster
	{
		ENUM_END
	};

	enum class ENPCType
	{
		//ªÛ¿Œ
		MERCHANT,
		//π–∑∆≤€
		POACHER,
		ENUM_END
	};

	enum class EToolMapObjectType
	{
		WOOD,
		STONE,
		TOWER,
		ENUM_END
	};

	enum class EMinableType
	{
		TREE,
		ROCK,
		ENUM_END
	};

	enum class EDayType
	{
		DAY,
		NIGHT,
		ENUM_END
	};

	/********************
		Habitat Enum
	********************/
	enum class EHabitatType
	{
		NATURE,
		PAL,
		ARCHITECTURE,
		SPOTLIGHT,
		POINTLIGHT,
		DIRECTIONALLIGHT,
		CAMERA,
		ENUM_END
	};

	enum class ENatureType
	{
		TREE,
		ROCK,
		BRANCH,
		STONE,
		ENUM_END
	};
}
