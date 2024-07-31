/**
 * SPDX-FileCopyrightText: 2024 Your Name <your.email@domain.com>
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
    Q_PROPERTY(bool shouldShowInfo READ shouldShowInfo NOTIFY shouldShowInfoChanged) // HACK: Make KCM disable certain elements till advised so

public:
    explicit IMAUpdateSettings(QObject *parent = nullptr, const KPluginMetaData &data = KPluginMetaData());

    QString currentVersion() const;
    QString latestVersion() const;
    QString changelog() const;
    bool updateInProgress() const;
    bool shouldShowInfo() const;

    Q_INVOKABLE void checkForUpdates();

Q_SIGNALS:
    void currentVersionChanged();
    void latestVersionChanged();
    void changelogChanged();
    void updateInProgressChanged();
    void shouldShowInfoChanged();

private:
    QString readOSVersion() const;
    void fetchUpdateInfo();

    QString m_currentVersion;
    QString m_latestVersion;
    QString m_changelog;
    bool m_updateInProgress;
    bool m_updateAvailable;
    bool m_shouldShowInfo;

    QProcess *m_updateProcess;
};

#endif // IMAUPDATESETTINGS_H
