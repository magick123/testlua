/*
--------------------------------------------------------------------
//Maker: No idea
//Updated & thanks go to: Rochet
--------------------------------------------------------------------
*/
#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Item.h"
#include "ItemTemplate.h"
#include "Player.h"
#include "Spellmgr.h"

enum Enchants
{
	ENCHANT_WEP_BLADE_WARD = 3869,
	ENCHANT_WEP_BLOOD_DRAINING = 3870,
	ENCHANT_WEP_BERSERKING = 3789,

	ENCHANT_WEP_ACCURACY = 3788,
	ENCHANT_WEP_AGILITY_1H = 1103,
	ENCHANT_WEP_AGILITY_2H = 2670,
	ENCHANT_WEP_SPIRIT = 3844,
	ENCHANT_WEP_BATTLEMASTER = 2675,
	ENCHANT_WEP_BLACK_MAGIC = 3790,
	ENCHANT_WEP_ICEBREAKER = 3239,
	ENCHANT_WEP_LIFEWARD = 3241,
	ENCHANT_WEP_MIGHTY_SPELL_POWER = 3834, // One-hand
	ENCHANT_WEP_GREATER_SPELL_POWER = 3854, // Two-hand (staff)
	ENCHANT_WEP_MONGOOSE = 2673,
	ENCHANT_WEP_EXECUTIONER = 3225,
	ENCHANT_WEP_POTENCY = 3833,
	ENCHANT_WEP_TITANGUARD = 3851,
	ENCHANT_WEP_MASSACRE = 3827, // Two-hand

	ENCHANT_WEP_CINDERGLACIER = 3369,
	ENCHANT_WEP_LICHBANE = 3366,
	ENCHANT_WEP_RAZORICE = 3370,
	ENCHANT_WEP_SPELLBREAKING = 3595,
	ENCHANT_WEP_SPELLSHATTERING = 3367, // Two-Hand
	ENCHANT_WEP_SWORDBREAKING = 3594,
	ENCHANT_WEP_SWORDSHATTERING = 3365, // Two-Hand
	ENCHANT_WEP_FALLEN_CRUSADER = 3368,
	ENCHANT_WEP_NERUBIAN_CARAPACE = 3883,
	ENCHANT_WEP_STONESKIN_GARGOYLE = 3847, // Two-Hand

	ENCHANT_WEP_TITANIUM_CHAIN = 3731,
	ENCHANT_WEP_ADAMANTITE_CHAIN = 3223, // New

	ENCHANT_SHIELD_DEFENSE = 1952,
	ENCHANT_SHIELD_INTELLECT = 1128,
	ENCHANT_SHIELD_MAJOR_STAMINA = 1071,
	ENCHANT_SHIELD_RESILIENCE = 3229,
	ENCHANT_SHIELD_RESISTANCE = 1888, // New
	ENCHANT_SHIELD_TOUGH_SHIELD = 2653, // New
	ENCHANT_SHIELD_LESSER_PROTECTION = 848, // New
	ENCHANT_SHIELD_VITALITY = 1890, // New

	ENCHANT_SHIELD_TITANIUM_PLATING = 3849,
	ENCHANT_SHIELD_TITANIUM_SPIKE = 3748,

	ENCHANT_HEAD_BLISSFUL_MENDING = 3819,
	ENCHANT_HEAD_BURNING_MYSTERIES = 3820,
	ENCHANT_HEAD_DOMINANCE = 3796,
	ENCHANT_HEAD_SAVAGE_GLADIATOR = 3842,
	ENCHANT_HEAD_STALWART_PROTECTOR = 3818,
	ENCHANT_HEAD_TORMENT = 3817,
	ENCHANT_HEAD_TRIUMPH = 3795,

	ENCHANT_SHOULDER_MASTERS_AXE = 3835, // Inscribe REQ
	ENCHANT_SHOULDER_MASTERS_CRAG = 3836, // Inscribe REQ
	ENCHANT_SHOULDER_MASTERS_PINNACLE = 3837, // Inscribe REQ
	ENCHANT_SHOULDER_MASTERS_STORM = 3838, // Inscribe REQ
	ENCHANT_SHOULDER_GREATER_AXE = 3808,
	ENCHANT_SHOULDER_GREATER_CRAG = 3809,
	ENCHANT_SHOULDER_GREATER_GLADIATOR = 3852,
	ENCHANT_SHOULDER_GREATER_PINNACLE = 3811,
	ENCHANT_SHOULDER_GREATER_STORM = 3810,
	ENCHANT_SHOULDER_DOMINANCE = 3794,
	ENCHANT_SHOULDER_TRIUMPH = 3793,

	ENCHANT_CLOAK_DARKGLOW_EMBROIDERY = 3728, // Inscribe REQ
	ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY = 3730, // Inscribe REQ
	ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY = 3722, // Inscribe REQ
	ENCHANT_CLOAK_FLEXWEAVE_UNDERLAY = 3605, // Engineer REQ
	ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE = 3859, // Engineer REQ
	ENCHANT_CLOAK_WISDOM = 3296,
	ENCHANT_CLOAK_TITANWEAVE = 1951,
	ENCHANT_CLOAK_SPELL_PIERCING = 3243,
	// ENCHANT_CLOAK_SHADOW_ARMOR = 3256, // Stealth Detection
	ENCHANT_CLOAK_MIGHTY_ARMOR = 3294,
	ENCHANT_CLOAK_MAJOR_AGILITY = 1099,
	ENCHANT_CLOAK_GREATER_SPEED = 3831,
	ENCHANT_CLOAK_DODGE = 2622, // New

	ENCHANT_CHEST_EXCEPTIONAL_MANA = 3233,
	ENCHANT_CHEST_GREATER_MANA_RESTO = 3533,
	ENCHANT_CHEST_EXCEPTIONAL_RESILIENCE = 3245,
	ENCHANT_CHEST_SUPER_HEALTH = 3297,
	ENCHANT_CHEST_ALL_STATS = 3832,
	ENCHANT_CHEST_GREATER_DEF = 1953, // New
	ENCHANT_CHEST_MAJOR_SPIRIT = 1144, // New
	ENCHANT_CHEST_LESSER_ABSORP = 63, // New
	ENCHANT_CHEST_RUNE_OF_WARDING = 2791, // New

