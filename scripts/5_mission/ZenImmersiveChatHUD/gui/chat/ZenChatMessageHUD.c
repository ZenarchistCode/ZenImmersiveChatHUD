class ZenChatMessageHUD extends Managed
{
	static const string LAYOUT_ZENIMMERSIVECHATHUD = "ZenImmersiveChatHUD/data/layouts/chathud.layout";

	private EntityAI m_RootEntity;
	private ref Widget m_RootWidget;

	private ref MultilineTextWidget m_TextWidget;
	private ref MultilineTextWidget m_TextWidgetShadow;

	private ref MultilineTextWidget m_TextWidgetWhisper;
	private ref MultilineTextWidget m_TextWidgetWhisperShadow;

	private int m_VoiceLevel;
	private int m_DurationSecs;
	private int m_Timestamp;

	private float m_MasterWidthPixels;
	private float m_MasterHeightPixels;

	void ZenChatMessageHUD(EntityAI p_obj, ZenChatMessage p_msg)
	{
		m_RootEntity = p_obj;
		m_RootWidget = GetGame().GetWorkspace().CreateWidgets(LAYOUT_ZENIMMERSIVECHATHUD);
		m_TextWidget = MultilineTextWidget.Cast(m_RootWidget.FindAnyWidget("ChatTextWidget"));
		m_TextWidgetShadow = MultilineTextWidget.Cast(m_RootWidget.FindAnyWidget("ChatTextWidget2"));
		m_TextWidgetWhisper = MultilineTextWidget.Cast(m_RootWidget.FindAnyWidget("ChatTextWidgetWhisper"));
		m_TextWidgetWhisperShadow = MultilineTextWidget.Cast(m_RootWidget.FindAnyWidget("ChatTextWidgetWhisper2"));

		SetText(p_msg.GetText(), p_msg.GetVoiceLevel());
		m_RootWidget.GetSize(m_MasterWidthPixels, m_MasterHeightPixels);

		UpdateTimestamp();
		UpdateScreenPos();
	}

	void SetText(string p_text, int shoutLevel)
	{
		m_VoiceLevel = shoutLevel;
		int color = -1;
		int modifier = 1;

		if (m_VoiceLevel == 0) // Whisper
		{
			p_text.ToLower();
			color = -8862465;
		}
		else if (m_VoiceLevel == 1) // Normal
		{
			if (p_text.Length() > 0)
			{
				p_text.ToLower();
				string firstChar = p_text.Substring(0, 1);
				firstChar.ToUpper();
				p_text = firstChar + p_text.Substring(1, p_text.Length() - 1);
			}
		}
		else if (m_VoiceLevel == 2) // Shout
		{
			p_text.ToUpper();
			p_text = p_text + "!";
			color = -52686;
			modifier = 3;
		}

		m_TextWidgetWhisper.SetColor(color);
		m_TextWidget.SetColor(color);

		m_TextWidget.SetText(p_text);
		m_TextWidgetShadow.SetText(p_text);
		m_TextWidgetWhisper.SetText(p_text);
		m_TextWidgetWhisperShadow.SetText(p_text);

		m_TextWidgetWhisper.Show(m_VoiceLevel == 0);
		m_TextWidgetWhisperShadow.Show(m_VoiceLevel == 0);
		m_TextWidget.Show(m_VoiceLevel != 0);
		m_TextWidgetShadow.Show(m_VoiceLevel != 0);

		UpdateTimestamp();

		m_DurationSecs = Math.Clamp(p_text.Length(), 3, 10) * modifier * 1000;
	}

	void UpdateTimestamp()
	{
		m_Timestamp = GetGame().GetTime();
	}

	void UpdateScreenPos()
	{
		vector position = m_RootEntity.GetPosition();
		vector head_pos = GetGame().GetCurrentCameraPosition();
		float distance = vector.Distance(head_pos, position);

		if (m_RootEntity.IsInherited(SurvivorBase))
		{
			position = m_RootEntity.GetBonePositionWS(m_RootEntity.GetBoneIndex("Head"));
			position[1] = position[1] + 0.3;
		}

		vector screenPositionRelative = GetGame().GetScreenPosRelative(position);

		int maxDistance = 25;
		if (m_VoiceLevel == 0)
		{
			maxDistance = 7;
		}
		else if (m_VoiceLevel == 2)
		{
			maxDistance = 45;
		}

		if (distance >= maxDistance || screenPositionRelative[0] >= 1 || screenPositionRelative[0] == 0 || screenPositionRelative[1] >= 1 || screenPositionRelative[1] == 0 || screenPositionRelative[2] < 0)
		{
			m_RootWidget.Show(false);
			return;
		}

		int sx, sy;
		GetScreenSize(sx, sy);

		float x, y;
		vector screenPosition = GetGame().GetScreenPos(position);

		float sizeMultiplier = 1.0;

		if (distance < 10)
		{
			sizeMultiplier = 1.0;
		}
		else if (distance < 20)
		{
			sizeMultiplier = 0.6;
		}
		else
		{
			sizeMultiplier = 0.3;
		}

		m_RootWidget.SetSize(m_MasterWidthPixels * sizeMultiplier, m_MasterHeightPixels * sizeMultiplier);

		// Get widget size in pixels
		float widgetWidth, widgetHeight;
		m_RootWidget.GetSize(widgetWidth, widgetHeight);

		x = Math.Round(screenPosition[0] - (widgetWidth / 2));
		y = Math.Round(screenPosition[1]);

		m_RootWidget.SetPos(x, sy - y);
		m_RootWidget.Show(true);
	}

	float SmoothStep(float t)
	{
		return t * t * (3 - 2 * t);
	}

	bool Update()
	{
		if (!m_RootEntity)
		{
			return false;
		}

		UpdateScreenPos();

		// check timestamp & fade, once faded return false to delete
		int durationMs = GetGame().GetTime() - m_Timestamp;

		if (GetGame().GetTime() - m_Timestamp >= m_DurationSecs)
		{
			m_RootWidget.Show(false);
			return false;
		}

		return true;
	}
}