#include "BlockBuilderPluginPrivatePCH.h"
#include "BlockBuilderWindow.h"

#define LOCTEXT_NAMESPACE "BlockBuilderPluginWindow"

FString BlockBuilderWindow::GetBoundsActorName() const
{
	if (_boundsActor == nullptr)	
		return FString::Printf(TEXT("No Actor Selected"));

	return _boundsActor->GetName();
}

FSlateColor BlockBuilderWindow::GetBoundsActorColorAndOpacity() const
{
	if (_boundsActor == nullptr)
		return FLinearColor(1, 0, 0);

	return FLinearColor(1, 1, 1);
}

FSlateColor BlockBuilderWindow::GetBoundsActorForeground() const
{
	if (_boundsActorDropdown->IsHovered())
		return FEditorStyle::GetSlateColor("InvertedForeground");

	return FEditorStyle::GetSlateColor("DefaultForeground");
}

bool BlockBuilderWindow::IsActorSelected(ATriggerBox* actor)
{
	return actor == _boundsActor;
}

void BlockBuilderWindow::SelectBoundsActor(ATriggerBox* actor)
{
	_boundsActor = actor;
}

TSharedRef<SWidget> BlockBuilderWindow::GetBoundsActorContent()
{
	UWorld* world = GetWorld();
	FMenuBuilder menu(true, nullptr);

	for (TActorIterator<ATriggerBox> i(world); i; ++i)
	{
		menu.AddMenuEntry(
			FText::FromString(i->GetName()),
			FText::FromString(i->GetName()),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &BlockBuilderWindow::SelectBoundsActor, *i),
				FCanExecuteAction(),
				FIsActionChecked::CreateSP(this, &BlockBuilderWindow::IsActorSelected, *i)
			),
			NAME_None,
			EUserInterfaceActionType::ToggleButton);
	}

	return menu.MakeWidget();
}

FString BlockBuilderWindow::GetAlleySpaceText() const
{
	if (_alleySpace == AlleySpace::NoAlley)
		return FString::Printf(TEXT("No Alley"));

	if (_alleySpace == AlleySpace::XAlley)
		return FString::Printf(TEXT("X Alley"));

	if (_alleySpace == AlleySpace::YAlley)
		return FString::Printf(TEXT("Y Alley"));

	return FString::Printf(TEXT("Error"));
}

void BlockBuilderWindow::SelectAlleySpace(AlleySpace::E space)
{
	_alleySpace = space;
}

bool BlockBuilderWindow::IsAlleySpaceSelected(AlleySpace::E space) const
{
	return _alleySpace == space;
}

TSharedRef<SWidget> BlockBuilderWindow::GetAlleySpaceContext()
{
	FMenuBuilder menu(true, nullptr);

	menu.AddMenuEntry(
		FText::FromString(TEXT("No Alley")),
		FText::FromString(TEXT("No Alley")),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateSP(this, &BlockBuilderWindow::SelectAlleySpace, AlleySpace::NoAlley),
			FCanExecuteAction(),
			FIsActionChecked::CreateSP(this, &BlockBuilderWindow::IsAlleySpaceSelected, AlleySpace::NoAlley)
		),
		NAME_None,
		EUserInterfaceActionType::ToggleButton);

	menu.AddMenuEntry(
		FText::FromString(TEXT("X Alley")),
		FText::FromString(TEXT("X Alley")),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateSP(this, &BlockBuilderWindow::SelectAlleySpace, AlleySpace::XAlley),
			FCanExecuteAction(),
			FIsActionChecked::CreateSP(this, &BlockBuilderWindow::IsAlleySpaceSelected, AlleySpace::XAlley)
		),
		NAME_None,
		EUserInterfaceActionType::ToggleButton);

	menu.AddMenuEntry(
		FText::FromString(TEXT("Y Alley")),
		FText::FromString(TEXT("Y Alley")),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateSP(this, &BlockBuilderWindow::SelectAlleySpace, AlleySpace::YAlley),
			FCanExecuteAction(),
			FIsActionChecked::CreateSP(this, &BlockBuilderWindow::IsAlleySpaceSelected, AlleySpace::YAlley)
		),
		NAME_None,
		EUserInterfaceActionType::ToggleButton);

	return menu.MakeWidget();
}

void BlockBuilderWindow::BuildinsPerXChanged(int32 value)
{
	_buildingsPerX = value;
}

TOptional<int32> BlockBuilderWindow::GetBuildingsPerX() const
{
	return _buildingsPerX;
}

