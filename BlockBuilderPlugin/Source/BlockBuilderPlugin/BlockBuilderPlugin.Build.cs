namespace UnrealBuildTool.Rules
{
	public class BlockBuilderPlugin : ModuleRules
	{
        public BlockBuilderPlugin(TargetInfo Target)
		{

			PrivateIncludePaths.AddRange(
				new string[] {
					"BlockBuilderPlugin/Private",
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Engine",
                    "UnrealEd",
                    "InputCore",
                    "Core",
                    "Slate",
                    "SlateCore",
                    "EditorStyle",
                    "CoreUObject",
                    "LevelEditor"
				}
				);
		}
	}
}
