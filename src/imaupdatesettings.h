/**
 * SPDX-FileCopyrightText: 2024 Adam Jafarov <thevakhovske@petalmail.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef IMAUPDATESETTINGS_H
#define IMAUPDATESETTINGS_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <KQuickConfigModule>
#include <QQmlEngine>

class IMAUpdateSettings : public KQuickConfigModule
{
    Q_OBJECT
    Q_PROPERTY(QString currentVersion READ currentVersion NOTIFY currentVersionChanged)
    Q_PROPERTY(QString latestVersion READ latestVersion NOTIFY latestVersionChanged)
    Q_PROPERTY(QString changelog READ changelog NOTIFY changelogChanged)
    Q_PROPERTY(bool updateInProgress READ updateInProgress NOTIFY updateInProgressChanged)
    Q_PROPERTY(bool shouldShowInfo READ shouldShowInfo NOTIFY shouldShowInfoChanged)
    Q_PROPERTY(bool shouldShowCurrentInfo READ shouldShowCurrentInfo NOTIFY shouldShowCurrentInfoChanged)
    Q_PROPERTY(QString updateStatus READ updateStatus NOTIFY updateStatusChanged)
    Q_PROPERTY(int updateProgress READ updateProgress NOTIFY updateProgressChanged)
    Q_PROPERTY(bool updateFailed READ updateFailed NOTIFY updateFailedChanged)
    Q_PROPERTY(bool updateCompleted READ updateCompleted NOTIFY updateCompletedChanged)
    Q_PROPERTY(bool checkingUpdates READ checkingUpdates NOTIFY checkingUpdatesChanged)
    Q_PROPERTY(bool upToDate READ upToDate NOTIFY upToDateChanged)

public:
    explicit IMAUpdateSettings(QObject *parent = nullptr, const KPluginMetaData &data = KPluginMetaData());

    QString currentVersion() const;
    QString latestVersion() const;
    QString changelog() const;
    bool updateInProgress() const;
    bool shouldShowInfo() const;
    bool shouldShowCurrentInfo() const;
    QString updateStatus() const;
    int updateProgress() const;
    bool updateFailed() const;
    bool updateCompleted() const;
    bool checkingUpdates() const;
    bool upToDate() const;

    Q_INVOKABLE void checkForUpdates();
    Q_INVOKABLE void startUpdateChain();
    Q_INVOKABLE void handleReboot();

Q_SIGNALS:
    void currentVersionChanged();
    void latestVersionChanged();
    void changelogChanged();
    void updateInProgressChanged();
    void shouldShowInfoChanged();
    void shouldShowCurrentInfoChanged();
    void updateStatusChanged();
    void updateProgressChanged();
    void updateFailedChanged();
    void updateCompletedChanged();
    void checkingUpdatesChanged();
    void upToDateChanged();

private:
    QString readOSVersion() const;
    void fetchUpdateInfo();
    void handleUpdateOutput();

    QString m_currentVersion;
    QString m_latestVersion;
    QString m_changelog;
    bool m_updateInProgress;
    bool m_updateAvailable;
    bool m_shouldShowCurrentInfo;
    bool m_shouldShowInfo;
    bool m_updateFailed;
    bool m_updateCompleted;
    bool m_checkingUpdates;
    bool m_upToDate;
    QString m_updateStatus;
    int m_updateProgress;

    QProcess *m_updateProcess;
    QProcess *m_applyUpdateProcess;
    QProcess *m_rebootSeq;
};

#endif // IMAUPDATESETTINGS_H
