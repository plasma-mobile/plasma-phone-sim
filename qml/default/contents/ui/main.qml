import QtQuick 2.0
import QtQuick.Controls 1.0 as QtControls
import org.kde.plasma.core 2.0 as Plasmacore
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1

ColumnLayout {
    anchors.fill: parent
    FileDialog {
        id: fileDialog
        selectExisting: true
        selectMultiple: false
        onAccepted: {
            api.packagePath = fileDialog.fileUrl
        }
    }

    QtControls.Button {
        Layout.alignment: Qt.AlignCenter
        text: i18n("Load QML file")
        onClicked: {
            fileDialog.selectedNameFilter = ".qml";
            fileDialog.selectFolder = false;
            fileDialog.visible = true;
        }
    }

    QtControls.Button {
        Layout.alignment: Qt.AlignCenter
        text: i18n("Load QML package")
        onClicked: {
            fileDialog.selectedNameFilter = "";
            fileDialog.selectFolder = true;
            fileDialog.visible = true;
        }
    }

    QtControls.Button {
        Layout.alignment: Qt.AlignCenter
        text: i18n("Load shell package")
        onClicked: {
            fileDialog.selectedNameFilter = "";
            fileDialog.selectFolder = true;
            fileDialog.visible = true;
        }
    }
}
