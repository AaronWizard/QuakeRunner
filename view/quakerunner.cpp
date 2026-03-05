#include "quakerunner.h"
#include "ui_quakerunner.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>

const QString SETTINGS_ORGANIZATION = "AaronWizard";
const QString SETTINGS_APPLICATION = "Quake Runner";

const QString SETTING_QUAKE_PATH = "quakePath";
const QString SETTING_BASE_FOLDER = "baseFolder";
const QString SETTING_MOD_INDEX = "modIndex";

const QString BROWSE_CAPTION_QUAKE_PATH = "Select Quake executable";
const QString BROWSE_CAPTION_BASE_FOLDER = "Select Base Folder";

const QString MOD_ID1 = "id1";

const QString COMMAND_ARGUMENT_BASEDIR = "-basedir";
const QString COMMAND_ARGUMENT_GAME = "-game";

QuakeRunner::QuakeRunner(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QuakeRunner)
    , quakeProcess(new QProcess(this))
{
    ui->setupUi(this);

    initFields();
    connect(quakeProcess, &QProcess::finished, this, &QuakeRunner::quakeProcessFinished);
}

QuakeRunner::~QuakeRunner()
{
    delete ui;
}

void QuakeRunner::on_btnBrowseQuakePath_clicked()
{
    browseForPath(*(ui->txtQuakePath), false, BROWSE_CAPTION_QUAKE_PATH, QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

void QuakeRunner::on_txtQuakePath_textChanged(const QString &quakePath)
{
    QFileInfo quake(quakePath);
    if (quake.exists() && quake.isFile())
    {
        saveSetting(SETTING_QUAKE_PATH, quakePath);
        updateRunEnabled();
    }
}

void QuakeRunner::on_btnBrowseBaseFolder_clicked()
{
    browseForPath(*(ui->txtBaseFolder), true, BROWSE_CAPTION_BASE_FOLDER, QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

void QuakeRunner::on_txtBaseFolder_textChanged(const QString &baseFolderPath)
{
    QDir baseFolder(baseFolderPath);
    if (baseFolder.exists())
    {
        saveSetting(SETTING_BASE_FOLDER, baseFolderPath);
        updateRunEnabled();
        updateModsCombo();
    }
}

void QuakeRunner::on_comboMod_currentIndexChanged(int index)
{
    if (!stillLoading)
    {
        saveSetting(SETTING_MOD_INDEX, index);
    }
}

void QuakeRunner::on_btnRun_clicked()
{
    ui->btnRun->setEnabled(false);

    QString quakePath = ui->txtQuakePath->text();
    QStringList arguments = getQuakeArguments();

    quakeProcess->start(quakePath, arguments);
}

void QuakeRunner::initFields()
{
    stillLoading = true;

    ui->txtQuakePath->setText(getStringSetting(SETTING_QUAKE_PATH));
    ui->txtBaseFolder->setText(getStringSetting(SETTING_BASE_FOLDER));
    ui->comboMod->setCurrentIndex(getIntSetting(SETTING_MOD_INDEX));

    stillLoading = false;
}

void QuakeRunner::quakeProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    ui->btnRun->setEnabled(true);
}

void QuakeRunner::browseForPath(QLineEdit &field, const bool pathIsFolder, const QString &caption, const QString &startDir)
{
    QString path;
    if (pathIsFolder)
    {
        path = QFileDialog::getExistingDirectory(this, caption, startDir, QFileDialog::Option::DontResolveSymlinks);
    }
    else
    {
        path = QFileDialog::getOpenFileName(this, caption, startDir, QString(), nullptr, QFileDialog::Option::DontResolveSymlinks);
    }

    field.setText(path);
}

void QuakeRunner::updateRunEnabled()
{
    QFileInfo quakePath(ui->txtQuakePath->text());
    QDir baseFolder(ui->txtBaseFolder->text());

    bool canRun = quakePath.exists() && quakePath.isFile() && baseFolder.exists();

    ui->btnRun->setEnabled(canRun);
}

void QuakeRunner::updateModsCombo()
{
    ui->comboMod->clear();
    ui->comboMod->addItem("");

    QDir baseFolder(ui->txtBaseFolder->text());

    QFileInfoList mods = baseFolder.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    for (const QFileInfo &mod : std::as_const(mods))
    {
        if (mod.fileName() != MOD_ID1)
        {
            ui->comboMod->addItem(mod.fileName());
        }
    }
}

const QStringList QuakeRunner::getQuakeArguments() const
{
    QStringList result = {};

    QString baseFolder = ui->txtBaseFolder->text();
    if (!baseFolder.isEmpty())
    {
        result << COMMAND_ARGUMENT_BASEDIR << baseFolder;
    }

    QString mod = ui->comboMod->currentText();
    if (!mod.isEmpty())
    {
        result << COMMAND_ARGUMENT_GAME << mod;
    }

    return result;
}

QString QuakeRunner::getStringSetting(const QString &settingName) const
{
    QSettings settings(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
    return settings.value(settingName).toString();
}

int QuakeRunner::getIntSetting(const QString &settingName) const
{
    QSettings settings(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
    return settings.value(settingName).toInt();
}

void QuakeRunner::saveSetting(QAnyStringView settingName, QVariant settingValue)
{
    QSettings settings(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
    settings.setValue(settingName, settingValue);
}

