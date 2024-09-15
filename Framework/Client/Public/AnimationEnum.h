#pragma once

#pragma region PLAYER ANIMATION

enum class EPLAYER_FEMALE
{
	AS_Player_Female_Aim_None,
	AS_Player_Female_Attack_None,
	AS_Player_Female_Attack_None_Additive,
	AS_Player_Female_BiggerHorseRide_None,
	AS_Player_Female_CarpetRide_JumpIn,
	AS_Player_Female_Climb_ClimpAtTop,
	AS_Player_Female_Climb_Down,
	AS_Player_Female_Climb_Idle,
	AS_Player_Female_Climb_Left,
	AS_Player_Female_Climb_Right,
	AS_Player_Female_Climb_Up,
	AS_Player_Female_CommonWork,
	AS_Player_Female_Cooking,
	AS_Player_Female_Crawl_Idle,
	AS_Player_Female_Crawl_Walk_Fwd,
	AS_Player_Female_Crouch_Aim,
	AS_Player_Female_Crouch_Architecture,
	AS_Player_Female_Crouch_Idle_None,
	AS_Player_Female_Crouch_Revive,
	AS_Player_Female_Crouch_Walk,
	AS_Player_Female_Damage,
	AS_Player_Female_Deforest_None,
	AS_Player_Female_Eat,
	AS_Player_Female_EggLauncherIdle,
	AS_Player_Female_EggLauncherShot,
	AS_Player_Female_Emote_Cheer,
	AS_Player_Female_Emote_Dance,
	AS_Player_Female_Emote_Wave,
	AS_Player_Female_Feeding,
	AS_Player_Female_FlipBwd,
	AS_Player_Female_Glide_Idle,
	AS_Player_Female_Glide_Start,
	AS_Player_Female_hit,
	AS_Player_Female_HorseRide_JumpIn_Front,
	AS_Player_Female_HorseRide_JumpIn_Left,
	AS_Player_Female_HorseRide_JumpIn_Rear,
	AS_Player_Female_HorseRide_JumpIn_Right,
	AS_Player_Female_HorseRide_None,
	AS_Player_Female_HorseRide_Yure,
	AS_Player_Female_HorseRide_Yure_None,
	AS_Player_Female_HumanRide_JumpIn,
	AS_Player_Female_Idle_None,
	AS_Player_Female_Jog_None,
	AS_Player_Female_JumpDownLoop_None,
	AS_Player_Female_JumpEnd_None,
	AS_Player_Female_JumpStart_None,
	AS_Player_Female_JumpUpLoop_None,
	AS_Player_Female_Mining,
	AS_Player_Female_PalCommand,
	AS_Player_Female_PalReturn,
	AS_Player_Female_Petting_Large,
	AS_Player_Female_Petting_Middle,
	AS_Player_Female_Petting_Middle_Beckon,
	AS_Player_Female_Petting_Small,
	AS_Player_Female_Petting_SmallMiddle,
	AS_Player_Female_Petting_Small_Beckon,
	AS_Player_Female_Planting_None,
	AS_Player_Female_RollFwd,
	AS_Player_Female_RollLeft,
	AS_Player_Female_RollRight,
	AS_Player_Female_SitRide_Yure,
	AS_Player_Female_SitRide_Yure_None,
	AS_Player_Female_SleepEnd,
	AS_Player_Female_SleepIn,
	AS_Player_Female_SleepLoop,
	AS_Player_Female_SleepOnSideLoop,
	AS_Player_Female_SlidingEnd,
	AS_Player_Female_SlidingLoop,
	AS_Player_Female_Sprint,
	AS_Player_Female_Swim,
	AS_Player_Female_Swim_Idle,
	AS_Player_Female_TwoHandGlide_Idle,
	AS_Player_Female_TwoHandGlide_Start,
	AS_Player_Female_Use_ReturnItem,
	AS_Player_Female_Wakeup_FaceDown,
	AS_Player_Female_WakeUp_Faceup,
	AS_Player_Female_Walk_None,
	AS_Player_Female_Watering_None,
	AS_Player_Female_Aim_AssaultRifle,
	AS_Player_Female_BiggerHorseRide_Aim_AssaultRifle,
	AS_Player_Female_BiggerHorseRide_Idle_AssaultRifle,
	AS_Player_Female_Crouch_Aim_AssaultRifle,
	AS_Player_Female_Crouch_Idle_AssaultRifle,
	AS_Player_Female_Crouch_WalkBwd_Aim_AssaultRifle,
	AS_Player_Female_Crouch_WalkFwd_Aim_AssaultRifle,
	AS_Player_Female_Crouch_WalkLeft_Aim_AssaultRifle,
	AS_Player_Female_Crouch_WalkRight_Aim_AssaultRifle,
	AS_Player_Female_Crouch_Walk_AssaultRifle,
	AS_Player_Female_Draw_AssaultRifle,
	AS_Player_Female_Fire_AssaultRifle,
	AS_Player_Female_HorseRide_Aim_AssaultRifle,
	AS_Player_Female_HorseRide_Idle_AssaultRifle,
	AS_Player_Female_Idle_AssaultRifle,
	AS_Player_Female_Idle_Rifle_Hip_AssaultRifle,
	AS_Player_Female_JogBwd_Aim_AssaultRifle,
	AS_Player_Female_JogFwd_Aim_AssaultRifle,
	AS_Player_Female_JogLeft_Aim_AssaultRifle,
	AS_Player_Female_JogRight_Aim_AssaultRifle,
	AS_Player_Female_Jog_AssaultRifle,
	AS_Player_Female_JumpDownLoop_AssaultRifle,
	AS_Player_Female_JumpEnd_AssaultRifle,
	AS_Player_Female_JumpUpLoop_AssaultRifle,
	AS_Player_Female_Reload_AssaultRifle,
	AS_Player_Female_SlidingEnd_AssaultRifle,
	AS_Player_Female_SlidingLoop_AssaultRifle,
	AS_Player_Female_Sprint_AssaultRifle,
	AS_Player_Female_WalkBwd_Aim_AssaultRifle,
	AS_Player_Female_WalkFwd_Aim_AssaultRifle,
	AS_Player_Female_WalkLeft_Aim_AssaultRifle,
	AS_Player_Female_WalkRight_Aim_AssaultRifle,
	AS_Player_Female_Walk_AssaultRifle,
	AS_Player_Female_HorseRide_AssaultRifle_c,
	AS_Player_Female_HorseRide_AssaultRifle_d,
	AS_Player_Female_HorseRide_AssaultRifle_u,
	AS_Player_Female_Ride_AssaultRifle_lbc,
	AS_Player_Female_Ride_AssaultRifle_lbd,
	AS_Player_Female_Ride_AssaultRifle_lblc,
	AS_Player_Female_Ride_AssaultRifle_lbu,
	AS_Player_Female_Ride_AssaultRifle_lc,
	AS_Player_Female_Ride_AssaultRifle_ld,
	AS_Player_Female_Ride_AssaultRifle_lu,
	AS_Player_Female_Ride_AssaultRifle_rbc,
	AS_Player_Female_Ride_AssaultRifle_rbd,
	AS_Player_Female_Ride_AssaultRifle_rbu,
	AS_Player_Female_Ride_AssaultRifle_rc,
	AS_Player_Female_Ride_AssaultRifle_rc_Dummy,
	AS_Player_Female_Ride_AssaultRifle_rd,
	AS_Player_Female_Ride_AssaultRifle_ru,
	AS_Player_Female_Aim_Bow,
	AS_Player_Female_BiggerHorseRide_Aim_Bow,
	AS_Player_Female_BiggerHorseRide_Idle_Bow,
	AS_Player_Female_Crouch_Aim_Bow,
	AS_Player_Female_Crouch_Idle_Bow,
	AS_Player_Female_Crouch_Walk_Bow,
	AS_Player_Female_Draw_Bow,
	AS_Player_Female_FireChargeLoop_Bow,
	AS_Player_Female_FireCharge_Bow,
	AS_Player_Female_Fire_Bow,
	AS_Player_Female_HorseRide_Aim_Bow,
	AS_Player_Female_HorseRide_FireChargeLoop_Bow,
	AS_Player_Female_HorseRide_FireCharge_Bow,
	AS_Player_Female_HorseRide_Fire_Bow,
	AS_Player_Female_HorseRide_Idle_Bow,
	AS_Player_Female_Idle_Bow,
	AS_Player_Female_Jog_Bow,
	AS_Player_Female_JumpDownLoop_Bow,
	AS_Player_Female_JumpEnd_Bow,
	AS_Player_Female_JumpUpLoop_Bow,
	AS_Player_Female_SlidingEnd_Bow,
	AS_Player_Female_SlidingLoop_Bow,
	AS_Player_Female_Sprint_Bow,
	AS_Player_Female_Walk_Bow,
	AS_Player_Female_Reload_Bowgun,
	AS_Player_Female_Aim_DoubleBarrelShotgun,
	AS_Player_Female_Crouch_Aim_DoubleBarrelShotgun,
	AS_Player_Female_Crouch_Idle_DoubleBarrelShotgun,
	AS_Player_Female_Crouch_Walk_DoubleBarrelShotgun,
	AS_Player_Female_Draw_DoubleBarrelShotgun,
	AS_Player_Female_Idle_DoubleBarrelShotgun,
	AS_Player_Female_Jog_DoubleBarrelShotgun,
	AS_Player_Female_JumpDownLoop_DoubleBarrelShotgun,
	AS_Player_Female_JumpEnd_DoubleBarrelShotgun,
	AS_Player_Female_JumpUpLoop_DoubleBarrelShotgun,
	AS_Player_Female_Reload_DoubleBarrelShotgun,
	AS_Player_Female_SlidingEnd_DoubleBarrelShotgun,
	AS_Player_Female_SlidingLoop_DoubleBarrelShotgun,
	AS_Player_Female_Sprint_DoubleBarrelShotgun,
	AS_Player_Female_Walk_DoubleBarrelShotgun,
	AS_Player_Female_Aim_GrenadeLauncher,
	AS_Player_Female_Idle_GrenadeLauncher,
	AS_Player_Female_Reload_GrenadeLauncher,
	AS_Player_Female_Aim_Handgun,
	AS_Player_Female_BiggerHorseRide_Aim_Handgun,
	AS_Player_Female_BiggerHorseRide_Idle_Handgun,
	AS_Player_Female_Crouch_Aim_Handgun,
	AS_Player_Female_Crouch_Idle_Handgun,
	AS_Player_Female_Crouch_Walk_Handgun,
	AS_Player_Female_Draw_Handgun,
	AS_Player_Female_Fire_Handgun,
	AS_Player_Female_Grapple_Handgun,
	AS_Player_Female_HorseRide_Aim_Handgun,
	AS_Player_Female_HorseRide_Idle_Handgun,
	AS_Player_Female_Idle_Handgun,
	AS_Player_Female_Jog_Handgun,
	AS_Player_Female_JumpDownLoop_Handgun,
	AS_Player_Female_JumpEnd_Handgun,
	AS_Player_Female_JumpUpLoop_Handgun,
	AS_Player_Female_Reload_DecalGun,
	AS_Player_Female_Reload_Handgun,
	AS_Player_Female_SlidingEnd_Handgun,
	AS_Player_Female_SlidingLoop_Handgun,
	AS_Player_Female_Sprint_Handgun,
	AS_Player_Female_Walk_Handgun,
	AS_Player_Female_HorseRide_Handgun_c,
	AS_Player_Female_HorseRide_Handgun_cd,
	AS_Player_Female_HorseRide_Handgun_cu,
	AS_Player_Female_HorseRide_Handgun_lbc,
	AS_Player_Female_HorseRide_Handgun_lbd,
	AS_Player_Female_HorseRide_Handgun_lbu,
	AS_Player_Female_HorseRide_Handgun_lc,
	AS_Player_Female_HorseRide_Handgun_ld,
	AS_Player_Female_HorseRide_Handgun_lu,
	AS_Player_Female_HorseRide_Handgun_rbc,
	AS_Player_Female_HorseRide_Handgun_rbd,
	AS_Player_Female_HorseRide_Handgun_rbu,
	AS_Player_Female_HorseRide_Handgun_rc,
	AS_Player_Female_HorseRide_Handgun_rcc,
	AS_Player_Female_HorseRide_Handgun_rd,
	AS_Player_Female_HorseRide_Handgun_ru,
	AS_Player_Female_Aim_HeavyWeapon,
	AS_Player_Female_BiggerHorseRide_Aim_HeavyWeapon,
	AS_Player_Female_BiggerHorseRide_Idle_HeavyWeapon,
	AS_Player_Female_Crouch_Aim_HeavyWeapon,
	AS_Player_Female_Crouch_Idle_HeavyWeapon,
	AS_Player_Female_Crouch_Walk_HeavyWeapon,
	AS_Player_Female_Draw_HeavyWeapon,
	AS_Player_Female_Fire_HeavyWeapon,
	AS_Player_Female_HorseRide_Aim_HeavyWeapon,
	AS_Player_Female_HorseRide_Idle_HeavyWeapon,
	AS_Player_Female_Idle_HeavyWeapon,
	AS_Player_Female_Jog_HeavyWeapon,
	AS_Player_Female_JumpDownLoop_HeavyWeapon,
	AS_Player_Female_JumpEnd_HeavyWeapon,
	AS_Player_Female_JumpUpLoop_HeavyWeapon,
	AS_Player_Female_Reload_Flamethrower,
	AS_Player_Female_Reload_HeavyWeapon,
	AS_Player_Female_SlidingLoop_HeavyWeapon,
	AS_Player_Female_Sprint_HeavyWeapon,
	AS_Player_Female_HorseRide_HeavyWeapon_c,
	AS_Player_Female_HorseRide_HeavyWeapon_cd,
	AS_Player_Female_HorseRide_HeavyWeapon_cu,
	AS_Player_Female_HorseRide_HeavyWeapon_lbc,
	AS_Player_Female_HorseRide_HeavyWeapon_lbd,
	AS_Player_Female_HorseRide_HeavyWeapon_lbu,
	AS_Player_Female_HorseRide_HeavyWeapon_lc,
	AS_Player_Female_HorseRide_HeavyWeapon_ld,
	AS_Player_Female_HorseRide_HeavyWeapon_lu,
	AS_Player_Female_HorseRide_HeavyWeapon_rbc,
	AS_Player_Female_HorseRide_HeavyWeapon_rbd,
	AS_Player_Female_HorseRide_HeavyWeapon_rbu,
	AS_Player_Female_HorseRide_HeavyWeapon_rc,
	AS_Player_Female_HorseRide_HeavyWeapon_rd,
	AS_Player_Female_HorseRide_HeavyWeapon_ru,
	AS_Player_Female_Aim_LaserRifle,
	AS_Player_Female_Reload_LaserRifle,
	AS_Player_Female_Attack_Melee,
	AS_Player_Female_Attack_Melee_Additive,
	AS_Player_Female_BiggerHorseRide_Aim_Melee,
	AS_Player_Female_BiggerHorseRide_Idle_Melee,
	AS_Player_Female_Crouch_Idle_Melee,
	AS_Player_Female_Crouch_Walkbwd_Aim_Melee,
	AS_Player_Female_Crouch_WalkFwd_Aim_Melee,
	AS_Player_Female_Crouch_WalkLeft_Aim_Melee,
	AS_Player_Female_Crouch_WalkRight_Aim_Melee,
	AS_Player_Female_Crouch_Walk_Melee,
	AS_Player_Female_Dismembering_Melee,
	AS_Player_Female_Draw_Melee,
	AS_Player_Female_HorseRide_Aim_Melee,
	AS_Player_Female_HorseRide_Idle_Melee,
	AS_Player_Female_Idle_Melee,
	AS_Player_Female_JogLeft_Aim_Melee,
	AS_Player_Female_JogRight_Aim_Melee,
	AS_Player_Female_Jog_Melee,
	AS_Player_Female_JumpDownLoop_Melee,
	AS_Player_Female_JumpEnd_Melee,
	AS_Player_Female_JumpUpLoop_Melee,
	AS_Player_Female_SlidingEnd_Melee,
	AS_Player_Female_SlidingLoop_Melee,
	AS_Player_Female_Sprint_Melee,
	AS_Player_Female_Walk_Melee,
	AS_Player_Female_HorseRide_Melee_c,
	AS_Player_Female_HorseRide_Melee_d,
	AS_Player_Female_HorseRide_Melee_lbc,
	AS_Player_Female_HorseRide_Melee_lbd,
	AS_Player_Female_HorseRide_Melee_lbu,
	AS_Player_Female_HorseRide_Melee_lc,
	AS_Player_Female_HorseRide_Melee_ld,
	AS_Player_Female_HorseRide_Melee_lu,
	AS_Player_Female_HorseRide_Melee_rbc,
	AS_Player_Female_HorseRide_Melee_rbd,
	AS_Player_Female_HorseRide_Melee_rbu,
	AS_Player_Female_HorseRide_Melee_rc,
	AS_Player_Female_HorseRide_Melee_rd,
	AS_Player_Female_HorseRide_Melee_ru,
	AS_Player_Female_HorseRide_Melee_u,
	AS_Player_Female_Attack_Spear,
	AS_Player_Female_Crouch_Idle_Spear,
	AS_Player_Female_Crouch_Walk_Spear,
	AS_Player_Female_Idle_Spear,
	AS_Player_Female_Jog_Spear,
	AS_Player_Female_SlidingLoop_Spear,
	AS_Player_Female_Walk_Spear,
	AS_Player_Female_Aim_MissileLauncher,
	AS_Player_Female_BiggerHorseRide_Aim_MissileLauncher,
	AS_Player_Female_BiggerHorseRide_Idle_MissileLauncher,
	AS_Player_Female_Crouch_Aim_MissileLauncher,
	AS_Player_Female_Crouch_Idle_MissileLauncher,
	AS_Player_Female_Crouch_Walk_MissileLauncher,
	AS_Player_Female_Draw_MissileLauncher,
	AS_Player_Female_HorseRide_Aim_MissileLauncher,
	AS_Player_Female_HorseRide_Idle_MissileLauncher,
	AS_Player_Female_Idle_MissileLauncher,
	AS_Player_Female_Jog_MissileLauncher,
	AS_Player_Female_JumpDownLoop_MissileLauncher,
	AS_Player_Female_JumpEnd_MissileLauncher,
	AS_Player_Female_JumpUpLoop_MissileLauncher,
	AS_Player_Female_Reload_MissileLauncher,
	AS_Player_Female_Reload_MultiGuidedMissileLauncher,
	AS_Player_Female_SlidingLoop_MissileLauncher,
	AS_Player_Female_Sprint_MissileLauncher,
	AS_Player_Female_Walk_MissileLauncher,
	AS_Player_Female_Reload_Musket,
	AS_Player_Female_Crouch_Idle_EggLauncher,
	AS_Player_Female_Crouch_Idle_PartnerSkill_Kitsunebi,
	AS_Player_Female_Crouch_Walk_EggLauncher,
	AS_Player_Female_Crouch_Walk_PartnerSkill_Kitsunebi,
	AS_Player_Female_Idle_PartnerSkill_Kitsunebi,
	AS_Player_Female_Jog_EggLauncher,
	AS_Player_Female_Jog_PartnerSkill_Kitsunebi,
	AS_Player_Female_JumpDownLoop_PartnerSkill_Kitsunebi,
	AS_Player_Female_JumpEnd_PartnerSkill_Kitsunebi,
	AS_Player_Female_JumpUpLoop_PartnerSkill_Kitsunebi,
	AS_Player_Female_SheepShield_End,
	AS_Player_Female_SheepShield_Loop,
	AS_Player_Female_SheepShield_Start,
	AS_Player_Female_SlidingLoop_PartnerSkill_Kitsunebi,
	AS_Player_Female_Sprint_EggLauncher,
	AS_Player_Female_Sprint_PartnerSkill_Kitsunebi,
	AS_Player_Female_Aim_PumpActionShotgun,
	AS_Player_Female_BiggerHorseRide_Aim_PumpActionShotgun,
	AS_Player_Female_BiggerHorseRide_Idle_PumpActionShotgun,
	AS_Player_Female_Crouch_Aim_PumpActionShotgun,
	AS_Player_Female_Crouch_Idle_PumpActionShotgun,
	AS_Player_Female_Crouch_Walk_PumpActionShotgun,
	AS_Player_Female_Draw_PumpActionShotgun,
	AS_Player_Female_Fire_PumpActionShotgun,
	AS_Player_Female_HorseRide_Aim_PumpActionShotgun,
	AS_Player_Female_HorseRide_Idle_PumpActionShotgun,
	AS_Player_Female_Idle_PumpActionShotgun,
	AS_Player_Female_Jog_PumpActionShotgun,
	AS_Player_Female_JumpDownLoop_PumpActionShotgun,
	AS_Player_Female_JumpEnd_PumpActionShotgun,
	AS_Player_Female_JumpUpLoop_PumpActionShotgun,
	AS_Player_Female_Reload_PumpActionShotgun,
	AS_Player_Female_SlidingEnd_PumpActionShotgun,
	AS_Player_Female_SlidingLoop_PumpActionShotgun,
	AS_Player_Female_Sprint_PumpActionShotgun,
	AS_Player_Female_Walk_PumpActionShotgun,
	AS_Player_Female_Horseride_PumpActionShotgun_c,
	AS_Player_Female_Horseride_PumpActionShotgun_cd,
	AS_Player_Female_Horseride_PumpActionShotgun_cu,
	AS_Player_Female_Horseride_PumpActionShotgun_lbc,
	AS_Player_Female_Horseride_PumpActionShotgun_lbd,
	AS_Player_Female_Horseride_PumpActionShotgun_lbu,
	AS_Player_Female_Horseride_PumpActionShotgun_lc,
	AS_Player_Female_Horseride_PumpActionShotgun_ld,
	AS_Player_Female_Horseride_PumpActionShotgun_lu,
	AS_Player_Female_Horseride_PumpActionShotgun_rbc,
	AS_Player_Female_Horseride_PumpActionShotgun_rbd,
	AS_Player_Female_Horseride_PumpActionShotgun_rbrc,
	AS_Player_Female_Horseride_PumpActionShotgun_rbu,
	AS_Player_Female_Horseride_PumpActionShotgun_rc,
	AS_Player_Female_Horseride_PumpActionShotgun_rc_dummy,
	AS_Player_Female_Horseride_PumpActionShotgun_rd,
	AS_Player_Female_Horseride_PumpActionShotgun_ru,
	AS_Player_Female_Aim_RocketLauncher,
	AS_Player_Female_BiggerHorseRide_Aim_RocketLauncher,
	AS_Player_Female_BiggerHorseRide_Idle_RocketLauncher,
	AS_Player_Female_Crouch_Aim_RocketLauncher,
	AS_Player_Female_Crouch_Idle_RocketLauncher,
	AS_Player_Female_Crouch_Walk_RocketLauncher,
	AS_Player_Female_Draw_RocketLauncher,
	AS_Player_Female_HorseRide_Aim_RocketLauncher,
	AS_Player_Female_HorseRide_Idle_RocketLauncher,
	AS_Player_Female_Idle_RocketLauncher,
	AS_Player_Female_Jog_RocketLauncher,
	AS_Player_Female_JumpDownLoop_RocketLauncher,
	AS_Player_Female_JumpEnd_RocketLauncher,
	AS_Player_Female_JumpUpLoop_RocketLauncher,
	AS_Player_Female_Reload_RocketLauncher,
	AS_Player_Female_SlidingLoop_RocketLauncher,
	AS_Player_Female_Sprint_RocketLauncher,
	AS_Player_Female_Walk_RocketLauncher,
	AS_Player_Female_Aim_SingleShotRifle,
	AS_Player_Female_Crouch_Aim_SingleShotRifle,
	AS_Player_Female_Crouch_Idle_SingleShotRifle,
	AS_Player_Female_Crouch_Walk_SingleShotRifle_,
	AS_Player_Female_Draw_SingleShotRifle,
	AS_Player_Female_Fire_SingleShotRifle,
	AS_Player_Female_HorseRide_Aim_SingleShotRifle,
	AS_Player_Female_HorseRide_Idle_SingleShotRifle,
	AS_Player_Female_Idle_SingleShotRifle,
	AS_Player_Female_Jog_SingleShotRifle,
	AS_Player_Female_JumpDownLoop_SingleShotRifle,
	AS_Player_Female_JumpEnd_SingleShotRifle,
	AS_Player_Female_JumpUpLoop_SingleShotRifle,
	AS_Player_Female_Reload_SingleShotRifle,
	AS_Player_Female_SlidingLoop_SingleShotRifle,
	AS_Player_Female_Sprint_SingleShotRifle,
	AS_Player_Female_Walk_SingleShotRifle,
	AS_Player_Female_Aim_Throw,
	AS_Player_Female_BiggerHorseRide_Aim_Throw,
	AS_Player_Female_BiggerHorseRide_Fire_Throw,
	AS_Player_Female_Crouch_Aim_Throw,
	AS_Player_Female_Crouch_Idle_Throw,
	AS_Player_Female_Crouch_Walk_Throw,
	AS_Player_Female_HorseRide_Aim_Throw,
	AS_Player_Female_HorseRide_Fire_Throw_Aim,
	AS_Player_Female_HorseRide_Throw_Left,
	AS_Player_Female_HorseRide_Throw_Right,
	AS_Player_Female_Idle_Throw,
	AS_Player_Female_Jog_Throw,
	AS_Player_Female_JumpDownLoop_Throw,
	AS_Player_Female_JumpEnd_Throw,
	AS_Player_Female_JumpUpLoop_Throw,
	AS_Player_Female_SlidingEnd_Throw,
	AS_Player_Female_SlidingLoop_Throw,
	AS_Player_Female_Sprint_Throw,
	AS_Player_Female_Throw,
	AS_Player_Female_HorseRide_Throw_c,
	AS_Player_Female_HorseRide_Throw_cd,
	AS_Player_Female_HorseRide_Throw_cu,
	AS_Player_Female_HorseRide_Throw_lbc,
	AS_Player_Female_HorseRide_Throw_lbd,
	AS_Player_Female_HorseRide_Throw_lbu,
	AS_Player_Female_HorseRide_Throw_lc,
	AS_Player_Female_HorseRide_Throw_ld,
	AS_Player_Female_HorseRide_Throw_lu,
	AS_Player_Female_HorseRide_Throw_rbc,
	AS_Player_Female_HorseRide_Throw_rbd,
	AS_Player_Female_HorseRide_Throw_rbu,
	AS_Player_Female_HorseRide_Throw_rc,
	AS_Player_Female_HorseRide_Throw_rd,
	AS_Player_Female_HorseRide_Throw_ru,
	AS_Player_Female_Grab_PalCarry,
	AS_Player_Female_Idle_PalCarry,
	AS_Player_Female_Jog_PalCarry,
	AS_Player_Female_JumpDownLoop_PalCarry,
	AS_Player_Female_JumpEnd_PalCarry,
	AS_Player_Female_JumpUpLoop_PalCarry,
	AS_Player_Female_Sprint_PalCarry,
	AS_Player_Female_Throw_PalCarry,
	AS_Player_Female_BlackFurDragonRide_Idle,
	AS_Player_Female_ElecPandaRide_Idle,
	AS_Player_Female_LazyDragonRide_Idle
};

