modded class PluginManager
{
	override void Init()
	{
		super.Init();

		RegisterPlugin("ZenChatMessagePlugin", true, false);
	}
}