/**
 * SPDX-FileCopyrightText: 2024 Adam Jafarov <thevakhovske@petalmail.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCMUtils
import org.example.imaupdatesettings 1.0

KCMUtils.SimpleKCM {
    id: updaterKCM
    title: i18n("System Update")

    property var imaupdateSettings: IMAUpdateSettings

    Kirigami.Page {
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 70
                color: Kirigami.Theme.backgroundColor
                radius: 6
                border.color: Kirigami.Theme.textColor
                visible: imaupdateSettings.shouldShowCurrentInfo

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Label {
                        text: i18n("Current Immutarch version")
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
                Layout.preferredHeight: 70
                color: Kirigami.Theme.backgroundColor
                radius: 6
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

            Label {
                visible: imaupdateSettings.checkingUpdates
                text: i18n("Checking for updates...")
                Layout.fillWidth: true
            }

            Label {
                visible: imaupdateSettings.upToDate
                text: i18n("Your installation is up-to-date.")
                Layout.fillWidth: true
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                color: Kirigami.Theme.backgroundColor
                radius: 6
                border.color: Kirigami.Theme.textColor
                visible: imaupdateSettings.updateFailed

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Label {
                        text: i18n("Failed to update")
                        Layout.fillWidth: true
                        font.bold: true
                    }
                    Label {
                        text: i18n("Error occurred. Unable to complete updating this installation with error: \"") + imaupdateSettings.updateStatus + ("\"")
                        Layout.fillWidth: true
                        wrapMode: Text.Wrap
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 70
                color: Kirigami.Theme.backgroundColor
                radius: 6
                border.color: Kirigami.Theme.textColor
                visible: imaupdateSettings.updateCompleted

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Label {
                        text: i18n("Update complete")
                        Layout.fillWidth: true
                        font.bold: true
                    }
                    Label {
                        text: imaupdateSettings.updateStatus
                        Layout.fillWidth: true
                        wrapMode: Text.Wrap
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 70
                color: Kirigami.Theme.backgroundColor
                radius: 6
                border.color: Kirigami.Theme.textColor
                visible: imaupdateSettings.updateInProgress

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Label {
                        text: i18n("Updating to")
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
                radius: 6
                border.color: Kirigami.Theme.textColor
                visible: false

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

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                color: Kirigami.Theme.backgroundColor
                radius: 6
                border.color: Kirigami.Theme.textColor
                visible: imaupdateSettings.updateInProgress

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Label {
                        text: i18n("Update Progress")
                        Layout.fillWidth: true
                        font.bold: true
                    }

                    Label {
                        text: imaupdateSettings.updateStatus
                        Layout.fillWidth: true
                    }

                    ProgressBar {
                        value: imaupdateSettings.updateProgress / 100
                        Layout.fillWidth: true
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignBottom
                spacing: 10

                Button {
                    id: checkUpdateButton
                    text: i18n("Check for updates")
                    Layout.alignment: Qt.AlignHCenter
                    visible: !imaupdateSettings.shouldShowInfo && !imaupdateSettings.checkingUpdates && !imaupdateSettings.updateInProgress && !imaupdateSettings.updateFailed && !imaupdateSettings.updateCompleted
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
                    visible: imaupdateSettings.shouldShowInfo && !imaupdateSettings.updateInProgress
                    onClicked: {
                        imaupdateSettings.startUpdateChain()
                        console.log("Calling ima-update for starting the update")
                    }
                }

                Button {
                    id: applyUpdateAgainButton
                    text: i18n("Apply update again")
                    Layout.alignment: Qt.AlignHCenter
                    visible: imaupdateSettings.updateFailed
                    onClicked: {
                        imaupdateSettings.startUpdateChain()
                        console.log("Calling ima-update for starting the update")
                    }
                }

                Button {
                    id: updateFinishedButton
                    text: i18n("Reboot")
                    Layout.alignment: Qt.AlignHCenter
                    visible: imaupdateSettings.updateCompleted
                    onClicked: {
                        imaupdateSettings.handleReboot()
                    }
                }
            }
        }
    }
}
