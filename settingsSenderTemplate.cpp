#include "settingsSenderTemplate.h"

#include <QDebug>
#include <QCoreApplication>

#include <settings.h>
#include <module1.h>

bool SettingsSender::sendSettings(){
    if(lockingSendingNew){return false;}
    lockingSendingNew = true;
    auto pair = Settings::getGlobInstance()->getModule1();
    auto module = static_cast<module1*>(pair.setModule);
    {
        auto lock = std::lock_guard(*pair.mutex);
        module->value = 1;
    }
    QObject::connect(Settings::getGlobInstance(),&Settings::settingsChangeResult,this,&SettingsSender::receiveSettingsRes);
    m_id = Settings::getNewId();
    QMetaObject::invokeMethod(Settings::getGlobInstance(),"changeSettings",Qt::QueuedConnection,Q_ARG(id_t,m_id));
    return true;
}
void SettingsSender::receiveSettingsRes(id_t id, bool success, const char* moduleName , const char* paramName ){
    if(m_id not_eq id){return;}
    lockingSendingNew = false;
    QObject::disconnect(Settings::getGlobInstance(),&Settings::settingsChangeResult,this,&SettingsSender::receiveSettingsRes);
    // your handle logic for values
    // in demo its quit
    qDebug() << id;
    if(id==0){return;}
    QCoreApplication::quit();
}
