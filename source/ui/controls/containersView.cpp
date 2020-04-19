#include "boxedwine.h"
#include "../boxedwineui.h"
#include "../../../lib/imgui/addon/imguitinyfiledialogs.h"

ContainersView::ContainersView(std::string tab) : BaseView("ContainersView"), currentContainer(NULL), currentContainerChanged(false), currentContainerMountChanged(false), currentApp(NULL), currentAppChanged(false) {
    std::shared_ptr<ImGuiLayout> model = std::make_shared<ImGuiLayout>();        
    section = model->addSection();

    containerNameControl = section->addTextInputRow(CONTAINER_VIEW_CONTAINER_NAME_LABEL, 0);
    containerNameControl->onChange = [this]() {
        this->setTabName(this->tabIndex, this->containerNameControl->getText());
        currentContainer->setName(this->containerNameControl->getText());
        section->setTitle(currentContainer->getName());
        this->currentContainerChanged = true;
    };

    containerWineVersionControl = createWineVersionCombobox(section);
    containerWineVersionControl->onChange = [this]() {        
        this->currentContainerChanged = true;
    };

    containerWindowsVersionControl = createWindowsVersionCombobox(section);
    containerWindowsVersionControl->onChange = [this]() {        
        this->currentContainerChanged = true;
    };


    containerGdiControl = section->addCheckbox(CONTAINER_VIEW_GDI_RENDERER_LABEL, CONTAINER_VIEW_GDI_RENDERER_HELP, false);
    containerGdiControl->onChange = [this]() {        
        this->currentContainerChanged = true;
    };

    std::vector<ComboboxItem> mountDrives;
    mountDrives.push_back(ComboboxItem(" "));
    for (int i = 3; i < 26; i++) {
        mountDrives.push_back(ComboboxItem(std::string(1, (char)('A' + i)) + ":", std::string(1, (char)('a' + i))));
    }
    std::shared_ptr<LayoutRow> row = section->addRow(CONTAINER_VIEW_MOUNT_DIR_LABEL, CONTAINER_VIEW_MOUNT_DIR_HELP);
    containerMountDriveControl = row->addComboBox(mountDrives);
    containerMountDriveControl->setWidth((int)GlobalSettings::scaleFloatUIAndFont(50));
    containerMountDriveControl->onChange = [this]() {
        this->currentContainerChanged = true;
        this->currentContainerMountChanged = true;
    };
    containerMountPathControl = row->addTextInput();
    containerMountPathControl->setBrowseDirButton();
    containerMountPathControl->onChange = [this]() {
        this->currentContainerChanged = true;
        this->currentContainerMountChanged = true;
    };

    row = section->addRow(CONTAINER_VIEW_CONTAINER_LOCATION_LABEL, 0);
    containerLocationControl = row->addTextInput("", true);
    std::shared_ptr<LayoutButtonControl> showButtonControl = row->addButton(getTranslation(GENERIC_OPEN_BUTTON));
    showButtonControl->onChange = [this]() {
        Platform::openFileLocation(currentContainer->getDir());
    };

    row = section->addRow(CONTAINER_VIEW_PROGRAMS_LABEL, 0);
    containerWineCfgButtonControl = row->addButton(getTranslation(CONTAINER_VIEW_WINECFG_BUTTON_LABEL));
    containerWineCfgButtonControl->onChange = [this]() {
        if (this->saveChanges()) {
            std::vector<std::string> args;
            args.push_back("/bin/wine");
            args.push_back("winecfg");
            this->currentContainer->launch(args, "Winecfg");
            std::string containerPath = this->currentContainer->getDir();
            GlobalSettings::startUpArgs.runOnRestartUI = [containerPath]() {
                gotoView(VIEW_CONTAINERS, containerPath);
            };
        }
    };
    containerRegeditButtonControl = row->addButton(getTranslation(CONTAINER_VIEW_REGEDIT_BUTTON_LABEL));
    containerRegeditButtonControl->onChange = [this]() {
        if (this->saveChanges()) {
            std::vector<std::string> args;
            args.push_back("/bin/wine");
            args.push_back("regedit");
            this->currentContainer->launch(args, "Regedit");
            std::string containerPath = this->currentContainer->getDir();
            GlobalSettings::startUpArgs.runOnRestartUI = [containerPath]() {
                gotoView(VIEW_CONTAINERS, containerPath);
            };
        }
    };

    section->addSeparator();
    std::shared_ptr<LayoutButtonControl> selectAppButton = section->addButton(CONTAINER_OPTIONS_DLG_ADD_APP_LABEL, CONTAINER_OPTIONS_DLG_ADD_APP_HELP, getTranslation(CONTAINER_OPTIONS_DLG_ADD_APP_BUTTON_LABEL));
    selectAppButton->onChange = [this]() {
        runOnMainUI([this] {
            new AppChooserDlg(this->currentContainer, [this](BoxedApp* app) {
                this->setCurrentApp(app);
                rebuildShortcutsCombobox();
                this->appPickerControl->setSelectionStringValue(app->getIniFilePath());
                });
            return false;
            });
    };

    appSection = model->addSection();
    appSection->setIndent(true);

    row = section->addRow(CONTAINER_VIEW_SHORTCUT_LIST_LABEL, CONTAINER_VIEW_SHORTCUT_LIST_HELP);
    appPickerControl = row->addComboBox();
    std::shared_ptr<LayoutButtonControl> deleteButton = row->addButton(getTranslation(CONTAINER_VIEW_DELETE_SHORTCUT));
    deleteButton->onChange = [this]() {
        std::string label = getTranslationWithFormat(CONTAINER_VIEW_DELETE_SHORTCUT_CONFIRMATION, true, this->currentApp->getName());
        runOnMainUI([label, this]() {
            new YesNoDlg(GENERIC_DLG_CONFIRM_TITLE, label, [this](bool yes) {
                if (yes) {
                    runOnMainUI([this]() {
                        this->currentAppChanged = false;
                        this->currentApp->remove();
                        if (this->currentContainer->getApps().size()) {
                            this->setCurrentApp(this->currentContainer->getApps()[0]);
                        } else {
                            this->currentApp = NULL;
                        }
                        rebuildShortcutsCombobox();
                        GlobalSettings::reloadApps();
                        return false;
                        });
                }
                });
            return false;
            });
    };
    row = appSection->addRow(0, 0);
    row->setTopMargin(0.0f);
    row->addCustomControl([this]() {
        if (this->currentApp && this->currentApp->getIconTexture()) {
            ImVec2 pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosX(0);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + GlobalSettings::extraVerticalSpacing);
            ImGui::Image(this->currentApp->getIconTexture()->texture, ImVec2((float)UiSettings::ICON_SIZE, (float)UiSettings::ICON_SIZE));
            ImGui::SetCursorPos(pos);
        }
        });

    appNameControl = appSection->addTextInputRow(CONTAINER_VIEW_NAME_LABEL, CONTAINER_VIEW_NAME_HELP);
    appNameControl->onChange = [this]() {
        this->currentApp->name = appNameControl->getText();
        this->currentAppChanged = true;
        rebuildShortcutsCombobox();
    };

    appPathControl = appSection->addTextInputRow(CONTAINER_VIEW_SHORTCUT_PATH_LABEL, CONTAINER_VIEW_SHORTCUT_PATH_HELP, "", true);
    appArgumentsControl = appSection->addTextInputRow(CONTAINER_VIEW_SHORTCUT_ARGUMENTS_LABEL, CONTAINER_VIEW_SHORTCUT_ARGUMENTS_HELP);
    appArgumentsControl->setNumberOfLines(1);
    appArgumentsControl->onChange = [this]() {
        this->currentAppChanged = true;
        std::vector<std::string> args;
        stringSplit(args, appArgumentsControl->getText(), '\n');
        appArgumentsControl->setNumberOfLines((int)args.size() + 1);
    };

    std::vector<ComboboxItem> resolutions;
    resolutions.push_back(ComboboxItem(getTranslation(GENERIC_DEFAULT), ""));
    for (auto& res : GlobalSettings::getAvailableResolutions()) {
        resolutions.push_back(ComboboxItem(res));
    }
    appResolutionControl = appSection->addComboboxRow(CONTAINER_VIEW_RESOLUTION_LABEL, CONTAINER_VIEW_RESOLUTION_HELP, resolutions);
    appResolutionControl->onChange = [this]() {
        this->currentAppChanged = true;
    };

    std::vector<ComboboxItem> bpp;
    bpp.push_back(ComboboxItem("32-bit (default)", 32));
    bpp.push_back(ComboboxItem("16-bit", 16));
    bpp.push_back(ComboboxItem("8-bit (256 colors)", 8));
    appBppControl = appSection->addComboboxRow(CONTAINER_VIEW_BPP_LABEL, CONTAINER_VIEW_BPP_HELP, bpp);
    appBppControl->onChange = [this]() {
        this->currentAppChanged = true;
    };

    appFullScreenControl = appSection->addCheckbox(CONTAINER_VIEW_FULL_SCREEN_LABEL, CONTAINER_VIEW_FULL_SCREEN_HELP, false);
    appFullScreenControl->onChange = [this]() {
        this->currentAppChanged = true;
        if (appFullScreenControl->isChecked()) {
            appScaleControl->setSelection(0);
        }
        appScaleControl->setReadOnly(appFullScreenControl->isChecked());
    };

    std::vector<ComboboxItem> scales;
    scales.push_back(ComboboxItem(getTranslation(GENERIC_DEFAULT), 0));
    scales.push_back(ComboboxItem("1/2x", 50));
    scales.push_back(ComboboxItem("1x", 100));
    scales.push_back(ComboboxItem("2x", 200));
    scales.push_back(ComboboxItem("3x", 300));
    appScaleControl = appSection->addComboboxRow(CONTAINER_VIEW_SCALE_LABEL, CONTAINER_VIEW_SCALE_HELP, scales);
    appScaleControl->onChange = [this]() {
        this->currentAppChanged = true;
    };

    std::vector<ComboboxItem> quality;
    quality.push_back(ComboboxItem("Nearest Pixel Sampling (default)"));
    quality.push_back(ComboboxItem("Linear Filtering"));
    appScaleQualityControl = appSection->addComboboxRow(CONTAINER_VIEW_SCALE_QUALITY_LABEL, CONTAINER_VIEW_SCALE_QUALITY_HELP, quality);
    appScaleQualityControl->onChange = [this]() {
        this->currentAppChanged = true;
    };

    row = appSection->addRow(CONTAINER_VIEW_GL_EXT_LABEL, CONTAINER_VIEW_GL_EXT_HELP);
    appGlExControl = row->addTextInput();
    appGlExControl->setNumberOfLines(3);
    appGlExControl->onChange = [this]() {
        this->currentAppChanged = true;
    };
    std::shared_ptr<LayoutButtonControl> setButtonControl = row->addButton(getTranslation(CONTAINER_VIEW_GL_EXT_SET_BUTTON_LABEL));
    setButtonControl->onChange = [this]() {
        appGlExControl->setText("GL_EXT_multi_draw_arrays GL_ARB_vertex_program\nGL_ARB_fragment_program GL_ARB_multitexture\nGL_EXT_secondary_color GL_EXT_texture_lod_bias\nGL_NV_texture_env_combine4 GL_ATI_texture_env_combine3\nGL_EXT_texture_filter_anisotropic GL_ARB_texture_env_combine\nGL_EXT_texture_env_combine GL_EXT_texture_compression_s3tc\nGL_ARB_texture_compression GL_EXT_paletted_texture");
        this->currentAppChanged = true;
    };

    for (auto& item : BoxedwineData::getContainers()) {
        addTab(item->getName(), model, [this, item](bool buttonPressed, BaseViewTab& tab) {
            if (buttonPressed) {
                this->setCurrentContainer(item);
            }
            });
        if (item->getDir() == tab) {
            this->tabIndex = this->getTabCount() - 1;
        }
    }

    std::shared_ptr<LayoutSection> bottomSection = model->addSection();
    bottomSection->addSeparator();
    std::shared_ptr<LayoutButtonControl> deleteContainerButton = bottomSection->addButton(0, CONTAINER_VIEW_DELETE_BUTTON_HELP, getTranslation(CONTAINER_VIEW_DELETE_BUTTON_LABEL));
    deleteContainerButton->onChange = [this]() {
        std::string label;
        if (!currentContainer->getApps().size()) {
            label = getTranslationWithFormat(CONTAINER_VIEW_DELETE_CONFIRMATION, true, currentContainer->getName());
        } else {
            label = "";

            for (auto& app : currentContainer->getApps()) {
                if (label.length() != 0) {
                    label += ", ";
                }
                label += app->getName();
            }
            label = getTranslationWithFormat(CONTAINER_VIEW_DELETE_CONFIRMATION_WITH_APPS, true, currentContainer->getName(), label);
        }
        runOnMainUI([label, this]() {
            new YesNoDlg(GENERIC_DLG_CONFIRM_TITLE, label, [this](bool yes) {
                if (yes) {
                    runOnMainUI([this]() {
                        currentContainer->deleteContainerFromFilesystem();
                        this->currentContainer = NULL;
                        this->currentApp = NULL;
                        this->currentContainerChanged = false;
                        BoxedwineData::reloadContainers();
                        gotoView(VIEW_CONTAINERS);
                        return false;
                        });
                }
                });
            return false;
            });
    };
}

