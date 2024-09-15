#pragma once

namespace FbxBinary
{
	/********************
		Engine Enum
	********************/
	enum class ETransformType
	{
		RIGHT,
		UP,
		FORWARD,
		POSITION,
		ENUM_END,
	};

	enum class ERenderType
	{
		NONE,
		RENDER_PRE,
		RENDER_NBLEND,
		RENDER_LBLEND,
		RENDER_PBR,
		RENDER_BLEND,
		RENDER_UI,
		RENDER_POST,
		RENDER_DEBUG,
		ENUM_END
	};

	enum class EComponentType
	{
		TRANSFORM,
		CAMERA,
		MESH,
		SPHERE,
		BOX,
		CAPSULE,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
		RECT,
		PARTICLE,
		EFFECTMESH,
		HEIGHT_FIELD,
		BEHAVIOR_TREE,
		FSM,
		TRIANGLE_MESH,
		ENUM_END
	};

	enum class ERenderTargetType
	{
		NORMAL,
		DIFFUSE_LINEAR,
		DIFFUSE_GAMMA,
		NORMAL_TEXTURE,
		MRSO_COMBINDED,
		MRSO_METALIC,
		MRSO_ROUGHNESS,
		MRSO_SPECULAR,
		MRSO_OCCLUSION,
		EMISSION,
		SSS,
		OBJECT_WORLDPOSITION,
		OBJECT_ID,
		ENUM_END
	};

	enum class ESamplerStateType
	{

	};

	enum class EProjectionType
	{
		ORTHOGONAL,
		PERSPECTIVE,
		ENUM_END
	};

	enum class ECbufferType
	{
		PERFRAME,
		PERCAMERA,
	};

	enum class ELightType
	{
		DIRECTIONAL,
		POINT,
		ENUM_END
	};

	enum class ECameraType
	{
		DEFAULT,	// 기본 카메라
		SPHERICAL,	// 구 카메라
		CINEMATIC,	// 연출 카메라
		ENUM_END
	};

	/********************
		UI Enum
	********************/
	enum class EUIType
	{
		STATIC_IMAGE,
		DYNAMIC_IMAGE,
		SLIDER,
		BUTTON,
		TEXT,
		LINEAR_HORIZONTAL_PROGRESSBAR,
		LINEAR_VERTICAL_PROGRESSBAR,
		CIRCULAR_PROGRESSBAR,
		ENUM_END
	};

	enum class EImageType
	{
		DEFAULT,
		INVENTORY,
		ITEM,
		ENUM_END
	};

	enum class EButtonState
	{
		HOVER,
		UP,
		ENUM_END
	};

	/********************
		Effect Enum
	********************/
	enum class EEffectMode
	{
		LERP,
		KEYFRAME,
		ENUM_END
	};

	enum class EEffectType
	{
		PARTICLE,
		EFFECTMESH,
		INSTANCEMESH,
		ENUM_END
	};

	enum class EParticleType
	{
		DROP,
		SPREAD,
		ERUPTION,
		ENUM_END
	};

	enum class ETextureType
	{
		UI,
		DIFFUSE,
		NOISE,
		MASK,
		DISTORTION,
		DISSOLVE,
		MODEL,
		ENUM_END
	};

	/* 24-04-09 이동준 추가 */
	enum class EMapMeshType
	{
		GRID,
		ENVMESH,
		ENVCOLLISION,
		TREE,
		ROCK,
		WOOD,
		STONE,
		PAL,
		ARCHITECTURE,
		LIGHT,
		CAMERA,
		ENUM_END
	};

	enum class EEasingType
	{
		EASE_NONE,
		EASE_LINEAR,
		EASE_IN_SINE,
		EASE_OUT_SINE,
		EASE_INOUT_SINE,
		EASE_IN_SQUARE,
		EASE_OUT_SQUARE,
		EASE_INOUT_SQUARE,
		EASE_IN_CIRC,
		EASE_OUT_CIRC,
		EASE_INOUT_CIRC,
		EASE_IN_BACK,
		EASE_OUT_BACK,
		EASE_INOUT_BACK,
		EASE_IN_ELASTIC,
		EASE_OUT_ELASTIC,
		EASE_INOUT_ELASTIC,
		EASE_IN_BOUNCE,
		EASE_OUT_BOUNCE,
		EASE_INOUT_BOUNCE,
		ENUM_END
	};

	/********************
		Sound Enum
	********************/
	enum class ESoundAttenuationType
	{
		LINEAR,
		ENUM_END
	};

	enum class ESoundType
	{
		BACKGROUND,
		EFFECT,
		TALK,
		USER_INTERFACE,
		ENUM_END
	};

	/********************
		Event Enum
	********************/
	enum class EEventType
	{
		OPEN_LEVEL,
		ENUM_END
	};

	enum class EEventInvokeType
	{
		FORWARD,
		DEFERRED,
		ENUM_END
	};

	enum class EControllerShapeType
	{
		CAPSULE,
		BOX,
		ENUM_END
	};

	enum class EMobilityType
	{
		STATIC,
		DYNAMIC,
		ENUM_END
	};

	enum class EModelType
	{
		ANIM,
		NONANIM,
		MAP_LANDSCAPE,
		EXTERN,
		MAP_OBJECT,
		MAP_OBJECT_COLLISION,
		MAP_DEBUG,
		ENUM_END
	};

	/********************
		BehaviorTree Enum
	********************/
	enum class EBTNodeType
	{
		COMPOSITE,
		TASK,
		ENUM_END
	};

	enum class EBTResult
	{
		BT_FAIL,
		BT_RUNNING,
		BT_SUCCESS,
		ENUM_END
	};

