modded class MissionServer
{
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		if (player.GetZenChatName() != "")
			GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenLocalChatMessageNameOverride", new Param1<bool>(true), true, identity);
	}
}