void BlockBuilderWindow::BuildinsPerYChanged(int32 value)
{
	_buildingsPerY = value;
}

TOptional<int32> BlockBuilderWindow::GetBuildingsPerY() const
{
	return _buildingsPerY;
}

void BlockBuilderWindow::AlleyWidthChanged(float value)
{
	_alleyWidth = value;
}

TOptional<float> BlockBuilderWindow::GetAlleyWidth() const
{
	return _alleyWidth;
}

void BlockBuilderWindow::HeightPercentChanged(float value)
{
	_heightPercent = value;
}

TOptional<float> BlockBuilderWindow::GetHeightPercent() const
{
	return _heightPercent;
}

void BlockBuilderWindow::MinSizePercentChanged(float value)
{
	_minSizePercent = value;
}

TOptional<float> BlockBuilderWindow::GetMinSizePercent() const
{
	return _minSizePercent;
}

void BlockBuilderWindow::SpacingChanged(float value)
{
	_spacing = value;
}

TOptional<float> BlockBuilderWindow::GetSpacing() const
{
	return _spacing;
}

FReply BlockBuilderWindow::DoBuild()
{
	if (_boundsActor == nullptr)
		return FReply::Handled();

	FVector boundsSize = _boundsActor->GetComponentsBoundingBox().GetExtent();
	FVector boundsPosition = _boundsActor->GetActorLocation();
	FVector boundsMin = boundsPosition - boundsSize;
	boundsSize *= 2;

	float xBuildingBounds = (boundsSize.X - (_alleySpace == AlleySpace::XAlley ? _alleyWidth : 0)) / static_cast<float>(_buildingsPerX);
	float yBuildingBounds = (boundsSize.Y - (_alleySpace == AlleySpace::YAlley ? _alleyWidth : 0)) / static_cast<float>(_buildingsPerY);

	float alleyPivot = 0;
	if (_alleySpace == AlleySpace::XAlley)
		alleyPivot = _buildingsPerX / 2.f;
	if (_alleySpace == AlleySpace::YAlley)
		alleyPivot = _buildingsPerY / 2.f;

	FVector position, size;
	for (int x = 0; x < _buildingsPerX; x++)
	{
		for (int y = 0; y < _buildingsPerY; y++)
		{
			size.X = FMath::FRandRange(xBuildingBounds * (_minSizePercent / 100.f), xBuildingBounds) - _spacing;
			size.Y = FMath::FRandRange(yBuildingBounds * (_minSizePercent / 100.f), yBuildingBounds) - _spacing;
			size.Z = FMath::FRandRange(boundsSize.Z * (_minSizePercent / 100.f), boundsSize.Z);

			position.X = boundsMin.X + (xBuildingBounds * x) + _spacing + (size.X / 2.f);
			position.Y = boundsMin.Y + (yBuildingBounds * y) + _spacing + (size.Y / 2.f);
			position.Z = boundsMin.Z + (size.Z / 2.f);

			if (_alleySpace == AlleySpace::XAlley && x >= alleyPivot)
				position.X += _alleyWidth;
			else if (_alleySpace == AlleySpace::YAlley && y >= alleyPivot)
				position.Y += _alleyWidth;

			CreateBrush(position, size);
		}
	}

	return FReply::Handled();
}

void BlockBuilderWindow::CreateBrush(FVector position, FVector size)
{
	UWorld* world = GetWorld();

	UCubeBuilder* cubeBuilder = Cast<UCubeBuilder>(GEditor->FindBrushBuilder(UCubeBuilder::StaticClass()));

	cubeBuilder->X = size.X;
	cubeBuilder->Y = size.Y;
	cubeBuilder->Z = size.Z;
	cubeBuilder->Build(world);

	world->GetBrush()->SetActorLocation(position);
	GEditor->RedrawLevelEditingViewports();
	GEditor->Exec(world, TEXT("BRUSH ADD"));

}

UWorld* BlockBuilderWindow::GetWorld()
{
	UWorld* world = nullptr;
	const TIndirectArray < FWorldContext >& contexts = GEngine->GetWorldContexts();
	for (int32 i = 0; i < contexts.Num(); i++)
	{
		if (contexts[i].WorldType == EWorldType::PIE)
			return contexts[i].World();
		
		if (contexts[i].WorldType == EWorldType::Editor)
			world = contexts[i].World();
	}

	return world;
}

BlockBuilderWindow::BlockBuilderWindow() :
	_buildingsPerX(1),
	_buildingsPerY(1),
	_alleyWidth(100),
	_heightPercent(50),
	_minSizePercent(50),
	_spacing(10),
	_boundsActor(nullptr),
	_alleySpace(AlleySpace::NoAlley)
{
}

