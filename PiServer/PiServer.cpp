#include <PiServer.h>

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    udpServer PiUdpCoreServer;
    XmlReader *reader = XmlReader::get_instance();
    return 0;
}