	ENCHANT_BRACER_GREATER_ASSAULT = 3845,
	ENCHANT_BRACER_EXCEPTIONAL_INTELLECT = 1119, // Fixed 3458
	ENCHANT_BRACER_MAJOR_SPIRIT = 1147,
	//ENCHANT_BRACER_EXPERTISE = 3231, // No dupe
	ENCHANT_BRACER_GREATER_STATS = 2661,
	ENCHANT_BRACER_SUPERIOR_SPELLPOWER = 2332,
	ENCHANT_BRACER_MAJOR_STAMINA = 3850,
	ENCHANT_BRACER_BRAWN = 2647, // New
	ENCHANT_BRACER_RESTORE_MANA = 2679, // New
	ENCHANT_BRACER_MAJOR_DEFENSE = 2648, // New

	// Requires Leatherworking Skill
	ENCHANT_BRACER_FUR_LINING_AP = 3756,
	ENCHANT_BRACER_FUR_LINING_STAMINA = 3757,
	ENCHANT_BRACER_FUR_LINING_SP = 3758,
	ENCHANT_BRACER_ARCANE_RESIST = 3763,
	ENCHANT_BRACER_NATURE_RESIST = 3762,
	ENCHANT_BRACER_SHADOW_RESIST = 3761,
	ENCHANT_BRACER_FROST_RESIST = 3760,
	ENCHANT_BRACER_FIRE_RESIST = 3759,

	// Requires Blacksmith
	ENCHANT_BRACER_SOCKET_BRACER = 3717,
	ENCHANT_GLOVES_SOCKET_GLOVES = 3723,

	ENCHANT_GLOVES_CRUSHER = 1603,
	// EXTREMELY ugly hack for duplicate enchants
	ENCHANT_GLOVES_EXPERTISE = 3231, // Fixed -3231
	ENCHANT_GLOVES_PRECISION = 3234,
	ENCHANT_GLOVES_GREATER_ASSAULT = 1603,
	ENCHANT_GLOVES_MAJOR_AGILITY = 1097,
	ENCHANT_GLOVES_EXCEPTIONAL_SPELLPOWER = 3246, // Fixed 2330
	ENCHANT_GLOVES_ARMSMAN = 3253,
	ENCHANT_GLOVES_MAJOR_STR = 684, // New 
	ENCHANT_GLOVES_MINOR_HASTE = 931, // New 
	ENCHANT_GLOVES_GREATER_BLASTING = 3249, // New 
	ENCHANT_GLOVES_RIDING = 930, // New 

	// Requires Engineer
	ENCHANT_GLOVES_PYROROCKET = 3603,
	ENCHANT_GLOVES_HYPERSPEED_ACCELERATORS = 3604,
	ENCHANT_BELT_FRAG_BELT = 3601,
	ENCHANT_BOOTS_NITRO_BOOSTS = 3606,

	ENCHANT_BELT_ETERNAL_BELT_BUCKLE = 3729,

	ENCHANT_LEGS_FROSTHIDE_ARMOR = 3822,
	ENCHANT_LEGS_ICESCALE_ARMOR = 3823,
	ENCHANT_LEGS_EARTHEN_ARMOR = 3853,
	ENCHANT_LEGS_SAPPHIRE_SPELLTHREAD = 3721, // Fixed 3873
	ENCHANT_LEGS_BRILLIANT_SPELLTHREAD = 3719, // Fixed 3872

	ENCHANT_BOOTS_GREATER_VITALITY = 3244, // Fixed from Null
	ENCHANT_BOOTS_TUSKARRS_VITALITY = 3232,
	ENCHANT_BOOTS_ICEWALKER = 3826, // Fixed 2599
	ENCHANT_BOOTS_GREATER_ASSAULT = 1597,
	ENCHANT_BOOTS_GREATER_SPIRIT = 528,
	ENCHANT_BOOTS_GREATER_FORTITUDE = 1075, // Fixed 3259
	ENCHANT_BOOTS_SUPERIOR_AGILITY = 983,
	//ENCHANT_BOOTS_NITRO_BOOSTS = 3606,
	ENCHANT_BOOTS_CATS_SWIFTNESS = 2939, // New 

	// Requires Enchanting
	ENCHANT_RING_STAMINA = 3791,
	ENCHANT_RING_GREATER_SPELLPOWER = 3840,
	ENCHANT_RING_ASSAULT = 3839,

	ENCHANT_RANGED_HEARTSEEKER_SCOPE = 3608,
	ENCHANT_RANGED_SUN_SCOPE = 3607, // Add comma when ready to add below news
	ENCHANT_RANGED_BIZNICKS_SCOPE = 2523, // New
	ENCHANT_RANGED_REFRACTOR_SCOPE = 3843 // New & leave a comma out of this one
};

Item* PlayerAddItem(Player* player, uint32 item_id)
{
	uint8 count = 1;
	uint32 noSpaceForCount = 0;
	ItemPosCountVec dest;

	InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT,
		dest, item_id, count, &noSpaceForCount);
	if (msg != EQUIP_ERR_OK)
		count -= noSpaceForCount;

	//Item* item = player->StoreNewItem(dest, item_id, true, //outdated
		//Item::GenerateItemRandomPropertyId(item_id)); //outdated

	Item* item = player->StoreNewItem(dest, item_id, true, GenerateItemRandomPropertyId(item_id));

	if (noSpaceForCount > 0)
	{
		player->GetSession()->SendNotification("Please make room in your "
			"inventory first.");
		return NULL;
	}
	return item;
}

void EnchantWithItem(Player* player, uint32 item_id, Item* target)
{
	Item* item = PlayerAddItem(player, item_id);
	SpellCastTargets* targets = new SpellCastTargets();
	targets->SetItemTarget(target);
	player->CastItemUseSpell(item, *targets, 1, 0);
}

