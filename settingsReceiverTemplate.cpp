#include "settingsReceiverTemplate.h"
#include <settings.h>
#include <module1.h>
#include <module2.h>

void SettingsReceiver::onSettingsChanged(id_t id)
{
    ModuleMutexPair module1_pair = Settings::getGlobInstance()->getModule1();
    ModuleMutexPair module2_pair = Settings::getGlobInstance()->getModule2();
    module1* mod1 = static_cast<module1*>(module1_pair.setModule);
    // static means no dynamic, so for better correctness check with isValidPtr() /would be no-op in release, if not check return value/
    mod1->isValidPtr();

                // dynamic is additional runtime..
    module2* mod2 = dynamic_cast<module2*>(module2_pair.setModule);
    if(not mod2){std::abort();}
    if(not mod2->isValidPtr()){std::abort();}

    assert(module1_pair.mutex == module2_pair.mutex);

    { // lock guard
        auto lock = std::lock_guard(*module1_pair.mutex);
        if(mod2->someConstantState){
            if(mod1->value == 0){
                // if(state) {throw std::runtime_error("invalid");}
                if(mod2->someChangeableState){
                    emit settingsChangeResult(id, false,"errorModule","errorReason");
                    return;
                }
            } // if value
        } // if const
        state = mod2->someChangeableState;
        value_some = mod1->value;
    } // lock guard

    emit settingsChangeResult(id, true); // should be send on any path, or you getting dead lock
}