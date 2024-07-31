import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCMUtils
import org.example.imaupdatesettings 1.0

KCMUtils.SimpleKCM {
    id: updaterKCM
    title: i18n("System update")

    property var imaupdateSettings: IMAUpdateSettings

    Kirigami.Page {
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: 20

            Rectangle {
                Layout.fillWidth: true
                height: 100
                color: Kirigami.Theme.backgroundColor
                radius: 10
                border.color: Kirigami.Theme.textColor

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Label {
                        text: i18n("Current Version")
                        Layout.fillWidth: true
                        font.bold: true
                    }
                    Label {
                        text: imaupdateSettings.currentVersion
                        Layout.fillWidth: true
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 100
                color: Kirigami.Theme.backgroundColor
                radius: 10
                border.color: Kirigami.Theme.textColor
                visible: imaupdateSettings.shouldShowInfo

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Label {
                        text: i18n("Latest Version")
                        Layout.fillWidth: true
                        font.bold: true
                    }
                    Label {
                        text: imaupdateSettings.latestVersion
                        Layout.fillWidth: true
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Kirigami.Theme.backgroundColor
                radius: 10
                border.color: Kirigami.Theme.textColor
                visible: imaupdateSettings.shouldShowInfo

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Label {
                        text: i18n("Changelog:")
                        font.bold: true
                    }

                    TextArea {
                        id: changelogArea
                        text: imaupdateSettings.changelog
                        wrapMode: Text.Wrap
                        readOnly: true
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }

            Button {
                id: checkUpdateButton
                text: i18n("Check for updates")
                Layout.alignment: Qt.AlignHCenter
                visible: !imaupdateSettings.shouldShowInfo
                onClicked: {
                    imaupdateSettings.checkForUpdates()
                    console.log("Current Version:", imaupdateSettings.currentVersion)
                    console.log("Calling ima-update for update check")
                }
            }
            Button {
                id: applyUpdateButton
                text: i18n("Apply update")
                Layout.alignment: Qt.AlignHCenter
                visible: imaupdateSettings.shouldShowInfo
                onClicked: {
                    console.log("Get trolled")
                }
            }
            Button {
                id: updateFinishedButton
                text: i18n("Reboot")
                Layout.alignment: Qt.AlignHCenter
                visible: imaupdateSettings.isOnStage2Reboot
                onClicked: {
                    console.log("Get trolled")
                }
            }
        }
    }
}
