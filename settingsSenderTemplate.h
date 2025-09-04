#include <QObject>

class SettingsSender : public QObject{
    Q_OBJECT
public:
    bool sendSettings();
    // this func can be hidden in awaiter, then with coroutine it even one function interface
    void receiveSettingsRes(id_t id, bool success, const char* moduleName = nullptr, const char* paramName = nullptr);
private:
    id_t m_id;
    bool lockingSendingNew = false;
};

/*
awaiter :QObject, suspend_always{
Q_OBJECT....
public:
    awaiter(id_t id):id(id){}
    void await_suspend(Handle h){
        handle = h;
        connect(....); // see callback..
        INVOKE_ARGS(...changeSettings...) // my macros for better syntax, if want one - see my some-repeatable-code repo
    }
    settingsReturnType await_resume(){
        return returnValue;
    }
    settingsReturnType returnValue;
    Handle handle;
    id_t id;
    void receiveSettingsResult(...){
        if(id == expected){....}
        returnValue = ...
        handle.resume()
        disconnect(settings, ... ,this, ...)
    }
}

*/