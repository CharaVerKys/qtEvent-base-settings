#ifndef SETTINGS_H
#define SETTINGS_H

#include <mutex>
#include <QEvent>
#include <QObject>
#include "defines.h"
#include "imodulesettings.h"


/*
## guideline:
1. init settings, noting special
2. settings-setter (aka setter) want to change settings
3. he remember old setting params and push new settings to module and emit signal change settings
4. setter blocking next settings-send event (or support multiple settings events indexes) and wait for answer
5. Settings:: invoke onSettingsChanged on settings-receivers (aka receivers)
6. receivers validate input settings and if pass returns true, or if validate fails return false + module name and reason
7. setter receive success and (if he want) 'flush()' module to write settings to file
7.5 if received fail then revers settings and emit changeSettings again, to restore last valid state
*/

// !!!!!!!!!!!!!!
#define numOfModules 2
// !!!!!!!!!!!!!!

static_assert(std::is_same_v<id_t,std::uint32_t>,"define own id_t = uint32_t; or idk, change every id_t to your one");

// type check
// c++17, if you use c++20 you can replace this with concept
template<typename T>
class fit_settingsSignalSlot
{
private:
    template<typename U>
    static auto testSignal(int) -> decltype(std::declval<U>().settingsChangeResult(
                                          std::declval<id_t>(),
                                          std::declval<bool>(),
                                            std::declval<const char*>(),
                                            std::declval<const char*>()
                                          ), std::true_type{});
    template<typename U>
    static auto testSlot(int) -> decltype(std::declval<U>().onSettingsChanged(std::declval<id_t>()), std::true_type{});
    //comma operator btw

    template<typename>
    static std::false_type testSignal(...);
    template<typename>
    static std::false_type testSlot(...);

public:
    static constexpr bool value = decltype(testSignal<T>(0))::value and decltype(testSlot<T>(0))::value;
};
// type check

struct changeResult;

struct ModuleMutexPair{
    IModuleSettings* setModule; // point of your module
    std::mutex* mutex; // global settings access mutex
};

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;
    
    ~Settings();

    static Settings* getGlobInstance();
    bool loadSettings();

    ////////////////////////////
    template<typename QObject_typename>
    std::enable_if_t<fit_settingsSignalSlot<QObject_typename>::value, void>
    registerObjectAsSettingsChangedEventHandler(QObject_typename *object)
    {
        std::lock_guard lock(*mutex);
        assert(object);
        eventSetChangedReceivers.push_back(object);
        connect(object, &QObject_typename::settingsChangeResult, this, &Settings::resultFromObject, Qt::QueuedConnection);
    }

    template<typename QObject_typename>
    std::enable_if_t<!fit_settingsSignalSlot<QObject_typename>::value, void>
    registerObjectAsSettingsChangedEventHandler(QObject_typename *object)
    {
        (void)object;
        static_assert(fit_settingsSignalSlot<QObject_typename>::value, "Object does not have the required signal {settingsChangeResult} or slot {onSettingsChanged}");
    }
////////////////////////////

    ModuleMutexPair getModule1();
    ModuleMutexPair getModule2();

    static id_t getNewId(){
        static std::atomic<id_t> idGen = 0;
        return idGen++;
    }

signals:
    void settingsChangeResult(id_t id, bool success, const char* moduleName = nullptr, const char* paramName = nullptr);

public slots:
    void changeSettings(id_t id);

private slots:
    void resultFromObject(id_t id, bool success, const char* moduleName = nullptr, const char* paramName = nullptr);

private:
    Settings();
    ModuleMutexPair createReturnSetPair(IModuleSettings* modSet) noexcept;
    void setPathsForSettings() noexcept;
    std::string getSettingsVariant(bool forSave, const std::string& moduleName) noexcept;
    void initHolderSetModules() noexcept;

private:
    std::map<id_t, uint16_t> transactionObjectsRemain;
    std::map<id_t, changeResult> transactionResult;
    std::map<SettingsModulesNames, IModuleSettings*> allModules;
    std::mutex* mutex = nullptr;
    std::vector<QObject*> eventSetChangedReceivers;
    std::atomic<bool> loaded = false;

    std::array<std::unique_ptr<IModuleSettings>, numOfModules> holderSetModules;
}; // end of class Settings

struct changeResult{
    bool success = true; const char *moduleName = nullptr; const char *paramName = nullptr;
};

#undef numOfModules
#endif // SETTINGS_H