enum class EASSAULTRIFLE
{
	AS_AssaultRifle_Empty,
	AS_AssaultRifle_Fire,
	AS_Reload_AssaultRifle_001,
	AS_Weapon_AssaultRifle_002_Reload,
	AS_Weapon_AssaultRifle_002_SpinUp,
	AS_AssaultRifle_003_Fire,
	AS_AssaultRifle_003_Idle,
	AS_AssaultRifle_003_Reload,
	AS_AssaultRifle_004_Fire,
	AS_AssaultRifle_005_Fire,
	AS_AssaultRifle_005_Idle,
	AS_AssaultRifle_005_Reload,
	AS_Weapon_AssaultRifle_006_Reload
};

enum class EBOW
{
	AS_Bow_002_Fire,
	AS_Bow_002_FireCharge,
	AS_Bow_002_FireChargeLoop,
	AS_Bow_003_Fire,
	AS_Bow_003_FireCharge,
	AS_Bow_003_FireChargeLoop,
	AS_Bow_003_Idle
};

enum class ECONSTRUCTIONTOOL
{
	AS_ConstructionTool_002_Cooking
};

enum class EGRENADELAUNCHER
{
	AS_Weapon_GrenadeLauncher_001_Reload
};

enum class EPALSPHERE
{
	AS_PalSphereCapture
};