void EnchantWithSpell(Player* player, uint32 spell_id, Item* item)
{
	SpellCastTargets* targets = new SpellCastTargets();
	targets->SetItemTarget(item);
	player->CastSpell(*targets, sSpellMgr->GetSpellInfo(spell_id),
		NULL, TRIGGERED_IGNORE_POWER_AND_REAGENT_COST);
}

void Enchant(Player* player, Creature* creature, Item* item, uint32 enchantid)
{
	if (!item)
	{
		player->GetSession()->SendNotification("Please equip an item first.");
		return;
	}

	uint32 item_id = 0;
	switch (enchantid)
	{
	case ENCHANT_WEP_GREATER_SPELL_POWER:
	case ENCHANT_WEP_AGILITY_2H:
	case ENCHANT_WEP_MASSACRE:
		if (item->GetTemplate()->InventoryType != INVTYPE_2HWEAPON)
		{
			creature->Whisper("That isn't a two-handed weapon", LANG_UNIVERSAL, player, false);
			return;
		}
		break;
	case ENCHANT_SHIELD_DEFENSE:
	case ENCHANT_SHIELD_INTELLECT:
	case ENCHANT_SHIELD_MAJOR_STAMINA:
	case ENCHANT_SHIELD_RESILIENCE:
	case ENCHANT_SHIELD_TITANIUM_PLATING:
	case ENCHANT_SHIELD_TITANIUM_SPIKE:
	case ENCHANT_SHIELD_RESISTANCE:
	case ENCHANT_SHIELD_TOUGH_SHIELD:
	case ENCHANT_SHIELD_LESSER_PROTECTION:
	case ENCHANT_SHIELD_VITALITY:
		if (item->GetTemplate()->InventoryType != INVTYPE_SHIELD)
		{
			creature->Whisper("That isn't a shield", LANG_UNIVERSAL, player, false);
			return;
		}
		break;
	case ENCHANT_RANGED_HEARTSEEKER_SCOPE:
	case ENCHANT_RANGED_SUN_SCOPE:
	case ENCHANT_RANGED_BIZNICKS_SCOPE:
	case ENCHANT_RANGED_REFRACTOR_SCOPE:
		if (item->GetTemplate()->InventoryType != INVTYPE_RANGED &&
			item->GetTemplate()->InventoryType != INVTYPE_RANGEDRIGHT)
		{
			creature->Whisper("That isn't a ranged weapon", LANG_UNIVERSAL, player, false);
			return;
		}
		break;
	default:
		break;
	}

	player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, false);
	item->ClearEnchantment(PERM_ENCHANTMENT_SLOT);
	item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);
	player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, true);

	switch (enchantid)
	{
	case ENCHANT_BELT_ETERNAL_BELT_BUCKLE:
		EnchantWithItem(player, 41611, item);
		break;
	case ENCHANT_BRACER_SOCKET_BRACER:
		PlayerAddItem(player, 5956); // Blacksmith hammer
		EnchantWithSpell(player, 55628, item);
		player->DestroyItemCount(5956, -1, true, false);
		break;
	case ENCHANT_GLOVES_SOCKET_GLOVES:
		PlayerAddItem(player, 5956); // Blacksmith hammer
		EnchantWithSpell(player, 55641, item);
		player->DestroyItemCount(5956, -1, true, false);
		break;
	}

	const char* item_name = item->GetTemplate()->Name1.c_str();
	char* message;
	player->GetSession()->SendNotification("|cff0000FF%s |cffFF0000succesfully enchanted!", item->GetTemplate()->Name1.c_str());
}

static int selected_enchant;

class npc_enchantment : public CreatureScript
{
public:
	npc_enchantment() : CreatureScript("npc_enchantment") { }

	static bool OnGossipHello(Player* player, Creature* creature)
	{
		ShowMainMenu(player, creature);
		if (player->IsMounted()) // Checks if player is mounted
			player->Dismount(); // Dismounts the player (requires aura removal)
			player->RemoveAurasByType(SPELL_AURA_MOUNTED); // Removes any mount auras (requires dismount)
		return true;
	}

