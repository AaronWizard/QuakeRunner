#ifndef QUAKERUNNER_H
#define QUAKERUNNER_H

#include <QWidget>
#include <QtWidgets/QLineEdit>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class QuakeRunner;
}
QT_END_NAMESPACE

class QuakeRunner : public QWidget
{
    Q_OBJECT

public:
    QuakeRunner(QWidget *parent = nullptr);
    ~QuakeRunner();

private slots:
    void on_btnBrowseQuakePath_clicked();
    void on_txtQuakePath_textChanged(const QString &arg1);

    void on_btnBrowseBaseFolder_clicked();
    void on_txtBaseFolder_textChanged(const QString &baseFolderPath);

    void on_comboMod_currentIndexChanged(int index);

    void on_btnRun_clicked();

    void quakeProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::QuakeRunner *ui;
    QProcess *quakeProcess;

    bool stillLoading;

    void initFields();

    void browseForPath(QLineEdit &field, const bool pathIsFolder, const QString &caption, const QString &startDir);
    void updateRunEnabled();
    void updateModsCombo();
    const QStringList getQuakeArguments() const;

    QString getStringSetting(const QString &settingName) const;
    int getIntSetting(const QString &settingName) const;
    void saveSetting(QAnyStringView settingName, QVariant settingValue);
};
#endif // QUAKERUNNER_H