bool ContainersView::saveChanges() {
    if (this->currentContainer && containerNameControl->getText().length() == 0) {
        this->errorMsg = getTranslation(CONTAINER_VIEW_ERROR_BLANK_NAME);
    } else if (this->containerMountDriveControl->getSelection() != 0 && containerMountPathControl->getText().length()==0) {
        this->errorMsg = getTranslation(CONTAINER_VIEW_ERROR_MISSING_MOUNT_LOCATION);
    } else if (this->containerMountDriveControl->getSelection() == 0 && containerMountPathControl->getText().length() != 0) {
        this->errorMsg = getTranslation(CONTAINER_VIEW_ERROR_MISSING_MOUNT_DRIVE);
    } else if (this->currentApp && appNameControl->getText().length()==0) {
        this->errorMsg = getTranslation(CONTAINER_VIEW_NAME_REQUIRED);
    }
    if (!this->errorMsg) {
        if (this->currentContainer && this->currentContainerChanged) {
            if (this->currentContainerMountChanged) {
                this->currentContainer->clearMounts();
                if (this->containerMountDriveControl->getSelection() != 0) {
                    this->currentContainer->addNewMount(MountInfo(this->containerMountDriveControl->getSelectionStringValue(), containerMountPathControl->getText(), true));
                }
            }
            this->currentContainer->setWineVersion(this->containerWineVersionControl->getSelectionStringValue());
            this->currentContainer->setWindowsVersion(BoxedwineData::getWinVersions()[this->containerWindowsVersionControl->getSelection()]);
            this->currentContainer->setGDI(containerGdiControl->isChecked());
            this->currentContainer->saveContainer();
            this->currentContainerChanged = false;            
        }
        if (this->currentApp && this->currentAppChanged) {
            std::string ext = this->appGlExControl->getText();
            stringReplaceAll(ext, "\n", " ");
            stringReplaceAll(ext, "  ", " ");
            stringReplaceAll(ext, "  ", " ");
            this->currentApp->glExt = ext;
            this->currentApp->name = appNameControl->getText();
            this->currentApp->args.clear();
            stringSplit(this->currentApp->args, appArgumentsControl->getText(), '\n');
            
            this->currentApp->resolution = appResolutionControl->getSelectionStringValue();
            this->currentApp->bpp = appBppControl->getSelectionIntValue();
            this->currentApp->scale = appScaleControl->getSelectionIntValue();
            this->currentApp->scaleQuality = this->appScaleQualityControl->getSelection();
            this->currentApp->fullScreen = this->appFullScreenControl->isChecked();
            this->currentApp->saveApp();
            this->currentAppChanged = false;
            GlobalSettings::reloadApps();
        }
    }
    return this->errorMsg == NULL;
}

