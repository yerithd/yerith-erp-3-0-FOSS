/*
 * yerith-erp-process.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-process.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"

#include "src/process/yerith-erp-process-info.hpp"


#include <QtCore/QThread>


bool YerithERPProcess::_yerith_YRI_DB_RUNTIME_VERIF_ProcessFilesSet(false);

bool YerithERPProcess::_yerithPOSAlertProcessFilesSet(false);


QProcess *YerithERPProcess::_yri_db_runtime_verif_PROCESS(new QProcess);

QProcess *YerithERPProcess::_alertDeamonProcess(new QProcess);


bool YerithERPProcess::START_yri_db_runtime_verif_GUI()
{
    bool GUI_STARTED = false;

    QProcess *GUI_yri_db_runtime_verif_PROCESS = new QProcess;

    if (0 != GUI_yri_db_runtime_verif_PROCESS)
    {
        GUI_yri_db_runtime_verif_PROCESS->setStandardErrorFile("/dev/null", QIODevice::Append);

        GUI_yri_db_runtime_verif_PROCESS->setStandardOutputFile("/dev/null", QIODevice::Append);

        QStringList progArguments;

        GUI_STARTED =
            YerithERPProcess::startDetached(*GUI_yri_db_runtime_verif_PROCESS,
                                            YerithUtils::GET_YRI_DB_RUNTIME_VERIF_EXECUTABLE_FULL_PATH(),
                                            progArguments);
    }

    return GUI_STARTED;
}


bool YerithERPProcess::STOP_yri_db_runtime_verif()
{
	if (!_yerith_YRI_DB_RUNTIME_VERIF_ProcessFilesSet)
	{
		_yri_db_runtime_verif_PROCESS->setStandardErrorFile("/dev/null");

		_yri_db_runtime_verif_PROCESS->setStandardOutputFile("/dev/null");

		_yerith_YRI_DB_RUNTIME_VERIF_ProcessFilesSet = true;
	}

	QString msg;

	QStringList progArguments;

	progArguments << "/usr/bin/systemctl";

	progArguments << "stop";

	progArguments << YerithERPConfig::YRI_DB_RUNTIME_VERIF_SYSTEM_DAEMON_ID;

	//We don't start this process with 'startDetached' so
	//the following call to function 'check_alert_daemon_process()'
	//works fine.

	YerithERPProcess::startAndWaitForFinished(*_yri_db_runtime_verif_PROCESS,
											  "lxqt-sudo",
											  progArguments,
											  60000);

	return _yri_db_runtime_verif_PROCESS->waitForFinished(5000);
}


bool YerithERPProcess::START_yri_db_runtime_verif()
{
    bool started = false;

    if (!_yerith_YRI_DB_RUNTIME_VERIF_ProcessFilesSet)
    {
        _yri_db_runtime_verif_PROCESS->setStandardErrorFile("/dev/null");

        _yri_db_runtime_verif_PROCESS->setStandardOutputFile("/dev/null");

        _yerith_YRI_DB_RUNTIME_VERIF_ProcessFilesSet = true;
    }


    started = YerithProcessInfo::check_Yr_Db_Runtime_Verif_Daemon_Running();


    QString msg;


    if (!started)
    {
        QStringList progArguments;

        progArguments << "/usr/bin/systemctl";

        progArguments << "start";

        progArguments << YerithERPConfig::YRI_DB_RUNTIME_VERIF_SYSTEM_DAEMON_ID;

        started =
            YerithERPProcess::startDetached(*_yri_db_runtime_verif_PROCESS,
                                            "lxqt-sudo",
                                            progArguments);
    }
    else
    {
        msg.clear();

        msg.append(QObject::tr
                   ("Le système de vérification en temps "
                    "d'exécution \"yri-db-runtime-verif\" est "
                    "déjà en marche !"));

        YerithQMessageBox::information(
            YerithUtils::getAllWindows()->_adminWindow,
            QObject::tr("démarrage du système de vérification en temps "
                        "d'exécution - yri-db-runtime-verif"),
            msg);
    }

    return started;
}


bool YerithERPProcess::stopYerithERPAlertProcess()
{
    if (!_yerithPOSAlertProcessFilesSet)
    {
        _alertDeamonProcess->setStandardErrorFile("/dev/null");

        _alertDeamonProcess->setStandardOutputFile("/dev/null");

        _yerithPOSAlertProcessFilesSet = true;
    }

    QString msg;

    QStringList progArguments;

    progArguments << "/usr/bin/systemctl";

    progArguments << "stop";

    progArguments << "yerith-erp-3-0-system-daemon";

    //We don't start this process with 'startDetached' so
    //the following call to function 'check_alert_daemon_process()'
    //works fine.

    YerithERPProcess::startAndWaitForFinished(*_alertDeamonProcess,
                                              "lxqt-sudo",
                                              progArguments, 60000);

    return _alertDeamonProcess->waitForFinished(5000);
}


bool YerithERPProcess::startYerithERPAlertProcess()
{
    if (!_yerithPOSAlertProcessFilesSet)
    {
        _alertDeamonProcess->setStandardErrorFile("/dev/null");

        _alertDeamonProcess->setStandardOutputFile("/dev/null");

        _yerithPOSAlertProcessFilesSet = true;
    }

    bool started = YerithProcessInfo::checkYerithERPAlertDaemonRunning();

    QString msg;

    if (!started)
    {
        QStringList progArguments;

        progArguments << "/usr/bin/systemctl";

        progArguments << "start";

        progArguments << "yerith-erp-3-0-system-daemon";

        started =
        		YerithERPProcess::startDetached(*_alertDeamonProcess,
                                        		"lxqt-sudo",
												progArguments);
    }
    else
    {
        msg.clear();

        msg.append(QObject::tr
                   ("Le système d'alerte \"yerith-erp-3-0-system-daemon\" est déjà en marche !"));

        YerithQMessageBox::information(YerithUtils::getAllWindows()->
                                       _adminWindow,
                                       QObject::
                                       trUtf8
                                       ("démarrage du système d'alerte - yerith-erp-3-0-system-daemon"),
                                       msg);
    }

    return started;
}


int YerithERPProcess::start_PROCESS_AND_READ_PROCESS_output_INTO_FILE(const
                                                                      QString &
                                                                      program_executable_location_full_path,
                                                                      const
                                                                      QString &
                                                                      output_file_full_path,
                                                                      const
                                                                      QStringList
                                                                      &
                                                                      program_executable_args)
{
    QProcess A_YERITH_PROCESS;

    A_YERITH_PROCESS.start(program_executable_location_full_path,
                           program_executable_args);

    if (!A_YERITH_PROCESS.waitForFinished())
    {
        return -1;
    }

    QFile tmpFile(output_file_full_path);

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(A_YERITH_PROCESS.readAllStandardOutput().trimmed());
    }
    else
    {
        return -1;
    }

    int output_file_size = tmpFile.size();

    tmpFile.close();

    return output_file_size;
}


QString YerithERPProcess::compileLatex(QString prefixFileName)
{
    QStringList progArguments;

    progArguments << "-interaction";

    progArguments << "nonstopmode";

    progArguments << QString("%1tex").arg(prefixFileName);

    YerithERPProcess::startAndWaitForFinished(YerithERPConfig::
                                              pathToPdfLatex(), progArguments,
                                              -1);

    QThread::sleep(0.3);

    YerithERPProcess::startAndWaitForFinished(YerithERPConfig::
                                              pathToPdfLatex(), progArguments,
                                              -1);

    QThread::sleep(0.3);

    progArguments.clear();

    QString pdfResultFile = QString("%1pdf").arg(prefixFileName);

    return pdfResultFile;
}


QString YerithERPProcess::compileWITH_LUATEX_Latex(QString prefixFileName)
{
    QStringList progArguments;

    progArguments << "-interaction";

    progArguments << "nonstopmode";

    progArguments << QString("%1tex").arg(prefixFileName);

    YerithERPProcess::startAndWaitForFinished(YerithERPConfig::
                                              pathToLualatex(), progArguments,
                                              -1);
    progArguments.clear();

    QString pdfResultFile = QString("%1pdf").arg(prefixFileName);

    return pdfResultFile;
}


QString YerithERPProcess::startPdfViewerProcess(QString aPDFFileName)
{
    QStringList progArguments(aPDFFileName);

    QProcess::startDetached(YerithERPConfig::pathToPdfReader,
                            progArguments, YerithERPConfig::temporaryFilesDir);

    return aPDFFileName;
}


bool YerithERPProcess::startDetached(QProcess &aProcess,
                                     QString programFileAbsolutePath,
                                     QStringList programArguments)
{
    aProcess.setWorkingDirectory(YerithERPConfig::temporaryFilesDir);

//    QDEBUG_STRING_OUTPUT_2("startDetached; programFileAbsolutePath",
//    					   programFileAbsolutePath);
//
//    qDebug() << "startDetached; programArguments"
//    		 << programArguments;

    return aProcess.startDetached(programFileAbsolutePath, programArguments);
}


bool YerithERPProcess::startAndWaitForFinished(QProcess &aProcess,
                                               QString programFileAbsolutePath,
                                               QStringList programArguments,
                                               int waitForFinished)
{
    aProcess.setWorkingDirectory(YerithERPConfig::temporaryFilesDir);

//    QDEBUG_STRING_OUTPUT_2("startAndWaitForFinished; programFileAbsolutePath",
//    					   programFileAbsolutePath);
//
//    qDebug() << "startAndWaitForFinished; programArguments"
//    		 << programArguments;

    aProcess.start(programFileAbsolutePath, programArguments);

    return aProcess.waitForFinished(waitForFinished);
}


bool YerithERPProcess::startAndWaitForFinished(QString programFileAbsolutePath,
                                               QStringList programArguments,
                                               int waitForFinished)
{
    QProcess aProcess;

    aProcess.setWorkingDirectory(YerithERPConfig::temporaryFilesDir);

    aProcess.start(programFileAbsolutePath, programArguments);

    return aProcess.waitForFinished(waitForFinished);
}
