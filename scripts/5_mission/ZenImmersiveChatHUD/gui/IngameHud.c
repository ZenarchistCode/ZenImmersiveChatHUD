modded class IngameHud
{
	protected ref map<EntityAI, ref ZenChatMessageHUD> m_ZenChatWidgets;
	protected ref ZenChatMessagePlugin m_ZenChatMessagePlugin;

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		m_ZenChatMessagePlugin = ZenChatMessagePlugin.Cast(GetPlugin(ZenChatMessagePlugin));
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		if (!m_ZenChatMessagePlugin)
			return;

		int i;
		for (i = m_ZenChatMessagePlugin.GetPlayerChatMessages().Count() - 1; i >= 0; i--)
		{
			CreateZenPlayerChatHUD(m_ZenChatMessagePlugin.GetPlayerChatMessages().GetKey(i), m_ZenChatMessagePlugin.GetPlayerChatMessages().GetElement(i));
			m_ZenChatMessagePlugin.GetPlayerChatMessages().RemoveElement(i);
		}

		if (!m_ZenChatWidgets)
			return;

		for (i = m_ZenChatWidgets.Count() - 1; i >= 0; i--)
		{
			if (!m_ZenChatWidgets.GetElement(i).Update())
			{
				m_ZenChatWidgets.RemoveElement(i);
			}
		}
	};

	void CreateZenPlayerChatHUD(EntityAI player, ZenChatMessage msg)
	{
		if (!m_ZenChatWidgets)
			m_ZenChatWidgets = new map<EntityAI, ref ZenChatMessageHUD>;

		ZenChatMessageHUD chatMsg;
		m_ZenChatWidgets.Find(player, chatMsg);

		if (chatMsg)
		{
			chatMsg.SetText(msg.GetText(), msg.GetVoiceLevel());
			return;
		}

		chatMsg = new ZenChatMessageHUD(player, msg);
		m_ZenChatWidgets.Set(player, chatMsg);
	}
}