void ContainersView::setCurrentApp(BoxedApp* app) {
    this->currentApp = app;
    appNameControl->setText(app->getName());
    appGlExControl->setText(app->glExt);
    

    if (app->link.length()) {
        appPathControl->setText(app->link);
    } else {
        appPathControl->setText(app->path+"/"+app->cmd);
    }
    std::string args;
    for (auto& arg : app->args) {
        if (args.length()) {
            args += "\n";
        }
        args += arg;
    }
    appArgumentsControl->setText(args);
    appArgumentsControl->setNumberOfLines((int)app->args.size() + 1);
    appResolutionControl->setSelectionByLabel(app->resolution);
    appBppControl->setSelectionIntValue(app->bpp);


    if (app->fullScreen) {
        appScaleControl->setSelection(0);
    } else {
        appScaleControl->setSelectionIntValue(app->scale);
    }
    appScaleControl->setReadOnly(app->fullScreen);
    appScaleQualityControl->setSelection(app->scaleQuality);
    appFullScreenControl->setCheck(app->fullScreen);
}

void ContainersView::rebuildShortcutsCombobox() {
    std::vector<ComboboxItem> apps;
    for (auto& app : this->currentContainer->getApps()) {
        apps.push_back(ComboboxItem(app->getName(), app->getIniFilePath()));
    }
    appPickerControl->setOptions(apps);
}

