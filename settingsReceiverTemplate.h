#include <QObject>

// using id_t = uint32_t;

class SettingsReceiver : public QObject{
Q_OBJECT
signals:
    void settingsChangeResult(id_t id, bool success, const char* moduleName = nullptr, const char* paramName = nullptr);
    
    // you would read constant in init settings, and not in event, cus it is const and not changes in actual runtime

public slots:
    void onSettingsChanged(id_t id);

private:
    int value_some;
    bool state;
    bool option = 1; //(const)
};