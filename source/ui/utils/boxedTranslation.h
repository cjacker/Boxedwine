#ifndef __BOXED_TRANSLATION_H__
#define __BOXED_TRANSLATION_H__

enum {
INSTALLVIEW_CONTAINER_HELP=1,
INSTALLVIEW_CONTAINER_NAME_HELP,
INSTALLVIEW_TYPE_SETUP_HELP,
INSTALLVIEW_TYPE_DIR_HELP,
INSTALLVIEW_TYPE_MOUNT_HELP,
INSTALLVIEW_RUN_WINE_CONFIG_HELP,
INSTALLVIEW_INSTALL_TYPE_HELP,
INSTALLVIEW_INSTALL_TYPE_LABEL,
INSTALLVIEW_CONTAINER_LABEL,
INSTALLVIEW_CONTAINER_NAME_LABEL,
INSTALLVIEW_CONTAINER_RUN_WINE_CONFIG_LABEL,
INSTALLVIEW_SETUP_FILE_LOCATION_LABEL,
INSTALLVIEW_DIRECTORY_LABEL,
INSTALLVIEW_TITLE,
INSTALLVIEW_OPEN_SETUP_FILE_TITLE,
INSTALLVIEW_ERROR_SETUP_FILE_MISSING,
INSTALLVIEW_ERROR_SETUP_FILE_NOT_FOUND,
INSTALLVIEW_ERROR_DIR_MISSING,
INSTALLVIEW_ERROR_DIR_NOT_FOUND,
INSTALLVIEW_ERROR_CONTAINER_NAME_MISSING,
INSTALLVIEW_ERROR_FAILED_TO_CREATE_CONTAINER_DIR,
INSTALLVIEW_ERROR_CONTAINER_ALREADY_EXISTS,
INSTALLVIEW_ERROR_FILESYSTEM_COPY_DIRECTORY,
INSTALLVIEW_ERROR_FILESYSTEM_FAIL_TO_CREATE_DIRS,
INSTALLVIEW_ERROR_FAILED_TO_MOUNT,
INSTALLVIEW_TYPE_SETUP,
INSTALLVIEW_TYPE_DIRECTORY,
INSTALLVIEW_TYPE_MOUNT,
INSTALLVIEW_TYPE_BLANK,
INSTALLVIEW_INSTALL_TITLE,
INSTALLVIEW_DEMO_TITLE,
INSTALLVIEW_INSTALL_BUTTON_LABEL,
INSTALLVIEW_NEW_CONTAINER,

CONTAINER_VIEW_TITLE,
CONTAINER_VIEW_NAME_LABEL,
CONTAINER_VIEW_NAME_HELP,
CONTAINER_VIEW_RESOLUTION_LABEL,
CONTAINER_VIEW_RESOLUTION_HELP,
CONTAINER_VIEW_BPP_LABEL,
CONTAINER_VIEW_BPP_HELP,
CONTAINER_VIEW_FULL_SCREEN_LABEL,
CONTAINER_VIEW_FULL_SCREEN_HELP,
CONTAINER_VIEW_SCALE_LABEL,
CONTAINER_VIEW_SCALE_HELP,
CONTAINER_VIEW_SCALE_QUALITY_LABEL,
CONTAINER_VIEW_SCALE_QUALITY_HELP,
CONTAINER_VIEW_GL_EXT_LABEL,
CONTAINER_VIEW_GL_EXT_HELP,
CONTAINER_VIEW_GL_EXT_SET_BUTTON_LABEL,
CONTAINER_VIEW_DEFAULT_RESOLUTION_LABEL,
CONTAINER_VIEW_NAME_REQUIRED,
CONTAINER_VIEW_CONTAINER_NAME_LABEL,
CONTAINER_VIEW_CONTAINER_LOCATION_LABEL,
CONTAINER_VIEW_CONTAINER_LOCATION_SIZE_LABEL,
CONTAINER_VIEW_ERROR_BLANK_NAME,
CONTAINER_VIEW_ERROR_MISSING_MOUNT_LOCATION,
CONTAINER_VIEW_ERROR_MISSING_MOUNT_DRIVE,
CONTAINER_VIEW_DELETE_BUTTON_LABEL,
CONTAINER_VIEW_DELETE_BUTTON_HELP,
CONTAINER_VIEW_DELETE_CONFIRMATION,
CONTAINER_VIEW_DELETE_CONFIRMATION_WITH_APPS,
CONTAINER_VIEW_MOUNT_DIR_LABEL,
CONTAINER_VIEW_MOUNT_DIR_HELP,
CONTAINER_VIEW_SHORTCUT_LIST_LABEL,
CONTAINER_VIEW_SHORTCUT_PATH_LABEL,
CONTAINER_VIEW_SHORTCUT_PATH_HELP,
CONTAINER_VIEW_SHORTCUT_ARGUMENTS_LABEL,
CONTAINER_VIEW_SHORTCUT_ARGUMENTS_HELP,
CONTAINER_VIEW_DELETE_SHORTCUT,
CONTAINER_VIEW_DELETE_SHORTCUT_CONFIRMATION,
CONTAINER_VIEW_SHORTCUT_LIST_HELP,
CONTAINER_VIEW_GDI_RENDERER_LABEL,
CONTAINER_VIEW_GDI_RENDERER_HELP,
CONTAINER_VIEW_PROGRAMS_LABEL,
CONTAINER_VIEW_REGEDIT_BUTTON_LABEL,
CONTAINER_VIEW_WINECFG_BUTTON_LABEL,
CONTAINER_VIEW_WINDOWS_VERION_LABEL,
CONTAINER_VIEW_WINDOWS_VERION_HELP,

CONTAINER_OPTIONS_DLG_TITLE,
CONTAINER_OPTIONS_DLG_WINE_VERSION_LABEL,
CONTAINER_OPTIONS_DLG_ADD_APP_LABEL,
CONTAINER_OPTIONS_DLG_ADD_APP_HELP,
CONTAINER_OPTIONS_DLG_ADD_APP_BUTTON_LABEL,

SETTINGS_DLG_TITLE,
OPTIONSVIEW_SAVE_FOLDER_LABEL,
OPTIONSVIEW_SAVE_FOLDER_HELP,
OPTIONSVIEW_THEME_LABEL,
OPTIONSVIEW_THEME_HELP,
OPTIONSVIEW_THEME_DARK,
OPTIONSVIEW_THEME_CLASSIC,
OPTIONSVIEW_THEME_LIGHT,
OPTIONSVIEW_ERROR_DATA_DIR_NOT_FOUND,
OPTIONSVIEW_TITLE_GENERAL,
OPTIONSVIEW_TITLE_DISPLAY,
OPTIONSVIEW_TITLE_WINE_VERSISONS,
OPTIONSVIEW_WINE_VERSION_UPTODATE,
OPTIONSVIEW_WINE_VERSION_UPDATE_AVAILABLE,
OPTIONSVIEW_WINE_VERSION_NOT_INSTALLED,
OPTIONSVIEW_WINE_VERSION_DELETE,
OPTIONSVIEW_WINE_VERSION_INSTALL,
OPTIONSVIEW_WINE_VERSION_UPDATE,
OPTIONSVIEW_WINE_VERSION_DELETE_CONFIRM_LABEL,
OPTIONSVIEW_DEFAULT_RESOLUTION_LABEL,
OPTIONSVIEW_DEFAULT_RESOLUTION_HELP,
OPTIONSVIEW_DEFAULT_SCALE_LABEL,
OPTIONSVIEW_DEFAULT_SCALE_HELP,
OPTIONSVIEW_DEFAULT_FONT_SCALE_LABEL,
OPTIONSVIEW_DEFAULT_FONT_SCALE_HELP,

APPCHOOSER_DLG_TITLE,
APPCHOOSER_DLG_CHOOSE_APP_LABEL,
APPCHOOSER_DLG_NO_APPS_LABEL,

GENERIC_BROWSE_BUTTON,
GENERIC_OPEN_BUTTON,
GENERIC_DLG_OK,
GENERIC_DLG_YES,
GENERIC_DLG_NO,
GENERIC_DLG_CANCEL,
GENERIC_DLG_ERROR_TITLE,
GENERIC_OPEN_FOLDER_TITLE,
GENERIC_DLG_CONFIRM_TITLE,
GENERIC_DEFAULT,

COMMON_WINE_VERSION_LABEL,
COMMON_WINE_VERSION_HELP,

WAITDLG_LAUNCH_APP_TITLE,
WAITDLG_LAUNCH_APP_LABEL,
WAITDLG_GET_FILE_LIST_TITLE,
WAITDLG_GET_FILE_LIST_LABEL,

DOWNLOADDLG_TITLE,
DOWNLOADDLG_LABEL,

MAIN_BUTTON_INSTALL,
MAIN_BUTTON_CONTAINERS,
MAIN_BUTTON_SETTINGS,
MAIN_BUTTON_APPS,

ERROR_NO_WINE,
ERROR_MISSING_WINE,
ERROR_NO_FILE_LIST
};

const char* getTranslation(int msg, bool useDefaultIfMissing=true);
std::string getTranslationWithFormat(int msg, bool useDefaultIfMissing, const std::vector<std::string>& replacements);
std::string getTranslationWithFormat(int msg, bool useDefaultIfMissing, const std::string& string1);
std::string getTranslationWithFormat(int msg, bool useDefaultIfMissing, const std::string& string1, const std::string& string2);

#endif