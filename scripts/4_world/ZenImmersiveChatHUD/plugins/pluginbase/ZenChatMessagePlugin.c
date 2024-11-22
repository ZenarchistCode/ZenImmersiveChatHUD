//! CLIENT-SIDE CHAT HUD QUEUE
class ZenChatMessagePlugin extends PluginBase
{
    protected ref map<PlayerBase, ref ZenChatMessage> m_PlayerChat;

	override void OnInit()
	{
		Print("[ZenChatMessagePlugin] :: OnInit");

		m_PlayerChat = new map<PlayerBase, ref ZenChatMessage>;
	}

	void AddChatMessage(PlayerBase p_pb, string p_msg, int p_lvl)
	{
		m_PlayerChat.Set(p_pb, new ZenChatMessage(p_msg, p_lvl));
	}

	map<PlayerBase, ref ZenChatMessage> GetPlayerChatMessages()
	{
		return m_PlayerChat;
	}
}