	void static ShowMainMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Weapon]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant 2H Weapon]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Ranged Weapon]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Shield]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Head]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Shoulders]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Cloak]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Chest]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Bracers]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Gloves]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Eternal Belt Buckle]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Legs]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[Enchant Feet]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
		//if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) >= 450)
			AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Enchant Rings", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static Show1HWeaponMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		if (player->getClass() == CLASS_DEATH_KNIGHT)
		{
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- +20% DMG next +2 ATKs that deal Frost or Shadow", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_CINDERGLACIER);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- +2% DMG as Fire or +4% VRS Undead Targets", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_LICHBANE);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- Heal self for +3% & +15% STR for 15 sec.", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_FALLEN_CRUSADER);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+2% DMG as Frost DMG & +Frost Vulnerability", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_RAZORICE);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+2% Deflection of all spell DMG & -50% Silence Duration", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_SPELLBREAKING);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+2% Parry & -50% Disarm Duration", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_SWORDBREAKING);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+13 Defense & +1% Stamina", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_NERUBIAN_CARAPACE);
		}
		if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) >= 450)
		{
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+200 Parry & +600 - 801 DMG on next parry & stacks x5 then depletes", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_BLADE_WARD);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "When striking with bleed attacks & fall below 35% health you restore 360 - 441 health & stacks x5 & lasts 20 sec.", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_BLOOD_DRAINING);
		}

		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- 250 Spell Haste", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_BLACK_MAGIC);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- Heal ally for 180 - 300", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_BATTLEMASTER);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- +400 Attack Power", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_BERSERKING);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- +120 Armor Pen", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_EXECUTIONER);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- Fire Damage 185 - 216", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_ICEBREAKER);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- Heal self for 310 to 357", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_LIFEWARD);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Proc -- +120 Agility", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_MONGOOSE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+65 Attack Power", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_POTENCY);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+63 Spell Power", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_MIGHTY_SPELL_POWER);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+26 Agility", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_AGILITY_1H);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+45 Spirit", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_SPIRIT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+50 Stamina", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_TITANGUARD);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "-50% Disarm Duration & +28 Hit", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_TITANIUM_CHAIN);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "-50 % Disarm Duration + 15 Parry", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_ADAMANTITE_CHAIN);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+25 Hit & +25 Crit", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_ACCURACY);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static Show2HWeaponMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		if (player->getClass() == CLASS_DEATH_KNIGHT)
		{
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+4% Deflection of all spell DMG & -50% Silence Duration", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_SPELLSHATTERING);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+4% Parry & -50% Disarm Duration", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_SWORDSHATTERING);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+25 Defense & +2% Stamina", GOSSIP_SENDER_MAIN,
				ENCHANT_WEP_STONESKIN_GARGOYLE);
		}
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+110 Attack Power", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_MASSACRE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+81 Spell Power", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_GREATER_SPELL_POWER);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+35 Agility", GOSSIP_SENDER_MAIN,
			ENCHANT_WEP_AGILITY_2H);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowShieldMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+20 Defense", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_DEFENSE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+25 Intellect", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_INTELLECT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+18 Stamina", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_MAJOR_STAMINA);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+12 Resilience", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_RESILIENCE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+81 Block & -50% Disarm Duration", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_TITANIUM_PLATING);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+5 Resist All", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_RESISTANCE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+36 Block", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_TOUGH_SHIELD);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+30 Armor", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_LESSER_PROTECTION);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+4 Health & +4 MP5", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_VITALITY);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Each Block Deals 45 - 68 damage", GOSSIP_SENDER_MAIN,
			ENCHANT_SHIELD_TITANIUM_SPIKE);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowHeadMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+50 Attack Power & +20 Crit", GOSSIP_SENDER_MAIN,
			ENCHANT_HEAD_TORMENT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+50 Attack Power & +20 Resil", GOSSIP_SENDER_MAIN,
			ENCHANT_HEAD_TRIUMPH);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+30 Spell Power & +10 Mp5", GOSSIP_SENDER_MAIN,
			ENCHANT_HEAD_BLISSFUL_MENDING);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+30 Spell Power & +20 Crit", GOSSIP_SENDER_MAIN,
			ENCHANT_HEAD_BURNING_MYSTERIES);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+29 Spell Power & +20 Resil", GOSSIP_SENDER_MAIN,
			ENCHANT_HEAD_DOMINANCE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+30 Stamina & +25 Resil", GOSSIP_SENDER_MAIN,
			ENCHANT_HEAD_SAVAGE_GLADIATOR);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+37 Stamina & +20 Defense", GOSSIP_SENDER_MAIN,
			ENCHANT_HEAD_STALWART_PROTECTOR);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowShouldersMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) >= 450)
		{
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Master's Inscription of the Axe", GOSSIP_SENDER_MAIN,
				ENCHANT_SHOULDER_MASTERS_AXE);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Master's Inscription of the Crag", GOSSIP_SENDER_MAIN,
				ENCHANT_SHOULDER_MASTERS_CRAG);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Master's Inscription of the Pinnacle", GOSSIP_SENDER_MAIN,
				ENCHANT_SHOULDER_MASTERS_PINNACLE);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Master's Inscription of the Storm", GOSSIP_SENDER_MAIN,
				ENCHANT_SHOULDER_MASTERS_STORM);
		}
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+24 Spell Power & +8 Mp5", GOSSIP_SENDER_MAIN,
			ENCHANT_SHOULDER_GREATER_CRAG);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+40 Attack Power & +15 Crit", GOSSIP_SENDER_MAIN,
			ENCHANT_SHOULDER_GREATER_AXE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+40 Attack Power & +15 Resil", GOSSIP_SENDER_MAIN,
			ENCHANT_SHOULDER_TRIUMPH);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+24 Spell Power & +15 Crit", GOSSIP_SENDER_MAIN,
			ENCHANT_SHOULDER_GREATER_STORM);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+23 Spell Power & +15 Resil", GOSSIP_SENDER_MAIN,
			ENCHANT_SHOULDER_DOMINANCE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+20 Dodge & +15 Defense", GOSSIP_SENDER_MAIN,
			ENCHANT_SHOULDER_GREATER_PINNACLE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+30 Stamina & +15 Resil", GOSSIP_SENDER_MAIN,
			ENCHANT_SHOULDER_GREATER_GLADIATOR);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowCloakMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) >= 450)
		{
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Springy Arachnoweave", GOSSIP_SENDER_MAIN,
				ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Flexweave Underlay", GOSSIP_SENDER_MAIN,
				ENCHANT_CLOAK_FLEXWEAVE_UNDERLAY);
		}
		//if (player->getClass() == CLASS_ROGUE)
			//AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+Stealth & +10 Agility", GOSSIP_SENDER_MAIN,
				//ENCHANT_CLOAK_SHADOW_ARMOR);
		if (player->HasSkill(SKILL_TAILORING) && player->GetSkillValue(SKILL_TAILORING) >= 450)
		{
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Darkglow Embroidery", GOSSIP_SENDER_MAIN,
				ENCHANT_CLOAK_DARKGLOW_EMBROIDERY);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Lightweave Embroidery", GOSSIP_SENDER_MAIN,
				ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Swordguard Embroidery", GOSSIP_SENDER_MAIN,
				ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY);
		}
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+35 Spell Pen", GOSSIP_SENDER_MAIN,
			ENCHANT_CLOAK_SPELL_PIERCING);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+225 Armor", GOSSIP_SENDER_MAIN,
			ENCHANT_CLOAK_MIGHTY_ARMOR);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+22 Agility", GOSSIP_SENDER_MAIN,
			ENCHANT_CLOAK_MAJOR_AGILITY);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+23 Haste", GOSSIP_SENDER_MAIN,
			ENCHANT_CLOAK_GREATER_SPEED);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+10 Spirit & +2% reduce threat", GOSSIP_SENDER_MAIN,
			ENCHANT_CLOAK_WISDOM);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+16 Defense", GOSSIP_SENDER_MAIN,
			ENCHANT_CLOAK_TITANWEAVE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+12 Dodge", GOSSIP_SENDER_MAIN,
			ENCHANT_CLOAK_DODGE);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowChestMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+10 all stats", GOSSIP_SENDER_MAIN,
			ENCHANT_CHEST_ALL_STATS);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+225 Health", GOSSIP_SENDER_MAIN,
			ENCHANT_CHEST_SUPER_HEALTH);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+150 Mana", GOSSIP_SENDER_MAIN,
			ENCHANT_CHEST_EXCEPTIONAL_MANA);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+10 Mp5", GOSSIP_SENDER_MAIN,
			ENCHANT_CHEST_GREATER_MANA_RESTO);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+15 Spirit", GOSSIP_SENDER_MAIN,
			ENCHANT_CHEST_MAJOR_SPIRIT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+20 Resilience", GOSSIP_SENDER_MAIN,
			ENCHANT_CHEST_EXCEPTIONAL_RESILIENCE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+22 Defense", GOSSIP_SENDER_MAIN,
			ENCHANT_CHEST_GREATER_DEF);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "5% on hit for self +25 DMG Absorb Shield", GOSSIP_SENDER_MAIN,
			ENCHANT_CHEST_LESSER_ABSORP);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "25% on hit for self +400 DMG Absorb Shield. Lasts for 1 hour & 90 sec CD.", GOSSIP_SENDER_MAIN,
			ENCHANT_CHEST_RUNE_OF_WARDING);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowBracerMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+50 Attack Power", GOSSIP_SENDER_MAIN,
			ENCHANT_BRACER_GREATER_ASSAULT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+30 Spell Power", GOSSIP_SENDER_MAIN,
			ENCHANT_BRACER_SUPERIOR_SPELLPOWER);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+16 Intellect", GOSSIP_SENDER_MAIN,
			ENCHANT_BRACER_EXCEPTIONAL_INTELLECT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+18 Spirit", GOSSIP_SENDER_MAIN,
			ENCHANT_BRACER_MAJOR_SPIRIT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+8 MP5", GOSSIP_SENDER_MAIN,
			ENCHANT_BRACER_RESTORE_MANA);
		// AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+15 Expertise", GOSSIP_SENDER_MAIN,
		//	ENCHANT_BRACER_EXPERTISE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+6 all stats", GOSSIP_SENDER_MAIN,
			ENCHANT_BRACER_GREATER_STATS);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+12 Strength", GOSSIP_SENDER_MAIN,
			ENCHANT_BRACER_BRAWN);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+16 Defense", GOSSIP_SENDER_MAIN,
			ENCHANT_BRACER_MAJOR_DEFENSE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+40 Stamina", GOSSIP_SENDER_MAIN,
			ENCHANT_BRACER_MAJOR_STAMINA);
		if (player->HasSkill(SKILL_LEATHERWORKING) && player->GetSkillValue(SKILL_LEATHERWORKING) >= 450)
		{
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Fur Lining: Attack Power", GOSSIP_SENDER_MAIN,
				ENCHANT_BRACER_FUR_LINING_AP);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Fur Lining: Stamina", GOSSIP_SENDER_MAIN,
				ENCHANT_BRACER_FUR_LINING_STAMINA);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Fur Lining: Spell Power", GOSSIP_SENDER_MAIN,
				ENCHANT_BRACER_FUR_LINING_SP);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Fur Lining: Arcane Resist", GOSSIP_SENDER_MAIN,
				ENCHANT_BRACER_ARCANE_RESIST);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Fur Lining: Nature Resist", GOSSIP_SENDER_MAIN,
				ENCHANT_BRACER_NATURE_RESIST);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Fur Lining: Shadow Resist", GOSSIP_SENDER_MAIN,
				ENCHANT_BRACER_SHADOW_RESIST);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Fur Lining: Frost Resist", GOSSIP_SENDER_MAIN,
				ENCHANT_BRACER_FROST_RESIST);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Fur Lining: Fire Resist", GOSSIP_SENDER_MAIN,
				ENCHANT_BRACER_FIRE_RESIST);
		}
		//if (player->HasSkill(SKILL_BLACKSMITHING) && player->GetSkillValue(SKILL_BLACKSMITHING) >= 450)
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Socket Bracer", GOSSIP_SENDER_MAIN,
				ENCHANT_BRACER_SOCKET_BRACER);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowGlovesMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+44 Attack Power", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_CRUSHER);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+28 Spell Power", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_EXCEPTIONAL_SPELLPOWER);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+15 Strength", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_MAJOR_STR);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+20 Agility", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_MAJOR_AGILITY);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+16 Crit", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_GREATER_BLASTING);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+10 Haste", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_MINOR_HASTE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+20 Hit Rating", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_PRECISION);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+15 Expertise", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_EXPERTISE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+10 Parry & +2% Threat", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_ARMSMAN);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+2% Mount Speed", GOSSIP_SENDER_MAIN,
			ENCHANT_GLOVES_RIDING);
		//if (player->HasSkill(SKILL_BLACKSMITHING) && player->GetSkillValue(SKILL_BLACKSMITHING) >= 450)
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Socket Gloves", GOSSIP_SENDER_MAIN,
				ENCHANT_GLOVES_SOCKET_GLOVES);
		//if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) >= 450)
		//{
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Hand-Mounted Pyro Rocket", GOSSIP_SENDER_MAIN,
				ENCHANT_GLOVES_PYROROCKET);
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Hyperspeed Accelerators", GOSSIP_SENDER_MAIN,
				ENCHANT_GLOVES_HYPERSPEED_ACCELERATORS);
		//}
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowBeltMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Eternal Belt Buckle", GOSSIP_SENDER_MAIN,
			ENCHANT_BELT_ETERNAL_BELT_BUCKLE);
		if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) >= 450)
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Frag Belt", GOSSIP_SENDER_MAIN,
				ENCHANT_BELT_FRAG_BELT);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		if (player->IsMounted()) // Checks if player is mounted
			player->Dismount(); // Dismounts the player (requires aura removal)
			player->RemoveAurasByType(SPELL_AURA_MOUNTED); // Removes any mount auras (requires dismount)
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowLegsMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+75 Attack Power & +22 Crit", GOSSIP_SENDER_MAIN,
			ENCHANT_LEGS_ICESCALE_ARMOR);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+50 Spell Power & +30 Stamina", GOSSIP_SENDER_MAIN,
			ENCHANT_LEGS_SAPPHIRE_SPELLTHREAD);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+50 Spell Power & +22 Spirit", GOSSIP_SENDER_MAIN,
			ENCHANT_LEGS_BRILLIANT_SPELLTHREAD);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+55 Stamina & +22 Agility", GOSSIP_SENDER_MAIN,
			ENCHANT_LEGS_FROSTHIDE_ARMOR);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+28 Stamina & +40 Resil", GOSSIP_SENDER_MAIN,
			ENCHANT_LEGS_EARTHEN_ARMOR);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowBootsMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+32 Attack Power", GOSSIP_SENDER_MAIN,
			ENCHANT_BOOTS_GREATER_ASSAULT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+18 Spirit", GOSSIP_SENDER_MAIN,
			ENCHANT_BOOTS_GREATER_SPIRIT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+7 Health & +7 MP5", GOSSIP_SENDER_MAIN,
			ENCHANT_BOOTS_GREATER_VITALITY);
		//if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) >= 450)
			AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Nitro Boosts", GOSSIP_SENDER_MAIN,
				ENCHANT_BOOTS_NITRO_BOOSTS);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+16 Agility", GOSSIP_SENDER_MAIN,
			ENCHANT_BOOTS_SUPERIOR_AGILITY);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+22 Staminas", GOSSIP_SENDER_MAIN,
			ENCHANT_BOOTS_GREATER_FORTITUDE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+15 Stamina & +8% Movement", GOSSIP_SENDER_MAIN,
			ENCHANT_BOOTS_TUSKARRS_VITALITY);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+6 Agility & +8% Movement", GOSSIP_SENDER_MAIN,
			ENCHANT_BOOTS_CATS_SWIFTNESS);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+12 Hit & +12 Crit", GOSSIP_SENDER_MAIN,
			ENCHANT_BOOTS_ICEWALKER);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowRingsMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+40 Attack Power", GOSSIP_SENDER_MAIN,
			ENCHANT_RING_ASSAULT);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+23 Spell Power", GOSSIP_SENDER_MAIN,
			ENCHANT_RING_GREATER_SPELLPOWER);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+30 Stamina", GOSSIP_SENDER_MAIN,
			ENCHANT_RING_STAMINA);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	void static ShowRangedMenu(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+40 Ranged Crit", GOSSIP_SENDER_MAIN,
			ENCHANT_RANGED_HEARTSEEKER_SCOPE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+40 Ranged Haste", GOSSIP_SENDER_MAIN,
			ENCHANT_RANGED_SUN_SCOPE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+30 Ranged Hit", GOSSIP_SENDER_MAIN,
			ENCHANT_RANGED_BIZNICKS_SCOPE);
		AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "+15 Ranged Damage", GOSSIP_SENDER_MAIN,
			ENCHANT_RANGED_REFRACTOR_SCOPE);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, -1);
		player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
	}

	static bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		int slot = -1;
		bool check_adjacent_slot = true;
		switch (action)
		{
		case -1: // Back
			ShowMainMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 1:
			Show1HWeaponMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 2:
			Show2HWeaponMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 3:
			ShowShieldMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 4:
			ShowHeadMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 5:
			ShowShouldersMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 6:
			ShowCloakMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 7:
			ShowChestMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 8:
			ShowBracerMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 9:
			ShowGlovesMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 10:
			ShowBeltMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 11:
			ShowLegsMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 12:
			ShowBootsMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 13:
			ShowRangedMenu(player, creature);
			break;
		case GOSSIP_ACTION_INFO_DEF + 14:
			ShowRingsMenu(player, creature);
			break;

			// 1H weapons
		case ENCHANT_WEP_BLADE_WARD:
		case ENCHANT_WEP_BLOOD_DRAINING:
		case ENCHANT_WEP_AGILITY_1H:
		case ENCHANT_WEP_SPIRIT:
		case ENCHANT_WEP_BERSERKING:
		case ENCHANT_WEP_ACCURACY:
		case ENCHANT_WEP_BLACK_MAGIC:
		case ENCHANT_WEP_BATTLEMASTER:
		case ENCHANT_WEP_ICEBREAKER:
		case ENCHANT_WEP_LIFEWARD:
		case ENCHANT_WEP_TITANGUARD:
		case ENCHANT_WEP_POTENCY:
		case ENCHANT_WEP_MONGOOSE:
		case ENCHANT_WEP_MIGHTY_SPELL_POWER:
		case ENCHANT_WEP_EXECUTIONER:
		case ENCHANT_WEP_TITANIUM_CHAIN:
		case ENCHANT_WEP_ADAMANTITE_CHAIN:

			// 2H weapons
			// case ENCHANT_WEP_BERSERKING:
			// case ENCHANT_WEP_MONGOOSE:
			// case ENCHANT_WEP_EXECUTIONER:
			// case ENCHANT_WEP_TITANIUM_CHAIN:
		case ENCHANT_WEP_GREATER_SPELL_POWER:
		case ENCHANT_WEP_AGILITY_2H:
		case ENCHANT_WEP_MASSACRE:

			// Runeforging
		case ENCHANT_WEP_CINDERGLACIER:
		case ENCHANT_WEP_LICHBANE:
		case ENCHANT_WEP_RAZORICE:
		case ENCHANT_WEP_SPELLBREAKING:
		case ENCHANT_WEP_SPELLSHATTERING:
		case ENCHANT_WEP_SWORDBREAKING:
		case ENCHANT_WEP_SWORDSHATTERING:
		case ENCHANT_WEP_FALLEN_CRUSADER:
		case ENCHANT_WEP_NERUBIAN_CARAPACE:
		case ENCHANT_WEP_STONESKIN_GARGOYLE:
			slot = EQUIPMENT_SLOT_MAINHAND;
			break;

			// Shields
		case ENCHANT_SHIELD_DEFENSE:
		case ENCHANT_SHIELD_INTELLECT:
		case ENCHANT_SHIELD_RESILIENCE:
		case ENCHANT_SHIELD_TITANIUM_PLATING:
		case ENCHANT_SHIELD_MAJOR_STAMINA:
		case ENCHANT_SHIELD_TITANIUM_SPIKE:
		case ENCHANT_SHIELD_RESISTANCE:
		case ENCHANT_SHIELD_TOUGH_SHIELD:
		case ENCHANT_SHIELD_LESSER_PROTECTION:
		case ENCHANT_SHIELD_VITALITY:
			slot = EQUIPMENT_SLOT_OFFHAND;
			break;

			// Head
		case ENCHANT_HEAD_BLISSFUL_MENDING:
		case ENCHANT_HEAD_BURNING_MYSTERIES:
		case ENCHANT_HEAD_DOMINANCE:
		case ENCHANT_HEAD_SAVAGE_GLADIATOR:
		case ENCHANT_HEAD_STALWART_PROTECTOR:
		case ENCHANT_HEAD_TORMENT:
		case ENCHANT_HEAD_TRIUMPH:
			slot = EQUIPMENT_SLOT_HEAD;
			break;

			// Shoulders
		case ENCHANT_SHOULDER_MASTERS_AXE:
		case ENCHANT_SHOULDER_MASTERS_CRAG:
		case ENCHANT_SHOULDER_MASTERS_PINNACLE:
		case ENCHANT_SHOULDER_MASTERS_STORM:
		case ENCHANT_SHOULDER_GREATER_AXE:
		case ENCHANT_SHOULDER_GREATER_CRAG:
		case ENCHANT_SHOULDER_GREATER_GLADIATOR:
		case ENCHANT_SHOULDER_GREATER_PINNACLE:
		case ENCHANT_SHOULDER_GREATER_STORM:
		case ENCHANT_SHOULDER_DOMINANCE:
		case ENCHANT_SHOULDER_TRIUMPH:
			slot = EQUIPMENT_SLOT_SHOULDERS;
			break;

			// Cloak
		case ENCHANT_CLOAK_DARKGLOW_EMBROIDERY:
		case ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY:
		case ENCHANT_CLOAK_FLEXWEAVE_UNDERLAY:
		case ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY:
		case ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE:
		case ENCHANT_CLOAK_WISDOM:
		case ENCHANT_CLOAK_TITANWEAVE:
		case ENCHANT_CLOAK_SPELL_PIERCING:
		//case ENCHANT_CLOAK_SHADOW_ARMOR:
		case ENCHANT_CLOAK_MIGHTY_ARMOR:
		case ENCHANT_CLOAK_MAJOR_AGILITY:
		case ENCHANT_CLOAK_GREATER_SPEED:
		case ENCHANT_CLOAK_DODGE:
			slot = EQUIPMENT_SLOT_BACK;
			break;

			// Chest
		case ENCHANT_CHEST_EXCEPTIONAL_MANA:
		case ENCHANT_CHEST_GREATER_MANA_RESTO:
		case ENCHANT_CHEST_EXCEPTIONAL_RESILIENCE:
		case ENCHANT_CHEST_SUPER_HEALTH:
		case ENCHANT_CHEST_ALL_STATS:
		case ENCHANT_CHEST_GREATER_DEF:
		case ENCHANT_CHEST_MAJOR_SPIRIT:
		case ENCHANT_CHEST_LESSER_ABSORP:
		case ENCHANT_CHEST_RUNE_OF_WARDING:
			slot = EQUIPMENT_SLOT_CHEST;
			break;

				// Bracers
		case ENCHANT_BRACER_GREATER_ASSAULT:
		case ENCHANT_BRACER_EXCEPTIONAL_INTELLECT:
		case ENCHANT_BRACER_MAJOR_SPIRIT:
			// case ENCHANT_BRACER_EXPERTISE:
		case ENCHANT_BRACER_GREATER_STATS:
		case ENCHANT_BRACER_SUPERIOR_SPELLPOWER:
		case ENCHANT_BRACER_MAJOR_STAMINA:
		case ENCHANT_BRACER_FUR_LINING_AP:
		case ENCHANT_BRACER_FUR_LINING_STAMINA:
		case ENCHANT_BRACER_FUR_LINING_SP:
		case ENCHANT_BRACER_ARCANE_RESIST:
		case ENCHANT_BRACER_NATURE_RESIST:
		case ENCHANT_BRACER_SHADOW_RESIST:
		case ENCHANT_BRACER_FROST_RESIST:
		case ENCHANT_BRACER_FIRE_RESIST:
		case ENCHANT_BRACER_SOCKET_BRACER:
		case ENCHANT_BRACER_BRAWN:
		case ENCHANT_BRACER_RESTORE_MANA:
		case ENCHANT_BRACER_MAJOR_DEFENSE:
			slot = EQUIPMENT_SLOT_WRISTS;
			break;

			// Gloves
		case ENCHANT_GLOVES_EXPERTISE:
		case ENCHANT_GLOVES_PRECISION:
		case ENCHANT_GLOVES_GREATER_ASSAULT:
		case ENCHANT_GLOVES_MAJOR_AGILITY:
		case ENCHANT_GLOVES_EXCEPTIONAL_SPELLPOWER:
		case ENCHANT_GLOVES_ARMSMAN:
		case ENCHANT_GLOVES_SOCKET_GLOVES:
		case ENCHANT_GLOVES_PYROROCKET:
		case ENCHANT_GLOVES_HYPERSPEED_ACCELERATORS:
		case ENCHANT_GLOVES_MAJOR_STR:
		case ENCHANT_GLOVES_MINOR_HASTE:
		case ENCHANT_GLOVES_GREATER_BLASTING:
		case ENCHANT_GLOVES_RIDING:
			slot = EQUIPMENT_SLOT_HANDS;
			break;

			// Belt
		case ENCHANT_BELT_ETERNAL_BELT_BUCKLE:
		case ENCHANT_BELT_FRAG_BELT:
			slot = EQUIPMENT_SLOT_WAIST;
			break;

			// Legs
		case ENCHANT_LEGS_FROSTHIDE_ARMOR:
		case ENCHANT_LEGS_ICESCALE_ARMOR:
		case ENCHANT_LEGS_EARTHEN_ARMOR:
		case ENCHANT_LEGS_SAPPHIRE_SPELLTHREAD:
		case ENCHANT_LEGS_BRILLIANT_SPELLTHREAD:
			slot = EQUIPMENT_SLOT_LEGS;
			break;

			// Boots
		case ENCHANT_BOOTS_GREATER_VITALITY:
		case ENCHANT_BOOTS_TUSKARRS_VITALITY:
		case ENCHANT_BOOTS_ICEWALKER:
		case ENCHANT_BOOTS_GREATER_ASSAULT:
		case ENCHANT_BOOTS_GREATER_SPIRIT:
		case ENCHANT_BOOTS_GREATER_FORTITUDE:
		case ENCHANT_BOOTS_SUPERIOR_AGILITY:
		case ENCHANT_BOOTS_NITRO_BOOSTS:
			slot = EQUIPMENT_SLOT_FEET;
			break;

			// Rings
		case ENCHANT_RING_STAMINA:
		case ENCHANT_RING_GREATER_SPELLPOWER:
		case ENCHANT_RING_ASSAULT:
			slot = EQUIPMENT_SLOT_FINGER1;
			break;

			// Ranged weapon
		case ENCHANT_RANGED_HEARTSEEKER_SCOPE:
		case ENCHANT_RANGED_SUN_SCOPE:
		case ENCHANT_RANGED_BIZNICKS_SCOPE:
		case ENCHANT_RANGED_REFRACTOR_SCOPE:
			slot = EQUIPMENT_SLOT_RANGED;
			break;

			// Main hand
		case GOSSIP_ACTION_INFO_DEF + 20:
			slot = EQUIPMENT_SLOT_MAINHAND;
			action = selected_enchant;
			check_adjacent_slot = false;
			break;

			// Off hand
		case GOSSIP_ACTION_INFO_DEF + 21:
			slot = EQUIPMENT_SLOT_OFFHAND;
			action = selected_enchant;
			check_adjacent_slot = false;
			break;

		default:
			CloseGossipMenuFor(player);
			break;
		}
		if (slot > -1)
		{
			// allows for a maximum of 2 duplicates, and the
			// convenience of using enchant ids as gossip 
			// userdata
			if (action < 0)
				action = -action;
			Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
			if (!item && slot == INVTYPE_RANGEDRIGHT)
				item = player->GetItemByPos(INVENTORY_SLOT_BAG_0,
					INVTYPE_RANGEDRIGHT);
			if (item && check_adjacent_slot &&
				item->GetEnchantmentId(PERM_ENCHANTMENT_SLOT) != 0)
			{
				switch (slot)
				{
				case EQUIPMENT_SLOT_MAINHAND:
				{
					Item* offhand = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
					if (offhand)
					{
						player->PlayerTalkClass->ClearMenus();
						AddGossipItemFor(player, GOSSIP_ICON_CHAT,
							item->GetTemplate()->Name1.c_str(),
							GOSSIP_SENDER_MAIN,
							GOSSIP_ACTION_INFO_DEF + 20);
						AddGossipItemFor(player, GOSSIP_ICON_CHAT,
							offhand->GetTemplate()->Name1.c_str(),
							GOSSIP_SENDER_MAIN,
							GOSSIP_ACTION_INFO_DEF + 21);
						AddGossipItemFor(player, GOSSIP_ICON_CHAT,
							"Back",
							GOSSIP_SENDER_MAIN, -1);
						player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
						selected_enchant = action;
						return true;
					}
					break;
				}
				case EQUIPMENT_SLOT_FINGER1:
				{
					Item* ring2 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2);
					if (ring2)
					{
						player->PlayerTalkClass->ClearMenus();
						AddGossipItemFor(player, GOSSIP_ICON_CHAT,
							item->GetTemplate()->Name1.c_str(),
							GOSSIP_SENDER_MAIN,
							GOSSIP_ACTION_INFO_DEF + 20);
						AddGossipItemFor(player, GOSSIP_ICON_CHAT,
							ring2->GetTemplate()->Name1.c_str(),
							GOSSIP_SENDER_MAIN,
							GOSSIP_ACTION_INFO_DEF + 21);
						AddGossipItemFor(player, GOSSIP_ICON_CHAT,
							"Back",
							GOSSIP_SENDER_MAIN, -1);
						player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
						selected_enchant = action;
						return true;
					}
					break;
				}
				default:
					break;
				}
			}
			Enchant(player, creature, item, action);
			ShowMainMenu(player, creature);
		}
		return true;
	}
	static bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code)
	{
		player->PlayerTalkClass->ClearMenus();
		// code
		return true;
	}
	struct MyAI : public ScriptedAI
	{
		MyAI(Creature* creature) : ScriptedAI(creature) { }
		bool GossipHello(Player* player) override
		{
			return OnGossipHello(player, me);
		}
		bool GossipSelect(Player* player, uint32 menuId, uint32 gossipListId) override
		{
			uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
			uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
			return OnGossipSelect(player, me, sender, action);
		}
		bool GossipSelectCode(Player* player, uint32 menuId, uint32 gossipListId, char const* code) override
		{
			uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
			uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
			return OnGossipSelectCode(player, me, sender, action, code);
		}
	};
	CreatureAI* GetAI(Creature* creature) const override
	{
		return new MyAI(creature);
	}
};

void AddSC_npc_enchantment()
{
	new npc_enchantment;
}