enum class EROCKETLAUNCHER
{
	AS_Weapon_RocketLauncher_003_Reload,
	AS_Weapon_RocketLauncher_004_Reload
};

#pragma endregion

#pragma region PAL ANIMATION

enum class EAnubis
{
	AS_Anubis_Architecture,
	AS_Anubis_Carrying,
	AS_Anubis_Carrying_Start,
	AS_Anubis_CommonWork,
	AS_Anubis_Damage,
	AS_Anubis_Dodge_Left,
	AS_Anubis_Dodge_Right,
	AS_Anubis_Eat,
	AS_Anubis_Encount,
	AS_Anubis_Farskill_Action,
	AS_Anubis_Farskill_ActionLoop,
	AS_Anubis_Farskill_End,
	AS_Anubis_Farskill_Start,
	AS_Anubis_Farskill_StartLoop,
	AS_Anubis_Getup_BackDown,
	AS_Anubis_Getup_FrontDown,
	AS_Anubis_Hit,
	AS_Anubis_idle,
	AS_Anubis_JumpEnd,
	AS_Anubis_JumpLoop,
	AS_Anubis_Nearskill_Action,
	AS_Anubis_Nearskill_ActionLoop,
	AS_Anubis_Nearskill_End,
	AS_Anubis_Nearskill_Start,
	AS_Anubis_Nearskill_StartLoop,
	AS_Anubis_Petting,
	AS_Anubis_Rest01,
	AS_Anubis_Rest02,
	AS_Anubis_Rest03,
	AS_Anubis_Run,
	AS_Anubis_Skill_JimenNaguri_DownLoop,
	AS_Anubis_Skill_JimenNaguri_End,
	AS_Anubis_Skill_JimenNaguri_Start,
	AS_Anubis_Skill_JimenNaguri_UpLoop,
	AS_Anubis_Skill_MawashiGeri,
	AS_Anubis_Skill_Tossin_End,
	AS_Anubis_Skill_Tossin_Loop,
	AS_Anubis_Skill_Tossin_Start,
	AS_Anubis_SleepEnd,
	AS_Anubis_SleepLoop,
	AS_Anubis_SleepStart,
	AS_Anubis_Stun,
	AS_Anubis_Transport,
	AS_Anubis_Walk
};