void ContainersView::setCurrentContainer(BoxedContainer* container) {
    this->currentContainer = container;
    this->currentContainerChanged = false;
    this->currentContainerMountChanged = false;
    section->setTitle(container->getName());
    containerWineVersionControl->setSelectionByLabel(container->getWineVersion());
    containerWindowsVersionControl->setSelectionByLabel(container->getWindowsVersion());
    containerNameControl->setText(container->getName());
    containerLocationControl->setText(container->getDir());

    container->updateCachedSize();

    if (!container->getMounts().size() || !container->getMounts()[0].wine || container->getMounts()[0].localPath.length() != 1) {
        containerMountDriveControl->setSelection(0);
        containerMountPathControl->setText("");
    } else {
        const MountInfo& mount = container->getMounts()[0];
        std::string lowerCase = mount.localPath;
        stringToLower(lowerCase);
        containerMountDriveControl->setSelectionStringValue(lowerCase);
        containerMountPathControl->setText(mount.nativePath);
    }
    appPickerControl->setSelection(0);
    if (this->currentContainer->getApps().size()) {
        setCurrentApp(this->currentContainer->getApps()[0]);
        rebuildShortcutsCombobox();        
    } else {
        this->currentApp = NULL;
    }
    containerGdiControl->setCheck(container->isGDI());    
}
