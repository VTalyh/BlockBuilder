#include "BlockBuilderPluginPrivatePCH.h"
#include "BlockBuilderWindow.h"

#define LOCTEXT_NAMESPACE "BlockBuilderPlugin"

const FName BlockBuilderWindowTabName(TEXT("BlockBuilderWindowTab"));

class FBlockBuilderPlugin : public IBlockBuilderPlugin
{
private:
	void CreateWindowMenu(FMenuBuilder& menuBuilder);
	void OpenPluginWindow();
	TSharedRef<SDockTab> CreateTab(const FSpawnTabArgs& args);
public:
	FBlockBuilderPlugin();
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FBlockBuilderPlugin, BlockBuilderPlugin)

DEFINE_LOG_CATEGORY(BlockBuilderPluginLog);

void FBlockBuilderPlugin::CreateWindowMenu(FMenuBuilder& menuBuilder)
{
	menuBuilder.AddMenuEntry(
		LOCTEXT("OpenWindow", "BlockBuilder Plugin Window"),
		LOCTEXT("OpenWindowTooltip", "Opens the BlockBuilder Plugin window"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FBlockBuilderPlugin::OpenPluginWindow)));
}

void FBlockBuilderPlugin::OpenPluginWindow()
{
	FGlobalTabmanager::Get()->InvokeTab(BlockBuilderWindowTabName);	
}

TSharedRef<SDockTab> FBlockBuilderPlugin::CreateTab(FSpawnTabArgs const& args)
{
	return 
		SNew(SDockTab)
		.Label(LOCTEXT("TabTitle", "BlockBuilder Window"))
		.TabRole(ETabRole::MajorTab)
		.ContentPadding(5)
		[
			SNew(SBorder)
			.Padding(4)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(BlockBuilderWindow)
			]
		];
}

FBlockBuilderPlugin::FBlockBuilderPlugin()
{
}

void FBlockBuilderPlugin::StartupModule()
{
	TSharedPtr<FExtender> extender = MakeShareable(new FExtender);
	extender->AddMenuExtension(
		"WindowLocalTabSpawners",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateRaw(this, &FBlockBuilderPlugin::CreateWindowMenu));

	FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditor.GetMenuExtensibilityManager()->AddExtender(extender);

	FGlobalTabmanager::Get()->RegisterTabSpawner(BlockBuilderWindowTabName, FOnSpawnTab::CreateRaw(this, &FBlockBuilderPlugin::CreateTab)).SetDisplayName(FText::FromString(TEXT("BlockBuilder Window")));
}

void FBlockBuilderPlugin::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
