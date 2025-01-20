// Copyright Epic Games, Inc. All Rights Reserved.

#include "ComponentPointerFixer.h"

#include "Editor.h"
#include "SSubobjectEditor.h"
#include "SSubobjectInstanceEditor.h"
#include "SubobjectEditorMenuContext.h"
#include "ToolMenus.h"
#include "ToolMenuSection.h"
#include "Subsystems/EditorAssetSubsystem.h"

#define LOCTEXT_NAMESPACE "FComponentPointerFixerModule"

namespace
{
	bool GetSubobjectEditorFromContext(const FToolMenuContext& InContext, TSharedPtr<SSubobjectEditor>& SubobjectEditor)
	{
		USubobjectEditorMenuContext* contextObject = InContext.FindContext<USubobjectEditorMenuContext>();
		if (!contextObject)
		{
			return false;
		}

		SubobjectEditor = contextObject->SubobjectEditor.Pin();
		if (!SubobjectEditor.IsValid() || !StaticCastSharedPtr<SSubobjectInstanceEditor>(SubobjectEditor))
		{
			return false;
		}

		return true;
	}

	bool GetEditorsFromContext(const FToolMenuContext& inContext, TSharedPtr<SSubobjectEditor>& subobjectEditor, TSharedPtr<FBlueprintEditor>& editor)
	{
		if(!GetSubobjectEditorFromContext(inContext, subobjectEditor))
		{
			return false;
		}

		const FBlueprintEditorModule& blueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
		for (const TSharedRef<IBlueprintEditor>& openEditor : blueprintEditorModule.GetBlueprintEditors())
		{
			const TSharedRef<FBlueprintEditor>& openBlueprintEditor = StaticCastSharedRef<FBlueprintEditor>(openEditor);
			if (openBlueprintEditor->GetSubobjectEditor() == subobjectEditor)
			{
				editor = openBlueprintEditor;
				return true;
			}
		}

		return false;
	}
}

void FComponentPointerFixerModule::StartupModule()
{
	RegisterMenus();
}

void FComponentPointerFixerModule::ShutdownModule() {}

void FComponentPointerFixerModule::RegisterMenus()
{
	FToolMenuOwnerScoped ownerScoped(this);

	UToolMenu* menu = UToolMenus::Get()->ExtendMenu("Kismet.SubobjectEditorContextMenu");
	FToolMenuSection& section = menu->AddSection("Component Pointer Fixer", LOCTEXT("ComponentFixerHeader", "Component Pointer Fixer"));
	RegisterFixAction(section);
}

void FComponentPointerFixerModule::RegisterFixAction(FToolMenuSection& section)
{
	FToolUIAction action;
	action.ExecuteAction = FToolMenuExecuteAction::CreateLambda([&](const FToolMenuContext& context)
	{
		TSharedPtr<SSubobjectEditor> subobjectEditor;
		TSharedPtr<FBlueprintEditor> bpEditor;
		if (!GetEditorsFromContext(context, subobjectEditor, bpEditor))
		{
			return;
		}

		const TArray<TSharedPtr<FSubobjectEditorTreeNode>> selectedNodes = subobjectEditor->GetSelectedNodes();
		if (selectedNodes.Num() < 1)
		{
			return;
		}
    	
		UObject* cdo = selectedNodes[0]->GetObject()->GetOuter()->GetClass()->GetDefaultObject(); // this is gross, oh well
		for (TSharedPtr<FSubobjectEditorTreeNode> node : selectedNodes)
		{
			UObject* outer = node->GetObject()->GetOuter();
			FObjectProperty* componentPointer = FindFProperty<FObjectProperty>(outer->GetClass(), node->GetVariableName());

			if (UObject** valuePtr = componentPointer->ContainerPtrToValuePtr<UObject*>(cdo))
			{
				*valuePtr = const_cast<UObject*>(node->GetObject());
			}
		}

		UEditorAssetSubsystem* editor = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
		editor->SaveAsset(cdo->GetClass()->GetPackage()->GetFullName(), false);
	});
    
	section.AddMenuEntry(
		"Component Pointer Fixer",
		LOCTEXT( "Component FixerLabel", "Fix component pointer"),
		LOCTEXT( "Component FixerTooltip", "Attempts to reassign the component pointer from the old component to the new one."),
		FSlateIcon(),
		action,
		EUserInterfaceActionType::Button);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FComponentPointerFixerModule, ComponentPointerFixer)