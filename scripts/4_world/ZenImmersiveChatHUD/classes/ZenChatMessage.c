class ZenChatMessage extends Managed
{
	string m_MessageText;
	int m_VoiceLevel;

	void ZenChatMessage(string p_msg, int p_lvl)
	{
		m_MessageText = p_msg;
		m_VoiceLevel = p_lvl;
	}

	string GetText()
	{
		return m_MessageText;
	}

	int GetVoiceLevel()
	{
		return m_VoiceLevel;
	}
}