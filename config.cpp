class CfgPatches
{
	class ZenImmersiveChatHUD
	{
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts"
		};
	};
};

class CfgMods
{
	class ZenImmersiveChatHUD
	{
		dir = "ZenImmersiveChatHUD";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ZenImmersiveChatHUD";
		credits = "";
		author = "Zenarchist";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = { "Game","World","Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "ZenImmersiveChatHUD/scripts/3_game" };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "ZenImmersiveChatHUD/scripts/4_world" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "ZenImmersiveChatHUD/scripts/5_mission" };
			};
		};
	};
};