enum class EBlackGriffon
{
	AS_BlackGriffon_Carrying,
	AS_BlackGriffon_Carrying_Start,
	AS_BlackGriffon_Damage,
	AS_BlackGriffon_Eat,
	AS_BlackGriffon_Encount,
	AS_BlackGriffon_FarSkill_Action,
	AS_BlackGriffon_FarSkill_ActionLoop,
	AS_BlackGriffon_FarSkill_End,
	AS_BlackGriffon_FarSkill_Start,
	AS_BlackGriffon_FarSkill_StartLoop,
	AS_BlackGriffon_GetUp_LeftDown,
	AS_BlackGriffon_GetUp_RightDown,
	AS_BlackGriffon_Hit,
	AS_BlackGriffon_Idle,
	AS_BlackGriffon_Idle_FlyingRiding,
	AS_BlackGriffon_JumpEnd,
	AS_BlackGriffon_JumpLoop,
	AS_BlackGriffon_Petting,
	AS_BlackGriffon_Rest01,
	AS_BlackGriffon_Rest02,
	AS_BlackGriffon_Run,
	AS_BlackGriffon_Run_FlyingRiding,
	AS_BlackGriffon_Run_Riding,
	AS_BlackGriffon_SleepEnd,
	AS_BlackGriffon_SleepLoop,
	AS_BlackGriffon_SleepStart,
	AS_BlackGriffon_Spa,
	AS_BlackGriffon_TackleLaser_End,
	AS_BlackGriffon_TackleLaser_Loop,
	AS_BlackGriffon_TackleLaser_Start,
	AS_BlackGriffon_Walk,
	AS_BlackGriffon_Walk_FlyingRiding
};