	enum class ECompositeType
	{
		SELECTOR,
		SEQUENCE,
		ENUM_END
	};

	/********************
		Shader Enum
	********************/
	enum class EShaderType
	{
		COMPUTE,
		DOMAIN_,// 이미 DOMAIN 매크로가 있습니다.
		PIXEL,
		GEOMETRY,
		HULL,
		VERTEX,
		ENUM_END
	};

	enum class EBufferType
	{
		VERTEX,
		PIXEL,
		CONSTANT,
		ENUM_END
	};

	enum class EConstantBufferType
	{
		ONE_INT,
		TWO_INT,
		THREE_INT,
		FOUR_INT,

		ONE_FLOAT,
		TWO_FLOAT,
		THREE_FLOAT,
		FOUR_FLOAT,

		ONE_MATRIX,
		TWO_MATRIX,
		THREE_MATRIX,
		FOUR_MATRIX,

		ONE_VECTOR3,
		TWO_VECTOR3,
		THREE_VECTOR3,
		FOUR_VECTOR3,

		ONE_VECTOR4,
		TWO_VECTOR4,
		THREE_VECTOR4,
		FOUR_VECTOR4,

		ENUM_END
	};

	/********************
		PhysX Enum
	********************/
	enum class ERigidbodyType
	{
		STATIC,
		DYNAMIC,
		ENUM_END,
	};

	enum class EAxisLockType
	{
		LOCK_LINEAR_X = (1 << 0),
		LOCK_LINEAR_Y = (1 << 1),
		LOCK_LINEAR_Z = (1 << 2),
		LOCK_ANGULAR_X = (1 << 3),
		LOCK_ANGULAR_Y = (1 << 4),
		LOCK_ANGULAR_Z = (1 << 5),
		ENUM_END
	};

	/*
	enum class EShapeType
	{
		SPHERE,
		BOX,
		CAPSULE,
		ENUM_END
	};
	*/

	enum class EColliderType
	{
		SPHERE,
		BOX,
		CAPSULE,
		HEIGHT_FIELD,
		TRIANGLE_MESH,
		ENUM_END
	};

	enum class ECollisionLayer
	{
		PLAYER = 1 << 1,
		BOSS = 1 << 2,
		MONSTER = 1 << 3,
		PLAYER_WEAPON = 1 << 4,
		MONSTER_WEAPON = 1 << 5,
		PLAYER_PROJECTILE = 1 << 6,
		MONSTER_PROJECTILE = 1 << 7,
		ITEM = 1 << 8,
		ENVIRONMENT = 1 << 9,
		BUILDING = 1 << 10,
		ENUM_END
	};

	enum class ECollisionFlag
	{
		PLAYER
		= (int)ECollisionLayer::ENVIRONMENT
		| (int)ECollisionLayer::BUILDING
		| (int)ECollisionLayer::BOSS
		| (int)ECollisionLayer::MONSTER
		| (int)ECollisionLayer::MONSTER_WEAPON
		| (int)ECollisionLayer::MONSTER_PROJECTILE
		| (int)ECollisionLayer::ITEM,

		BOSS
		= (int)ECollisionLayer::PLAYER
		| (int)ECollisionLayer::PLAYER_WEAPON
		| (int)ECollisionLayer::PLAYER_PROJECTILE,

		MONSTER
		= (int)ECollisionLayer::PLAYER
		| (int)ECollisionLayer::PLAYER_WEAPON
		| (int)ECollisionLayer::PLAYER_PROJECTILE,

		PLAYER_WEAPON
		= (int)ECollisionLayer::BOSS
		| (int)ECollisionLayer::MONSTER
		| (int)ECollisionLayer::ENVIRONMENT
		| (int)ECollisionLayer::BUILDING,

		MONSTER_WEAPON
		= (int)ECollisionLayer::PLAYER
		| (int)ECollisionLayer::ENVIRONMENT
		| (int)ECollisionLayer::BUILDING,

		PLAYER_PROJECTILE
		= (int)ECollisionLayer::BOSS
		| (int)ECollisionLayer::MONSTER
		| (int)ECollisionLayer::ENVIRONMENT
		| (int)ECollisionLayer::BUILDING,

		MONSTER_PROJECTILE
		= (int)ECollisionLayer::PLAYER
		| (int)ECollisionLayer::ENVIRONMENT
		| (int)ECollisionLayer::BUILDING,

		ITEM
		= (int)ECollisionLayer::PLAYER,

		ENVIRONMENT
		= (int)ECollisionLayer::PLAYER_WEAPON
		| (int)ECollisionLayer::PLAYER
		| (int)ECollisionLayer::MONSTER_WEAPON
		| (int)ECollisionLayer::PLAYER_PROJECTILE
		| (int)ECollisionLayer::MONSTER_PROJECTILE,

		BUILDING
		= (int)ECollisionLayer::PLAYER_WEAPON
		| (int)ECollisionLayer::MONSTER_WEAPON
		| (int)ECollisionLayer::PLAYER
		| (int)ECollisionLayer::PLAYER_PROJECTILE
		| (int)ECollisionLayer::MONSTER_PROJECTILE,

		ENUM_END
	};

	/********************
		Input Enum
	********************/
	enum class EMouseState
	{
		DOWN,
		PRESSED,
		UP,
		RELEASED,
		ENUM_END
	};

	enum class EKeyState
	{
		DOWN,
		PRESSED,
		UP,
		RELEASED,
		ENUM_END
	};

	enum class EMouseActionType
	{
		LEFT_BUTTON,
		RIGHT_BUTTON,
		SCROLL_WHEEL_BUTTON,
		ENUM_END
	};

	enum class EMouseAxisType
	{
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		ENUM_END
	};
}
