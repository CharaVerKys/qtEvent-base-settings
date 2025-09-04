#include <QCoreApplication>
#include "settings.h"
#include "settingsReceiverTemplate.h"
#include "settingsSenderTemplate.h"
#include <QTimer>

void* mainThreadId = nullptr;

Q_DECLARE_METATYPE(id_t)

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Settings::getGlobInstance()->loadSettings();
    
    SettingsReceiver receiver;
    SettingsSender sender;

    static_assert(fit_settingsSignalSlot<SettingsReceiver>::value);
    Settings::getGlobInstance()->registerObjectAsSettingsChangedEventHandler<SettingsReceiver>(&receiver);

    // btw sender can be also receiver, if you implement both logic-ends in one class

    QTimer::singleShot(100,[&sender]{
        bool res = sender.sendSettings(); // id 0
        assert(res);
    });
    QTimer::singleShot(500,[&sender]{
        bool res = sender.sendSettings(); // id 1 and quit
        assert(res);
    });

    qRegisterMetaType<id_t>("id_t");
    return a.exec();
}
