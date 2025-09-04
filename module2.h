#ifndef MODULE2_H
#define MODULE2_H

#include <imodulesettings.h>

class module2 : public IModuleSettings
{
public:
    module2();
    QJsonObject getJson() const final;
    bool setValuesOnJsonString(const std::string_view jsonStr) final;
    bool isValidPtr()const final;

    bool someChangeableState;
    const bool someConstantState = false;
        // ? this marked const for user, we use const cast in implementation to set variable from config
        // ? this is technically UB, but on practice for windows and linux its ok, because const cast on heap memory is ok
        // ? but you cant use instance of this class on constant memory regions, it will be crash
        // ? any way, assumed modules controlled by Settings root class only
};

#endif // MODULE2_H