enum class ECarbunclo
{
	AS_Carbunclo_Architecture,
	AS_Carbunclo_Carrying,
	AS_Carbunclo_Carrying_Start,
	AS_Carbunclo_Climb,
	AS_Carbunclo_CommonWork,
	AS_Carbunclo_Damage,
	AS_Carbunclo_Deforest,
	AS_Carbunclo_Eat,
	AS_Carbunclo_Encount,
	AS_Carbunclo_FarSkill_Action,
	AS_Carbunclo_FarSkill_ActionLoop,
	AS_Carbunclo_FarSkill_End,
	AS_Carbunclo_FarSkill_Start,
	AS_Carbunclo_FarSkill_StartLoop,
	AS_Carbunclo_GetUp_FrontDown,
	AS_Carbunclo_Hit,
	AS_Carbunclo_Idle,
	AS_Carbunclo_Jump_End,
	AS_Carbunclo_Jump_Loop,
	AS_Carbunclo_Jump_Start,
	AS_Carbunclo_NearSkill_Action,
	AS_Carbunclo_NearSkill_ActionLoop,
	AS_Carbunclo_NearSkill_End,
	AS_Carbunclo_NearSkill_Start,
	AS_Carbunclo_NearSkill_StartLoop,
	AS_Carbunclo_OnHead_Aim,
	AS_Carbunclo_OnHead_Idle,
	AS_Carbunclo_OnHead_Shoot,
	AS_Carbunclo_Petting,
	AS_Carbunclo_Planting,
	AS_Carbunclo_Rest01,
	AS_Carbunclo_Rest02,
	AS_Carbunclo_Rest03,
	AS_Carbunclo_Run,
	AS_Carbunclo_Sleep,
	AS_Carbunclo_SleepEnd,
	AS_Carbunclo_SleepStart,
	AS_Carbunclo_Spa,
	AS_Carbunclo_Walk
};

enum class EChickenPal
{
	AS_ChickenPal_Carrying,
	AS_ChickenPal_Carrying_Start,
	AS_ChickenPal_ChickenTsutsuki,
	AS_ChickenPal_Damage,
	AS_ChickenPal_Eat,
	AS_ChickenPal_Encount,
	AS_ChickenPal_FarSkill_Action,
	AS_ChickenPal_FarSkill_ActionLoop,
	AS_ChickenPal_FarSkill_End,
	AS_ChickenPal_FarSkill_Start,
	AS_ChickenPal_FarSkill_StartLoop,
	AS_ChickenPal_GetUp_FrontDown,
	AS_ChickenPal_Hit,
	AS_ChickenPal_Idle,
	AS_ChickenPal_JumpEnd,
	AS_ChickenPal_JumpLoop,
	AS_ChickenPal_Petting,
	AS_ChickenPal_Rest01,
	AS_ChickenPal_Rest02,
	AS_ChickenPal_Rest03,
	AS_ChickenPal_Run,
	AS_ChickenPal_SleepEnd,
	AS_ChickenPal_SleepLoop,
	AS_ChickenPal_SleepStart,
	AS_ChickenPal_Spa,
	AS_ChickenPal_Stun,
	AS_ChickenPal_Walk
};

enum class EDreamDemon
{
	AS_DreamDemon_Architecture,
	AS_DreamDemon_Carrying,
	AS_DreamDemon_Carrying_Start,
	AS_DreamDemon_CommonWork,
	AS_DreamDemon_Damage,
	AS_DreamDemon_Eat,
	AS_DreamDemon_Encount,
	AS_DreamDemon_FarSkill_Action,
	AS_DreamDemon_FarSkill_ActionLoop,
	AS_DreamDemon_FarSkill_End,
	AS_DreamDemon_FarSkill_Start,
	AS_DreamDemon_FarSkill_StartLoop,
	AS_DreamDemon_GetUp_FrontDown,
	AS_DreamDemon_Hit,
	AS_DreamDemon_Idle,
	AS_DreamDemon_JumpEnd,
	AS_DreamDemon_JumpLoop,
	AS_DreamDemon_Petting,
	AS_DreamDemon_Rest01,
	AS_DreamDemon_Rest02,
	AS_DreamDemon_Rest03,
	AS_DreamDemon_Rest04,
	AS_DreamDemon_Run,
	AS_DreamDemon_SleepEnd,
	AS_DreamDemon_SleepLoop,
	AS_DreamDemon_SleepStart,
	AS_DreamDemon_Spa,
	AS_DreamDemon_Transport,
	AS_DreamDemon_Walk
};

enum class EElecPanda
{
	AS_ElecPanda_Architecture,
	AS_ElecPanda_Carrying,
	AS_ElecPanda_Carrying_Start,
	AS_ElecPanda_CommonWork,
	AS_ElecPanda_Damage,
	AS_ElecPanda_Deforest,
	AS_ElecPanda_Eat,
	AS_ElecPanda_Encount,
	AS_ElecPanda_FarSkill_Action,
	AS_ElecPanda_FarSkill_ActionLoop,
	AS_ElecPanda_FarSkill_End,
	AS_ElecPanda_FarSkill_Start,
	AS_ElecPanda_FarSkill_StartLoop,
	AS_ElecPanda_GetUp_FrontDown,
	AS_ElecPanda_Hikkaki_Action_01,
	AS_ElecPanda_Hikkaki_Action_02,
	AS_ElecPanda_Hikkaki_Action_03,
	AS_ElecPanda_Hikkaki_End,
	AS_ElecPanda_Hikkaki_Start,
	AS_ElecPanda_Hit,
	AS_ElecPanda_Idle,
	AS_ElecPanda_JumpEnd,
	AS_ElecPanda_JumpLoop,
	AS_ElecPanda_Minigun_Idle,
	AS_ElecPanda_Minigun_WalkBack,
	AS_ElecPanda_Minigun_WalkFwd,
	AS_ElecPanda_Minigun_WalkLeft,
	AS_ElecPanda_Minigun_WalkRight,
	AS_ElecPanda_Petting,
	AS_ElecPanda_Rest01,
	AS_ElecPanda_Rest02,
	AS_ElecPanda_Rest03,
	AS_ElecPanda_Run,
	AS_ElecPanda_SleepEnd,
	AS_ElecPanda_SleepLoop,
	AS_ElecPanda_SleepStart,
	AS_ElecPanda_Transport,
	AS_ElecPanda_Walk
};

