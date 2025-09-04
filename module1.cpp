#include "module1.h" 

#include <QJsonObject>
#include <qjsondocument.h>

#define MODULENAME_DEFINED "module1"

bool module1::isValidPtr()const{
    VALIDATE_MODULE_PTR_TYPE
    return strong_ordering_equal == this->moduleName.compare(MODULENAME_DEFINED);
}

module1::module1()
{
    moduleEnum = SettingsModulesNames::Module1;
    moduleName = MODULENAME_DEFINED;
}

QJsonObject module1::getJson() const
{
    QJsonObject params;
    params["value"] = value;
    return params;
}

bool module1::setValuesOnJsonString(const std::string_view jsonStr)
{
    QJsonObject jsonObj;
    if(strong_ordering_equal not_eq jsonStr.compare("prompt_setOnDefault")){
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(jsonStr.data(),jsonStr.length()), &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {return false;}
        jsonObj = jsonDoc.object();
    }

    // * make your get value function, or use one from my some-repeatable-code
    if (jsonObj.contains("value") and jsonObj["value"].isDouble()){
        value = jsonObj["value"].toInt();
    }else{
        value = true;
    }
    return true;
}