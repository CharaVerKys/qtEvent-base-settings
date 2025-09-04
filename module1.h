#ifndef MODULE1_H
#define MODULE1_H

#include <imodulesettings.h>

class module1 : public IModuleSettings
{
public:
    module1();
    QJsonObject getJson() const final;
    bool setValuesOnJsonString(const std::string_view jsonStr) final;
    bool isValidPtr() const final;

    int value;
};

#endif // MODULE1_H
