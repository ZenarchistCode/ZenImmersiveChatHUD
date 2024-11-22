modded class ChatInputMenu
{
	static bool ZEN_CHAT_NAME_OVERRIDE = false;

	//! Chat seems to be handled internally by game files I'm too dumb to understand or can't access as a modder so I hijack 
	//	the text chat client-side and RPC any messages to server for custom handling. Not ideal but it's all I know how to do.
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (!finished)
			return false;

		string chatText = m_edit_box.GetText();

		if (chatText == "")
			return false;

		PlayerBase pb = PlayerBase.Cast(GetGame().GetPlayer());
		if (!pb)
			return false;

		if (pb.ZenChat_IsVoiceObstructed())
			chatText = ZenChat_ObfuscateMessage(chatText);

		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenLocalChatMessageOnServer", new Param1<string>(chatText), true, NULL);

		if (ZEN_CHAT_NAME_OVERRIDE || pb.ZenChat_IsVoiceObstructed())
		{
			m_close_timer.Run(0.1, this, "Close");
			GetUApi().GetInputByID(UAPersonView).Supress();

			// If mouth is obstructed, then obfuscate the text chat before relaying to server.
			if (pb.ZenChat_IsVoiceObstructed())
				GetGame().ChatPlayer(chatText);

			// If name override is enabled (ie. Syberia compatibility), 
			// or mouth is obstructed by rag/helmet, don't send text chat 
			// to vanilla code - my RPC will take care of it.
			return true;
		}

		return super.OnChange(w, x, y, finished);
	}

	// Replaced the majority of text with "." for obstructed voices (mouth rag/helmet etc)
	private static string ZenChat_ObfuscateMessage(string message)
	{
		string result = "";
		for (int i = 0; i < message.Length(); i++)
		{
			string currentChar = message.Substring(i, 1); // Extract the character as a string

			// Randomly decide whether to obfuscate the character
			if (Math.RandomFloat01() < 0.75 && currentChar != " ")
			{
				result += "."; // Replace with a period
			}
			else
			{
				result += currentChar; // Keep the original character
			}
		}

		return result;
	}
}