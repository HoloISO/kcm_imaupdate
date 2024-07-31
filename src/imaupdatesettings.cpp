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
m_updateProcess(new QProcess(this))
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

void IMAUpdateSettings::checkForUpdates()
{
    m_updateInProgress = true;
    Q_EMIT updateInProgressChanged();

    QString program = QStringLiteral("/usr/bin/ima-update");
    QStringList arguments;
    arguments << QStringLiteral("check") << QStringLiteral("--settingsapp");

    m_updateProcess->setProgram(program);
    m_updateProcess->setArguments(arguments);
    m_updateProcess->start();
}

#include "imaupdatesettings.moc"
