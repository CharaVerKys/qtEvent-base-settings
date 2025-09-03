#include "imodulesettings.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <fstream>

bool IModuleSettings::readFromFile(QFile& stream){
    if(not stream.isOpen()){
        setValuesOnJsonString("prompt_setOnDefault");
        return false;
    }
    try{
        std::string str = stream.readAll().toStdString();
        setValuesOnJsonString(str.c_str()); // ? string view
        return false;
    } catch (const std::exception & e) {
        qWarning() << (QString("UserMsg Исключение в IModuleSettings::readFromFile: %1").arg(e.what()));
        setValuesOnJsonString("prompt_setOnDefault");
        return false;
    }
}

void IModuleSettings::setWritePath(std::string const& path){
    this->path = path;
}

bool IModuleSettings::flush() const{
    std::ofstream stream(path, std::ios::out);
    if(not stream){
        qCritical()/*Warning?*/ << "UserMsg Failed to open file for writing: " << std::string(this->getModuleName()).c_str() << " error: "<< strerror(errno);
        return false;
    }
    
    auto js = getJson();
    QJsonDocument doc(js);
    errno = 0;
    if (stream << doc.toJson().toStdString(); errno) {
        qCritical() << "UserMsg Failed to write data to file. Error: " << strerror(errno);
        return true;
    }
    qInfo() << "UserMsg Settings saved to file.";
    return false;
}