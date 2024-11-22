modded class MissionBase
{
    //! SHARED 
    void MissionBase()
    {
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenLocalChatMessageOnServer", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenLocalChatMessageOnClient", this, SingeplayerExecutionType.Client);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenLocalChatMessageNameOverride", this, SingeplayerExecutionType.Client);
    }

    //! SERVER RECEIVE
    void RPC_ReceiveZenLocalChatMessageOnServer(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server)
            return;

        Param1<string> data;
        if (!ctx.Read(data))
        {
            Error("Error receiving data - RPC_ReceiveZenLocalChatMessageOnServer");
            return;
        }

        if (sender && data.param1)
        {
            int highBits, lowBits;
            GetGame().GetPlayerNetworkIDByIdentityID(sender.GetPlayerId(), lowBits, highBits);
            PlayerBase senderPlayer = PlayerBase.Cast(GetGame().GetObjectByNetworkId(lowBits, highBits));
            if (!senderPlayer)
                return;

            string playerName = senderPlayer.GetZenChatName();
            int voiceLevel = GetGame().GetVoiceLevel(senderPlayer);
            string message = data.param1;

            array<Man> players();
            PlayerBase pb;
            GetGame().GetPlayers(players);

            // Override to whisper for obstructed voice
            if (senderPlayer.ZenChat_IsVoiceObstructed())
                voiceLevel = 0;

            int maxDistance;
            if (voiceLevel == 0)
                maxDistance = 7;
            else
            if (voiceLevel == 1)
                maxDistance = 30;
            else
                maxDistance = 45;

            for (int i = 0; i < players.Count(); i++)
            {
                pb = PlayerBase.Cast(players.Get(i));
                if (pb && vector.Distance(pb.GetPosition(), senderPlayer.GetPosition()) <= maxDistance)
                {
                    if (playerName == "" && pb == senderPlayer)
                        continue;
 
                    GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenLocalChatMessageOnClient", new Param4<PlayerBase, string, string, int>(senderPlayer, playerName, message, voiceLevel), true, pb.GetIdentity());
                }
            }

            DayZPlayerType pt = senderPlayer.GetDayZPlayerType();

            // Add noise when message is sent
            NoiseParams vonpar;
            switch (voiceLevel)
            {
                case VoiceLevelWhisper:
                    vonpar = pt.GetNoiseParamsWhisper();
                    break;
                case VoiceLevelTalk:
                    vonpar = pt.GetNoiseParamsTalk();
                    break;
                case VoiceLevelShout:
                    vonpar = pt.GetNoiseParamsShout();
                    break;
            }

            senderPlayer.AddNoise(vonpar);

            // Player name can be overridden by mods (eg. Syberia) instead of using Steam profile name
            if (playerName != "")
            {
                // Essentially for other mod compatibility, this processes server-side and mimics when a vanilla text chat is received
                ChatMessageEventParams chatParams = new ChatMessageEventParams(CCDirect, playerName, message, "");
                MissionGameplay missionGP = MissionGameplay.Cast(GetGame().GetMission());
                if (missionGP)
                    missionGP.OnEvent(ChatMessageEventTypeID, chatParams);

#ifdef GAMELABS
                _LogPlayerEx logObjectPlayer = new _LogPlayerEx(senderPlayer);
                _Payload_PlayerChat payloadVicinityChat = new _Payload_PlayerChat(logObjectPlayer, playerName, message);
                GetGameLabs().GetApi().PlayerChat(new _Callback(), payloadVicinityChat);
#endif
            }
        }
    }

    //! CLIENT RECEIVE
    void RPC_ReceiveZenLocalChatMessageOnClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param4<PlayerBase, string, string, int> data;
        if (!ctx.Read(data))
        {
            Error("Error receiving data - RPC_ReceiveZenLocalChatMessageOnClient");
            return;
        }

        if (data.param1)
        {
            PlayerBase senderPlayer = data.param1;
            string playerName = data.param2;
            string message = data.param3;
            int shoutLevel = data.param4;

            PlayerBase ourPlayer = PlayerBase.Cast(GetGame().GetPlayer());
            if (ourPlayer != senderPlayer)
            {
                ZenChatMessagePlugin plugin = ZenChatMessagePlugin.Cast(GetPlugin(ZenChatMessagePlugin));
                if (plugin)
                {
                    plugin.AddChatMessage(senderPlayer, message, shoutLevel);
                }
            }

            if (playerName != "")
            {
                // If custom player name is not blank, then we've hijacked vanilla mechanics and we need to mimic vanilla here
                ChatMessageEventParams chatParams = new ChatMessageEventParams(CCDirect, playerName, message, "");
                MissionGameplay missionGP = MissionGameplay.Cast(GetGame().GetMission());
                if (missionGP)
                    missionGP.OnEvent(ChatMessageEventTypeID, chatParams);
            }
        }
    }

    //! CLIENT NAME OVERRIDE RECEIVE (ie. Syberia compatibility)
    void RPC_ReceiveZenLocalChatMessageNameOverride(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param1<bool> data;
        if (!ctx.Read(data))
        {
            Error("Error receiving data - RPC_ReceiveZenLocalChatMessageNameOverride");
            return;
        }

        if (data.param1)
        {
            ChatInputMenu.ZEN_CHAT_NAME_OVERRIDE = true;
        }
    }
}