void BlockBuilderWindow::Construct(FArguments const& args)
{
	TSharedRef<SVerticalBox> vertBox = SNew(SVerticalBox);

	ChildSlot
	[
		vertBox
	];

	vertBox->AddSlot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(this, &BlockBuilderWindow::GetBoundsActorName)
				.ColorAndOpacity(this, &BlockBuilderWindow::GetBoundsActorColorAndOpacity)
			]

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SAssignNew(_boundsActorDropdown, SComboButton)
				.ContentPadding(0)
				.ForegroundColor(this, &BlockBuilderWindow::GetBoundsActorForeground)
				.OnGetMenuContent(this, &BlockBuilderWindow::GetBoundsActorContent)
				.ButtonStyle(FEditorStyle::Get(), "ToggleButton")
				.ButtonContent()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SImage).Image(FEditorStyle::GetBrush("GenericViewButton"))
					]

					+SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(2, 0, 0, 0)
					[
						SNew(STextBlock).Text(this, &BlockBuilderWindow::GetBoundsActorName)
					]
				]
			]
		];

	vertBox->AddSlot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("AlleySpaceName", "Alley Space"))
				.ColorAndOpacity(this, &BlockBuilderWindow::GetBoundsActorColorAndOpacity)
			]

			+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(SComboButton)
					.ContentPadding(0)
					.OnGetMenuContent(this, &BlockBuilderWindow::GetAlleySpaceContext)
					.ButtonStyle(FEditorStyle::Get(), "ToggleButton")
					.ButtonContent()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SImage).Image(FEditorStyle::GetBrush("GenericViewButton"))
						]

						+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(2, 0, 0, 0)
							[
								SNew(STextBlock).Text(this, &BlockBuilderWindow::GetAlleySpaceText)
							]
					]
				]
		];

	vertBox->AddSlot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("BuildingXName", "Buildings X"))
			]

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)			
			[
				SNew(SNumericEntryBox<int32>)
				.AllowSpin(true)
				.MinValue(1)
				.MaxValue(30)
				.MinSliderValue(1)
				.MaxSliderValue(30)
				.Value(this, &BlockBuilderWindow::GetBuildingsPerX)
				.OnValueChanged(this, &BlockBuilderWindow::BuildinsPerXChanged)
			]
		];

	vertBox->AddSlot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("BuildingYName", "Buildings Y"))
			]

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SNumericEntryBox<int32>)
				.AllowSpin(true)
				.MinValue(1)
				.MaxValue(30)
				.MinSliderValue(1)
				.MaxSliderValue(30)
				.Value(this, &BlockBuilderWindow::GetBuildingsPerY)
				.OnValueChanged(this, &BlockBuilderWindow::BuildinsPerYChanged)
			]
		];

	vertBox->AddSlot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("AlleywidthName", "Alley Width"))
			]

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(1)
				.MaxValue(1000)
				.MinSliderValue(1)
				.MaxSliderValue(1000)
				.Value(this, &BlockBuilderWindow::GetAlleyWidth)
				.OnValueChanged(this, &BlockBuilderWindow::AlleyWidthChanged)
			]
		];

	vertBox->AddSlot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("HeightpercentName", "Height Percent"))
			]

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(0)
				.MaxValue(100)
				.MinSliderValue(0)
				.MaxSliderValue(100)
				.Value(this, &BlockBuilderWindow::GetHeightPercent)
				.OnValueChanged(this, &BlockBuilderWindow::HeightPercentChanged)
			]
		];

	vertBox->AddSlot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("MinsizepercentName", "Min Size Percent"))
			]

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(0)
				.MaxValue(100)
				.MinSliderValue(0)
				.MaxSliderValue(100)
				.Value(this, &BlockBuilderWindow::GetMinSizePercent)
				.OnValueChanged(this, &BlockBuilderWindow::MinSizePercentChanged)
			]
		];

	vertBox->AddSlot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("SpacingName", "Spacing"))
			]

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(0)
				.MaxValue(1000)
				.MinSliderValue(0)
				.MaxSliderValue(1000)
				.Value(this, &BlockBuilderWindow::GetSpacing)
				.OnValueChanged(this, &BlockBuilderWindow::SpacingChanged)
			]
		];

	vertBox->AddSlot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SButton)
			.Text(FString(TEXT("Build")))
			.OnClicked(this, &BlockBuilderWindow::DoBuild)
		];
}

#undef  LOCTEXT_NAMESPACE
