#ifndef DEFINES_H
#define DEFINES_H

enum class SettingsModulesNames{
    NotSetted = 0,
    Module1,
    Module2,
};

//your checkThread, if want to, see my some-repeatable-code if want my impl
#define checkThread(val) (void*)val
extern void* mainThreadID;

#endif // DEFINES_H