enum class EFireKirin
{
	AS_FireKirin_Carrying,
	AS_FireKirin_Carrying_Start,
	AS_FireKirin_Cooking,
	AS_FireKirin_Damage,
	AS_FireKirin_Deforest,
	AS_FireKirin_Eat,
	AS_FireKirin_Encount,
	AS_FireKirin_FarSkill_Action,
	AS_FireKirin_FarSkill_ActionLoop,
	AS_FireKirin_FarSkill_End,
	AS_FireKirin_FarSkill_Start,
	AS_FireKirin_FarSkill_StartLoop,
	AS_FireKirin_GetUp_LeftDown,
	AS_FireKirin_GetUp_RightDown,
	AS_FireKirin_Hit,
	AS_FireKirin_Idle,
	AS_FireKirin_JumpEnd,
	AS_FireKirin_JumpLoop,
	AS_FireKirin_JumpStart,
	AS_FireKirin_Petting,
	AS_FireKirin_Rest01,
	AS_FireKirin_Rest02,
	AS_FireKirin_Run,
	AS_FireKirin_SleepEnd,
	AS_FireKirin_SleepLoop,
	AS_FireKirin_SleepStart,
	AS_FireKirin_Tossin_Loop,
	AS_FireKirin_Tossin_Start,
	AS_FireKirin_Walk
};

enum class EHedgeHog
{
	AS_Hedgehog_Ball,
	AS_Hedgehog_Carrying,
	AS_Hedgehog_Carrying_Start,
	AS_Hedgehog_Damage,
	AS_Hedgehog_Eat,
	AS_Hedgehog_Encount,
	AS_Hedgehog_FarSkill_Action,
	AS_Hedgehog_FarSkill_ActionLoop,
	AS_Hedgehog_FarSkill_End,
	AS_Hedgehog_FarSkill_Start,
	AS_Hedgehog_FarSkill_StartLoop,
	AS_Hedgehog_GetUp_LeftDown,
	AS_Hedgehog_GetUp_RightDown,
	AS_Hedgehog_Hit,
	AS_Hedgehog_Idle,
	AS_Hedgehog_JumpEnd,
	AS_Hedgehog_JumpLoop,
	AS_Hedgehog_Petting,
	AS_Hedgehog_Rest01,
	AS_Hedgehog_Rest02,
	AS_Hedgehog_Rest03,
	AS_Hedgehog_Run,
	AS_Hedgehog_SleepEnd,
	AS_Hedgehog_SleepLoop,
	AS_Hedgehog_SleepStart,
	AS_Hedgehog_Spa,
	AS_Hedgehog_Stun,
	AS_Hedgehog_Walk
};

enum class EJetDragon
{
	AS_JetDragon_Carrying,
	AS_JetDragon_Carrying_Start,
	AS_JetDragon_Damage,
	AS_JetDragon_Eat,
	AS_JetDragon_Encount,
	AS_JetDragon_FarSkill_Action,
	AS_JetDragon_FarSkill_ActionLoop,
	AS_JetDragon_FarSkill_End,
	AS_JetDragon_FarSkill_Start,
	AS_JetDragon_FarSkill_StartLoop,
	AS_JetDragon_GetUp_LeftDown,
	AS_JetDragon_GetUp_RightDown,
	AS_JetDragon_Hit,
	AS_JetDragon_Idle,
	AS_JetDragon_JumpBeam_End,
	AS_JetDragon_JumpBeam_Loop,
	AS_JetDragon_JumpBeam_Start,
	AS_JetDragon_Petting,
	AS_JetDragon_Run,
	AS_JetDragon_Run_Riding,
	AS_JetDragon_SleepEnd,
	AS_JetDragon_SleepLoop,
	AS_JetDragon_SleepStart,
	AS_JetDragon_Spa,
	AS_JetDragon_Sprint,
	AS_JetDragon_Walk,
	AS_JetDragon_Walk_Riding
};

enum class EKitsunebi
{
	AS_Kitsunebi_Carrying,
	AS_Kitsunebi_Carrying_Start,
	AS_Kitsunebi_CookingLoop,
	AS_Kitsunebi_CookingStart,
	AS_Kitsunebi_Damage,
	AS_Kitsunebi_Eat,
	AS_Kitsunebi_Encount,
	AS_Kitsunebi_FarSkill_Action,
	AS_Kitsunebi_FarSkill_ActionLoop,
	AS_Kitsunebi_FarSkill_End,
	AS_Kitsunebi_FarSkill_Start,
	AS_Kitsunebi_FarSkill_StartLoop,
	AS_Kitsunebi_Flamethrower,
	AS_Kitsunebi_Flamethrower_Fire,
	AS_Kitsunebi_Flamethrower_Fire_Start,
	AS_Kitsunebi_GetUp_LeftDown,
	AS_Kitsunebi_GetUp_RightDown,
	AS_Kitsunebi_Hit,
	AS_Kitsunebi_Idle,
	AS_Kitsunebi_JumpEnd,
	AS_Kitsunebi_JumpLoop,
	AS_Kitsunebi_Petting,
	AS_Kitsunebi_Rest01,
	AS_Kitsunebi_Rest02,
	AS_Kitsunebi_Rest03,
	AS_Kitsunebi_Run,
	AS_Kitsunebi_SleepEnd,
	AS_Kitsunebi_SleepLoop,
	AS_Kitsunebi_SleepStart,
	AS_Kitsunebi_Walk
};

enum class ELazyDragon
{
	AS_Lazydragon_Aim,
	AS_LazyDragon_Aim_WalkBwd,
	AS_LazyDragon_Aim_WalkFwd,
	AS_LazyDragon_Aim_WalkLeft,
	AS_LazyDragon_Aim_WalkRight,
	AS_LazyDragon_Carrying,
	AS_LazyDragon_Carrying_Start,
	AS_LazyDragon_Damage,
	AS_LazyDragon_Eat,
	AS_LazyDragon_Encount,
	AS_LazyDragon_FarSkill_Action,
	AS_LazyDragon_FarSkill_ActionLoop,
	AS_LazyDragon_FarSkill_End,
	AS_LazyDragon_FarSkill_Start,
	AS_LazyDragon_FarSkill_StartLoop,
	AS_LazyDragon_GetUp_FrontDown,
	AS_LazyDragon_Hit,
	AS_LazyDragon_idle,
	AS_LazyDragon_JumpEnd,
	AS_LazyDragon_JumpLoop,
	AS_LazyDragon_Petting,
	AS_LazyDragon_Rest01,
	AS_LazyDragon_Rest02,
	AS_LazyDragon_Run,
	AS_LazyDragon_RunRiding,
	AS_LazyDragon_SleepEnd,
	AS_LazyDragon_SleepLoop,
	AS_LazyDragon_SleepStart,
	AS_LazyDragon_Spa,
	AS_LazyDragon_Transport,
	AS_LazyDragon_Walk,
	AS_LazyDragon_WalkRiding,
	AS_LazyDragon_Watering
};

