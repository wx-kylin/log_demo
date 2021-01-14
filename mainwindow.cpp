#include "mainwindow.h"

#include <qlogging.h>
#include <QDebug>
#include <QTime>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

#define DOUBLE 2
#define MAX_FILE_SIZE 1024
#define LOG_FILE0 "demo_0.log"
#define LOG_FILE1 "demo_1.log"
#define LOG_FILE_PATH "/.cache/log_demo/log"

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray currentTime = QTime::currentTime().toString().toLocal8Bit();

    QString name[DOUBLE] = {LOG_FILE0, LOG_FILE1};
    FILE *log_file = nullptr;
    QString logFilePath;
    int fileSize;
    static int i = 0;
    QDir dir;
    bool flag = 0;

    logFilePath = QDir::homePath() + LOG_FILE_PATH;
    if (dir.mkpath(logFilePath)) {
        flag = 1;
    }

    if (flag) {
        logFilePath = logFilePath + "/" + name[i];
        log_file = fopen(logFilePath.toLocal8Bit().constData(), "a+");
    }

    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        if (!log_file) {
            break;
        }
        fprintf(log_file, "Debug: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(log_file? log_file: stdout, "Info: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(log_file? log_file: stderr, "Warning: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(log_file? log_file: stderr, "Critical: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(log_file? log_file: stderr, "Fatal: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    }

    if (log_file) {
        fileSize = ftell(log_file);
        if (fileSize >= MAX_FILE_SIZE) {
            i = (i + 1) % DOUBLE;
            logFilePath = QDir::homePath() + LOG_FILE_PATH + "/" + name[i];
            if (QFile::exists(logFilePath)) {
                QFile temp(logFilePath);
                temp.remove();
            }
        }
        fclose(log_file);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    int i = 0;
    qInstallMessageHandler(messageOutput);
    QString logFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    qDebug() << "==================================logFilePath is " << logFilePath;
    while(i < 30) {
        qDebug() << i++;
    }
}

MainWindow::~MainWindow()
{
}

