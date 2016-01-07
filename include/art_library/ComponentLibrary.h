#pragma once

#include <map>
#include <string>

#include "components/ComponentFactories.h"

class ComponentLibrary {
protected:
    std::map<std::string, ComponentFactory*> component_factories_;

public:
    ComponentLibrary() {
        component_factories_ = std::map<std::string, ComponentFactory*>();
    }
    ~ComponentLibrary() {
        for(auto iter = component_factories_.begin(); iter!=component_factories_.end();) {
            delete iter->second;
            iter = component_factories_.erase(iter); //c++11 override of map.erase()
            //no iter++ because that would skip elements
        }
        //for some reason valgrind still whines however all ComponentFactory* 's are now deleted
    }

    void AddFactory(std::string name, ComponentFactory* factory) {
        auto exists = component_factories_.find(name);
        if(exists != component_factories_.end()) {
            delete component_factories_[name];
            component_factories_.erase(exists);
        }
        component_factories_.insert(std::pair<std::string, ComponentFactory*>(name, factory));
        return;
    }

    ComponentFactory* Search(std::string component_type) {
        return component_factories_.at(component_type);
    }
};
