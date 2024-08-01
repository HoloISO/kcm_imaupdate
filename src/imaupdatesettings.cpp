/**
 * SPDX-FileCopyrightText: 2024 Adam Jafarov <thevakhovske@petalmail.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */


#include "imaupdatesettings.h"
#include <KPluginFactory>
#include <QFile>
#include <QTextStream>
#include <QDebug>

static QObject *imaupdateSettingsSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new IMAUpdateSettings(nullptr, KPluginMetaData());
}

static void registerTypes()
{
    qmlRegisterSingletonType<IMAUpdateSettings>("org.example.imaupdatesettings", 1, 0, "IMAUpdateSettings", imaupdateSettingsSingletonProvider);
}

K_PLUGIN_FACTORY_WITH_JSON(IMAUpdateSettingsFactory, "kcm_imaupdate.json", registerPlugin<IMAUpdateSettings>();)

IMAUpdateSettings::IMAUpdateSettings(QObject *parent, const KPluginMetaData &data)
: KQuickConfigModule(parent, data),
m_updateInProgress(false),
m_updateAvailable(false),
m_shouldShowInfo(false),
m_shouldShowCurrentInfo(true),
m_updateProgress(0),
m_updateProcess(new QProcess(this)),
m_applyUpdateProcess(new QProcess(this))
{
    registerTypes();

    m_currentVersion = readOSVersion();

    connect(m_updateProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus) {
                if (exitCode == 70) {
                    fetchUpdateInfo();
                    m_shouldShowInfo = true;
                    Q_EMIT shouldShowInfoChanged();
                } else {
                    m_latestVersion.clear();
                    m_changelog.clear();
                    m_updateAvailable = false;
                    m_updateInProgress = false;
                    Q_EMIT updateInProgressChanged();
                    Q_EMIT latestVersionChanged();
                    Q_EMIT changelogChanged();
                }
            });

    connect(m_applyUpdateProcess, &QProcess::readyReadStandardOutput, this, &IMAUpdateSettings::handleUpdateOutput);
}

QString IMAUpdateSettings::readOSVersion() const
{
    QFile file(QLatin1String("/etc/ima-release"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith(QLatin1String("OS_TAG="))) {
                return line.mid(QStringLiteral("OS_TAG=").length());
            }
        }
    }
    return QStringLiteral("Unknown");
}

void IMAUpdateSettings::fetchUpdateInfo()
{
    QFile file(QLatin1String("/tmp/ima-update-temparg"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open update info file.";
        m_latestVersion.clear();
        m_changelog.clear();
        m_updateAvailable = false;
        m_updateInProgress = false;
        Q_EMIT updateInProgressChanged();
        Q_EMIT latestVersionChanged();
        Q_EMIT changelogChanged();
        return;
    }

    QTextStream in(&file);
    QString line;
    bool updateAvailable = false;

    while (!in.atEnd()) {
        line = in.readLine();
        if (line.startsWith(QStringLiteral("OS_TAG_NAME="))) {
            m_latestVersion = line.mid(QStringLiteral("OS_TAG_NAME=").length());
            qWarning() << "Update version: " << m_latestVersion;
            updateAvailable = true;
        } else if (line.startsWith(QStringLiteral("CHGLOG="))) {
            m_changelog = line.mid(QStringLiteral("CHGLOG=").length());
        }
    }

    file.close();

    m_updateAvailable = updateAvailable;
    m_updateInProgress = false;
    Q_EMIT updateInProgressChanged();
    Q_EMIT latestVersionChanged();
    Q_EMIT changelogChanged();
}

QString IMAUpdateSettings::currentVersion() const
{
    return m_currentVersion;
}

QString IMAUpdateSettings::latestVersion() const
{
    return m_updateAvailable ? m_latestVersion : QString();
}

QString IMAUpdateSettings::changelog() const
{
    return m_updateAvailable ? m_changelog : QString();
}

bool IMAUpdateSettings::updateInProgress() const
{
    return m_updateInProgress;
}

bool IMAUpdateSettings::shouldShowInfo() const
{
    return m_shouldShowInfo;
}

bool IMAUpdateSettings::shouldShowCurrentInfo() const
{
    return m_shouldShowCurrentInfo;
}

QString IMAUpdateSettings::updateStatus() const
{
    return m_updateStatus;
}

int IMAUpdateSettings::updateProgress() const
{
    return m_updateProgress;
}

void IMAUpdateSettings::checkForUpdates()
{

    QString program = QStringLiteral("/usr/bin/ima-update");
    QStringList arguments;
    arguments << QStringLiteral("check") << QStringLiteral("--settingsapp");

    m_updateProcess->setProgram(program);
    m_updateProcess->setArguments(arguments);
    m_updateProcess->start();
}

void IMAUpdateSettings::startUpdateChain()
{
    QString program = QStringLiteral("/usr/bin/ima-update");
    QStringList arguments;
    arguments << QStringLiteral("apply-now") << QStringLiteral("--settingsapp");

    m_shouldShowInfo = false;
    Q_EMIT shouldShowInfoChanged();

    m_shouldShowCurrentInfo = false;
    Q_EMIT shouldShowCurrentInfoChanged();

    m_updateInProgress = true;
    Q_EMIT updateInProgressChanged();
    m_updateProgress = 0;
    m_updateStatus = QStringLiteral("Starting update...");
    Q_EMIT updateProgressChanged();
    Q_EMIT updateStatusChanged();

    m_applyUpdateProcess->setProgram(program);
    m_applyUpdateProcess->setArguments(arguments);
    m_applyUpdateProcess->start();
}

void IMAUpdateSettings::handleUpdateOutput()
{
    while (m_applyUpdateProcess->canReadLine()) {
        QString line = QString::fromUtf8(m_applyUpdateProcess->readLine()).trimmed();
        if (line.startsWith(QStringLiteral("CMSG="))) {
            m_updateStatus = line.mid(QStringLiteral("CMSG=").length());
            Q_EMIT updateStatusChanged();
        } else if (line.startsWith(QStringLiteral("CPROGRESS="))) {
            bool ok;
            int progress = line.mid(QStringLiteral("CPROGRESS=").length()).toInt(&ok);
            if (ok) {
                m_updateProgress = progress;
                Q_EMIT updateProgressChanged();
            }
        }
    }
}

#include "imaupdatesettings.moc"
