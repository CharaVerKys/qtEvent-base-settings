#ifndef IModuleSettings_H
#define IModuleSettings_H

#include <defines.h>
#include <qfile.h>

#define strong_ordering_equal 0
#define VALIDATE_MODULE_PTR_TYPE \
    assert(this); \
    assert(strong_ordering_equal == this->moduleName.compare(MODULENAME_DEFINED));

class IModuleSettings
{
public:
    virtual ~IModuleSettings(){}
    std::string_view getModuleName()const {assert(not moduleName.empty()); return moduleName;}
    SettingsModulesNames getModuleEnum()const {assert(moduleEnum != SettingsModulesNames::NotSetted); return moduleEnum;}

    virtual bool isValidPtr() const = 0;
    bool readFromFile(QFile& stream);
    void setWritePath(std::string const& path); // actually do nothing, only set target path
    virtual QJsonObject getJson() const = 0;
    virtual bool setValuesOnJsonString(std::string_view) = 0;
    bool flush() const; // actual function for write in file

protected:
    std::string_view moduleName;
    SettingsModulesNames moduleEnum = SettingsModulesNames::NotSetted;
    std::string path;
};

#endif // IModuleSettings_H
