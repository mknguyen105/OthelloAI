// DynamicFactory.hpp

#ifndef DYNAMICFACTORY_HPP
#define DYNAMICFACTORY_HPP

#include <algorithm>
#include <map>
#include <memory>
#include <typeindex>
#include <vector>
#include "RegisteredType.hpp"
#include "UnregisteredNameException.hpp"
#include "UnregisteredTypeException.hpp"
#include "DuplicateTypeRegistrationException.hpp"



namespace ics46 { namespace factory
{
    template <typename BaseType>
    class DynamicFactory
    {
    public:
        static DynamicFactory<BaseType>& instance();

    public:
        const std::vector<std::shared_ptr<RegisteredType<BaseType>>>& allRegisteredTypes() const;

        template <typename ObjectType, typename... ConstructorArgs>
        void registerType(const std::string& name, ConstructorArgs... constructorArgs);

        template <typename ObjectType>
        std::unique_ptr<BaseType> make() const;

        std::unique_ptr<BaseType> make(const std::string& name) const;

    private:
        DynamicFactory() { }

    private:
        std::vector<std::shared_ptr<RegisteredType<BaseType>>> registeredTypes;
        std::map<std::type_index, std::shared_ptr<RegisteredType<BaseType>>> registeredTypesByType;
        std::map<std::string, std::shared_ptr<RegisteredType<BaseType>>> registeredTypesByName;
    };



    template <typename BaseType>
    DynamicFactory<BaseType>& DynamicFactory<BaseType>::instance()
    {
        static DynamicFactory<BaseType> factoryInstance;
        return factoryInstance;
    }


    template <typename BaseType>
    const std::vector<std::shared_ptr<RegisteredType<BaseType>>>& DynamicFactory<BaseType>
        ::allRegisteredTypes() const
    {
        return registeredTypes;
    }


    template <typename BaseType>
    template <typename ObjectType, typename... ConstructorArgs>
    void DynamicFactory<BaseType>::registerType(
        const std::string& name, ConstructorArgs... constructorArgs)
    {
        static_assert(
            std::is_base_of<BaseType, ObjectType>::value,
            "DynamicFactory registration requires that the object type is derived from the factory's base type");

        auto it = registeredTypesByType.find(std::type_index(typeid(ObjectType)));

        if (it == registeredTypesByType.end())
        {
            std::shared_ptr<RegisteredType<BaseType>> registeredType =
                std::make_shared<RegisteredType<BaseType>>(
                    name, typeid(ObjectType),
                    [=]()
                    {
                        return std::unique_ptr<ObjectType>{
                            new ObjectType{std::forward<ConstructorArgs>(constructorArgs)...}};
                    });

            registeredTypes.push_back(registeredType);
            registeredTypesByType[std::type_index(typeid(ObjectType))] = registeredType;
            registeredTypesByName[name] = registeredType;
        }
        else
        {
            throw DuplicateTypeRegistrationException{typeid(ObjectType)};
        }
    }


    template <typename BaseType>
    template <typename ObjectType>
    std::unique_ptr<BaseType> DynamicFactory<BaseType>::make() const
    {
        auto it = registeredTypesByType.find(std::type_index(typeid(ObjectType)));

        if (it != registeredTypesByType.end())
        {
            return it->second->build();
        }
        else
        {
            throw UnregisteredTypeException{typeid(ObjectType)};
        }
    }


    template <typename BaseType>
    std::unique_ptr<BaseType> DynamicFactory<BaseType>::make(const std::string& name) const
    {
        auto it = registeredTypesByName.find(name);

        if (it != registeredTypesByName.end())
        {
            return it->second->build();
        }
        else
        {
            throw UnregisteredNameException(name);
        }
    }
} }



#include "DynamicFactoryRegistration.hpp"



#endif // DYNAMICFACTORY_HPP

