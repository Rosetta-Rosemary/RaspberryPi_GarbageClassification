#ifndef _FILESERVICE_H_
#define _FILESERVICE_H_

#include <iostream>
#include <fstream>
#include <QtCore>
#include <QString>
#include <QWidget>
#include <mutex>
#include <QFile>
#include <QFileDialog>

#include <LogService.h>
#include <KeywordAnalsys.h>

class FileService : public QObject
{
    Q_OBJECT
public:
    static FileService * get_instance()
    {
        if (instance == nullptr)
        {
            instance = new FileService;
        }
        return instance;
    };
    ~FileService();
    bool copyPictureToImage(QString SourceDir);


signals:
    void getFileSuccess(QString str);

private:
    FileService();
    void OpenFileAndDeal();



protected:
    static FileService *instance;

};

#endif