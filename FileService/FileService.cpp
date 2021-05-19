#include <FileService.h>

FileService * FileService::instance;

FileService::FileService()
{
    connect(this,SIGNAL(getFileSuccess(QString)),
        KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(QString)));
}

FileService::~FileService()
{

}

void FileService::OpenFileAndDeal()
{
    QWidget *W_OpenFile = new QWidget;
    //通过文件对话框显示文件路径
    QString filePath = QFileDialog::getOpenFileName(W_OpenFile);
    if (filePath != "")
    {
        QString Ip = "127.0.0.1";
        QString Port = "26601";
        QString Keyword = "ImageRecognitionTask";
        QString GRE = filePath;
        QString TaskMsg = Ip + " " + Port + " " + Keyword + " " + GRE;
        emit((getFileSuccess(TaskMsg)));
    }
    else
    {

    }
    W_OpenFile->close();
}

bool FileService::copyPictureToImage(QString SourceDir)
{
    QString SinkDir = "./Image.jpeg";
	if (SourceDir == SinkDir)
    {
		return true;
	}
	if (!QFile::exists(SourceDir))
    {
		return false;
	}
	QDir *createfile = new QDir;
	bool exist = createfile->exists(SinkDir);
	if (exist)
    {
		createfile->remove(SinkDir);
	}//end if
 
	if(!QFile::copy(SourceDir, SinkDir))
	{
		return false;
	}
	return true;
}