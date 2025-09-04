#include "module2.h"
#include <QJsonObject>
#include <qjsondocument.h>

#define MODULENAME_DEFINED "module2"

bool module2::isValidPtr()const{
    VALIDATE_MODULE_PTR_TYPE
    return strong_ordering_equal == this->moduleName.compare(MODULENAME_DEFINED);
}

module2::module2()
{
    moduleEnum = SettingsModulesNames::Module2;
    moduleName = MODULENAME_DEFINED;
}

QJsonObject module2::getJson() const
{
    QJsonObject params;
    params["someConst"] = someConstantState;
    params["someState"] = someChangeableState;
    return params;
}

bool module2::setValuesOnJsonString(const std::string_view jsonStr)
{
    QJsonObject jsonObj;
    if(strong_ordering_equal not_eq jsonStr.compare("prompt_setOnDefault")){
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(jsonStr.data(),jsonStr.length()), &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {return false;}
        jsonObj = jsonDoc.object();
    }

    // * make your get value function, or use one from my some-repeatable-code
    if (jsonObj.contains("someConst") and jsonObj["someConst"].isBool()){
        *const_cast<bool*>(&someConstantState) = jsonObj["someConst"].toBool();
    }else{
        //? any way false in declaration // *const_cast<bool*>(&someConstantState) = false;
    }
    if (jsonObj.contains("someState") and jsonObj["someState"].isBool()){
        someChangeableState = jsonObj["someState"].toBool();
    }else{
        someChangeableState = true;
    }
    return true;
}