enum class EPenguin
{
	AS_PenguinLauncher_Idle,
	AS_Penguin_Architecture,
	AS_Penguin_Carrying,
	AS_Penguin_Carrying_Start,
	AS_Penguin_CommonWork,
	AS_Penguin_Damage,
	AS_Penguin_Eat,
	AS_Penguin_Encount,
	AS_Penguin_FarSkill_Action,
	AS_Penguin_FarSkill_ActionLoop,
	AS_Penguin_FarSkill_End,
	AS_Penguin_FarSkill_Start,
	AS_Penguin_FarSkill_StartLoop,
	AS_Penguin_GetUp_FrontDown,
	AS_Penguin_Hit,
	AS_Penguin_Idle,
	AS_Penguin_JumpEnd,
	AS_Penguin_JumpLoop,
	AS_Penguin_Petting,
	AS_Penguin_Rest01,
	AS_Penguin_Rest02,
	AS_Penguin_Rest03,
	AS_Penguin_Run,
	AS_Penguin_SleepEnd,
	AS_Penguin_SleepLoop,
	AS_Penguin_SleepStart,
	AS_Penguin_Transport,
	AS_Penguin_Walk,
	AS_Penguin_Watering
};

enum class EPinkCat
{
	AS_PinkCat_Architecture,
	AS_PinkCat_Carrying,
	AS_PinkCat_Carrying_Start,
	AS_PinkCat_CommonWork,
	AS_PinkCat_Damage,
	AS_PinkCat_Eat,
	AS_PinkCat_Encount,
	AS_PinkCat_FarSkill_Action,
	AS_PinkCat_FarSkill_ActionLoop,
	AS_PinkCat_FarSkill_End,
	AS_PinkCat_FarSkill_Start,
	AS_PinkCat_FarSkill_StartLoop,
	AS_PinkCat_GetUp_FrontDown,
	AS_PinkCat_Hit,
	AS_PinkCat_Idle,
	AS_PinkCat_JumpEnd,
	AS_PinkCat_JumpLoop,
	AS_PinkCat_Mining,
	AS_PinkCat_NekoPunch_End,
	AS_PinkCat_NekoPunch_Loop,
	AS_PinkCat_NekoPunch_Start,
	AS_PinkCat_Petting,
	AS_PinkCat_Rest01,
	AS_PinkCat_Rest02,
	AS_PinkCat_Rest03,
	AS_PinkCat_Run,
	AS_PinkCat_SleepEnd,
	AS_PinkCat_SleepLoop,
	AS_PinkCat_SleepStart,
	AS_PinkCat_Transport,
	AS_PinkCat_Walk
};

enum class EUmihebi
{
	AS_Umihebi_Carrying,
	AS_Umihebi_Carrying_Start,
	AS_Umihebi_Cooking,
	AS_Umihebi_Damage,
	AS_Umihebi_Eat,
	AS_Umihebi_Encount,
	AS_Umihebi_FarSkill_Action,
	AS_Umihebi_FarSkill_ActionLoop,
	AS_Umihebi_FarSkill_End,
	AS_Umihebi_FarSkill_Start,
	AS_Umihebi_FarSkill_StartLoop,
	AS_Umihebi_GetUp_FrontDown,
	AS_Umihebi_Hit,
	AS_Umihebi_Idle,
	AS_Umihebi_JumpEnd,
	AS_Umihebi_JumpLoop,
	AS_Umihebi_Petting,
	AS_Umihebi_Rest01,
	AS_Umihebi_Run,
	AS_Umihebi_SleepEnd,
	AS_Umihebi_SleepLoop,
	AS_Umihebi_SleepStart,
	AS_Umihebi_Spa,
	AS_Umihebi_Walk,
	AS_Umihebi_Watering
};

enum class ESheepBall
{
	AS_SheepBall_Architecture,
	AS_SheepBall_Carrying,
	AS_SheepBall_Carrying_Start,
	AS_SheepBall_CommonWork,
	AS_SheepBall_Damage,
	AS_SheepBall_Eat,
	AS_SheepBall_Encount,
	AS_SheepBall_FarSkill_Action,
	AS_SheepBall_FarSkill_ActionLoop,
	AS_SheepBall_FarSkill_End,
	AS_SheepBall_FarSkill_Start,
	AS_SheepBall_FarSkill_StartLoop,
	AS_SheepBall_GetUp_FrontDown,
	AS_SheepBall_Hit,
	AS_SheepBall_Idle,
	AS_SheepBall_JumpEnd,
	AS_SheepBall_JumpLoop,
	AS_SheepBall_Korogaru_End,
	AS_SheepBall_Korogaru_Loop,
	AS_SheepBall_Korogaru_Start,
	AS_SheepBall_Petting,
	AS_SheepBall_Rest01,
	AS_SheepBall_Rest02,
	AS_SheepBall_Rest03,
	AS_SheepBall_Run,
	AS_SheepBall_Shield,
	AS_SheepBall_SleepEnd,
	AS_SheepBall_SleepLoop,
	AS_SheepBall_SleepStart,
	AS_SheepBall_Transport,
	AS_SheepBall_Walk
};

enum class EWeaselDragon
{
	AS_WeaselDragon_Carrying,
	AS_WeaselDragon_Carrying_Start,
	AS_WeaselDragon_Damage,
	AS_WeaselDragon_Eat,
	AS_WeaselDragon_Encount,
	AS_WeaselDragon_FarSkill_Action,
	AS_WeaselDragon_FarSkill_ActionLoop,
	AS_WeaselDragon_FarSkill_End,
	AS_WeaselDragon_FarSkill_Start,
	AS_WeaselDragon_FarSkill_StartLoop,
	AS_WeaselDragon_FlyingTackle,
	AS_WeaselDragon_GetUp_FrontDown,
	AS_WeaselDragon_Hit,
	AS_WeaselDragon_Idle,
	AS_WeaselDragon_JumpEnd,
	AS_WeaselDragon_JumpLoop,
	AS_WeaselDragon_JumpStart,
	AS_WeaselDragon_Petting,
	AS_WeaselDragon_Rest01,
	AS_WeaselDragon_Rest02,
	AS_WeaselDragon_Rest03,
	AS_WeaselDragon_Run,
	AS_WeaselDragon_Run_Riding,
	AS_WeaselDragon_SleepEnd,
	AS_WeaselDragon_SleepLoop,
	AS_WeaselDragon_SleepStart,
	AS_WeaselDragon_Walk,
	AS_WeaselDragon_Walk_Riding
};

enum class EGrassMammoth
{
	AS_GrassMammoth_Carrying,
	AS_GrassMammoth_Carrying_Start,
	AS_GrassMammoth_Damage,
	AS_GrassMammoth_Deforest,
	AS_GrassMammoth_Earthquake,
	AS_GrassMammoth_Eat,
	AS_GrassMammoth_Encount,
	AS_GrassMammoth_FarSkill_Action,
	AS_GrassMammoth_FarSkill_ActionLoop,
	AS_GrassMammoth_FarSkill_End,
	AS_GrassMammoth_FarSkill_Start,
	AS_GrassMammoth_FarSkill_StartLoop,
	AS_GrassMammoth_GetUp_LeftDown,
	AS_GrassMammoth_GetUp_RightDown,
	AS_GrassMammoth_Hit,
	AS_GrassMammoth_Idle,
	AS_GrassMammoth_JumpEnd,
	AS_GrassMammoth_JumpLoop,
	AS_GrassMammoth_Mining,
	AS_GrassMammoth_Petting,
	AS_GrassMammoth_Planting,
	AS_GrassMammoth_Rest01,
	AS_GrassMammoth_Rest02,
	AS_GrassMammoth_Rest03,
	AS_GrassMammoth_Run,
	AS_GrassMammoth_SleepEnd,
	AS_GrassMammoth_SleepLoop,
	AS_GrassMammoth_SleepStart,
	AS_GrassMammoth_Spa,
	AS_GrassMammoth_Walk
};

