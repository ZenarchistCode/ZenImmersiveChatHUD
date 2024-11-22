modded class PlayerBase
{
	string GetZenChatName()
	{
		return ""; // Override this for mods like Syberia (eg. return m_charProfile.m_name)
	}

	bool ZenChat_IsVoiceObstructed()
	{
		// Check for mouth gag or helmet which obstructs voice
		Clothing item;
		Class.CastTo(item, GetInventory().FindAttachment(InventorySlots.MASK));
		if (!item)
		{
			Class.CastTo(item, GetInventory().FindAttachment(InventorySlots.HEADGEAR));
		}

		return item && item.IsObstructingVoice();
	}
}