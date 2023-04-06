install_name_tool -change @rpath/QtCore.framework/Versions/A/QtCore @loader_path/../../Frameworks/QtCore.framework/Versions/A/QtCore $1
install_name_tool -change @rpath/QtQml.framework/Versions/A/QtQml @loader_path/../../Frameworks/QtQml.framework/Versions/A/QtQml $1
install_name_tool -change @rpath/QtQuickLayouts.framework/Versions/A/QtQuickLayouts @loader_path/../../Frameworks/QtQuickLayouts.framework/Versions/A/QtQuickLayouts $1
install_name_tool -change @rpath/QtQuickControls2.framework/Versions/A/QtQuickControls2 @loader_path/../../Frameworks/QtQuickControls2.framework/Versions/A/QtQuickControls2 $1
install_name_tool -change @rpath/QtQuickControls2Impl.framework/Versions/A/QtQuickControls2Impl @loader_path/../../Frameworks/QtQuickControls2Impl.framework/Versions/A/QtQuickControls2Impl $1
install_name_tool -change @rpath/QtWidgets.framework/Versions/A/QtWidgets @loader_path/../../Frameworks/QtWidgets.framework/Versions/A/QtWidgets $1
install_name_tool -change @rpath/QtQuickWidgets.framework/Versions/A/QtQuickWidgets @loader_path/../../Frameworks/QtQuickWidgets.framework/Versions/A/QtQuickWidgets $1
install_name_tool -change @rpath/QtCore5Compat.framework/Versions/A/QtCore5Compat @loader_path/../../Frameworks/QtCore5Compat.framework/Versions/A/QtCore5Compat $1
install_name_tool -change @rpath/QtQuickTemplates2.framework/Versions/A/QtQuickTemplates2 @loader_path/../../Frameworks/QtQuickTemplates2.framework/Versions/A/QtQuickTemplates2 $1
install_name_tool -change @rpath/QtQuick.framework/Versions/A/QtQuick @loader_path/../../Frameworks/QtQuick.framework/Versions/A/QtQuick $1
install_name_tool -change @rpath/QtOpenGL.framework/Versions/A/QtOpenGL @loader_path/../../Frameworks/QtOpenGL.framework/Versions/A/QtOpenGL $1
install_name_tool -change @rpath/QtGui.framework/Versions/A/QtGui @loader_path/../../Frameworks/QtGui.framework/Versions/A/QtGui $1
install_name_tool -change @rpath/QtQmlModels.framework/Versions/A/QtQmlModels @loader_path/../../Frameworks/QtQmlModels.framework/Versions/A/QtQmlModels $1
install_name_tool -change @rpath/QtDBus.framework/Versions/A/QtDBus @loader_path/../../Frameworks/QtDBus.framework/Versions/A/QtDBus $1
install_name_tool -change @rpath/QtQmlWorkerScript.framework/Versions/A/QtQmlWorkerScript @loader_path/../../Frameworks/QtQmlWorkerScript.framework/Versions/A/QtQmlWorkerScript $1
install_name_tool -change @rpath/QtLabsFolderListModel.framework/Versions/A/QtLabsFolderListModel @loader_path/../../Frameworks/QtLabsFolderListModel.framework/Versions/A/QtLabsFolderListModel $1
install_name_tool -change @rpath/QtNetwork.framework/Versions/A/QtNetwork @loader_path/../../Frameworks/QtNetwork.framework/Versions/A/QtNetwork $1