enum class EBerryGoat
{
	AS_BerryGoat_Carrying,
	AS_BerryGoat_Carrying_Start,
	AS_BerryGoat_Damage,
	AS_BerryGoat_Eat,
	AS_BerryGoat_Encount,
	AS_BerryGoat_FarSkill_Action,
	AS_BerryGoat_FarSkill_ActionLoop,
	AS_BerryGoat_FarSkill_End,
	AS_BerryGoat_FarSkill_Start,
	AS_BerryGoat_FarSkill_StartLoop,
	AS_BerryGoat_GetUp_LeftDown,
	AS_BerryGoat_GetUp_RightDown,
	AS_BerryGoat_Hit,
	AS_BerryGoat_Idle,
	AS_BerryGoat_JumpEnd,
	AS_BerryGoat_JumpLoop,
	AS_BerryGoat_Petting,
	AS_BerryGoat_Planting,
	AS_BerryGoat_Rest01,
	AS_BerryGoat_Rest02,
	AS_BerryGoat_Rest03,
	AS_BerryGoat_Run,
	AS_BerryGoat_SleepEnd,
	AS_BerryGoat_SleepLoop,
	AS_BerryGoat_SleepStart,
	AS_BerryGoat_Walk
};

enum class EColorfulBird
{
	AS_ColofulBird_Carrying,
	AS_ColofulBird_Carrying_Start,
	AS_ColofulBird_Damage,
	AS_ColofulBird_Eat,
	AS_ColofulBird_EggLauncher_Idle,
	AS_ColofulBird_EggLauncher_Shot,
	AS_ColofulBird_Encount,
	AS_ColofulBird_FarSkill_Action,
	AS_ColofulBird_FarSkill_ActionLoop,
	AS_ColofulBird_FarSkill_End,
	AS_ColofulBird_FarSkill_Start,
	AS_ColofulBird_FarSkill_StartLoop,
	AS_ColofulBird_GetUp_FrontDown,
	AS_ColofulBird_Hit,
	AS_ColofulBird_Idle,
	AS_ColofulBird_JumpEnd,
	AS_ColofulBird_JumpLoop,
	AS_ColofulBird_Petting,
	AS_ColofulBird_Rest01,
	AS_ColofulBird_Rest02,
	AS_ColofulBird_Rest03,
	AS_ColofulBird_Run,
	AS_ColofulBird_SleepEnd,
	AS_ColofulBird_SleepLoop,
	AS_ColofulBird_SleepStart,
	AS_ColofulBird_Walk
};

enum class EFengyunDeeper
{
	AS_FengyunDeeper_Carrying,
	AS_FengyunDeeper_Carrying_Start,
	AS_FengyunDeeper_CloudTempest_End,
	AS_FengyunDeeper_CloudTempest_Loop,
	AS_FengyunDeeper_CloudTempest_Start,
	AS_FengyunDeeper_Damage,
	AS_FengyunDeeper_Deforest,
	AS_FengyunDeeper_Eat,
	AS_FengyunDeeper_Encount,
	AS_FengyunDeeper_FarSkill_Action,
	AS_FengyunDeeper_FarSkill_ActionLoop,
	AS_FengyunDeeper_FarSkill_End,
	AS_FengyunDeeper_FarSkill_Start,
	AS_FengyunDeeper_FarSkill_StartLoop,
	AS_FengyunDeeper_GetUp_LeftDown,
	AS_FengyunDeeper_GetUp_RightDown,
	AS_FengyunDeeper_Hit,
	AS_FengyunDeeper_Idle,
	AS_FengyunDeeper_JumpEnd,
	AS_FengyunDeeper_JumpLoop,
	AS_FengyunDeeper_JumpStart,
	AS_FengyunDeeper_Petting,
	AS_FengyunDeeper_Rest01,
	AS_FengyunDeeper_Rest02,
	AS_FengyunDeeper_Run,
	AS_FengyunDeeper_SleepEnd,
	AS_FengyunDeeper_SleepLoop,
	AS_FengyunDeeper_SleepStart,
	AS_FengyunDeeper_Walk
};

enum class EWoolFox
{
	AS_WoolFox_Carrying,
	AS_WoolFox_Carrying_Start,
	AS_WoolFox_Damage,
	AS_WoolFox_Eat,
	AS_WoolFox_Encount,
	AS_WoolFox_FarSkill_Action,
	AS_WoolFox_FarSkill_ActionLoop,
	AS_WoolFox_FarSkill_End,
	AS_WoolFox_FarSkill_Start,
	AS_WoolFox_FarSkill_StartLoop,
	AS_WoolFox_GetUp_LeftDown,
	AS_WoolFox_GetUp_RightDown,
	AS_WoolFox_Hit,
	AS_WoolFox_Idle,
	AS_WoolFox_JumpEnd,
	AS_WoolFox_JumpLoop,
	AS_WoolFox_Petting,
	AS_WoolFox_Rest01,
	AS_WoolFox_Rest02,
	AS_WoolFox_Rest03,
	AS_WoolFox_Rest04,
	AS_WoolFox_Run,
	AS_WoolFox_SleepEnd,
	AS_WoolFox_SleepLoop,
	AS_WoolFox_SleepStart,
	AS_WoolFox_Spa,
	AS_WoolFox_Walk
};

enum class EBoar
{
	AS_Boar_Carrying,
	AS_Boar_Carrying_Start,
	AS_Boar_Damage,
	AS_Boar_Eat,
	AS_Boar_Encount,
	AS_Boar_FarSkill_Action,
	AS_Boar_FarSkill_ActionLoop,
	AS_Boar_FarSkill_End,
	AS_Boar_FarSkill_Start,
	AS_Boar_FarSkill_StartLoop,
	AS_Boar_GetUp_LeftDown,
	AS_Boar_GetUp_RightDown,
	AS_Boar_Hit,
	AS_Boar_Idle,
	AS_Boar_JumpEnd,
	AS_Boar_JumpLoop,
	AS_Boar_Mining,
	AS_Boar_Petting,
	AS_Boar_Rest01,
	AS_Boar_Rest02,
	AS_Boar_Rest03,
	AS_Boar_Run,
	AS_Boar_SleepEnd,
	AS_Boar_SleepLoop,
	AS_Boar_SleepStart,
	AS_Boar_Tossin_End,
	AS_Boar_Tossin_Loop,
	AS_Boar_Tossin_Start,
	AS_Boar_Walk
};

#pragma endregion 

#pragma region NPC ANIMATION

enum class ENPCANIM
{
	AS_NPC_Female_Give,
	AS_NPC_Female_GroundSit,
	AS_NPC_Female_Surprise,
	AS_NPC_Female_Talking,
	AS_NPC_Female_UseDevice,
	AS_Player_Female_CommonWork,
	AS_Player_Female_Cooking,
	AS_Player_Female_Deforest_None,
	AS_Player_Female_Emote_Cheer,
	AS_Player_Female_Emote_Dance,
	AS_Player_Female_Emote_Wave,
	AS_Player_Female_Feeding,
	AS_Player_Female_Idle_None,
	AS_Player_Female_Mining,
	AS_Player_Female_Petting_Small,
	AS_Player_Female_SleepIn,
	AS_Player_Female_SleepLoop,
	AS_Player_Female_SleepOnSideLoop,
	AS_Player_Female_Wakeup_FaceDown,
	AS_Player_Female_WakeUp_Faceup,
	AS_Player_Female_Watering_None
};

#pragma endregion