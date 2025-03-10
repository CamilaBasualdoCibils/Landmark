#pragma once
#include <pch.h>
#include <ECS/Defines.hpp>
#include "SceneObject.hpp"
class IComponentData;

struct EntityData : SceneObject
{
	static const inline int MAX_NAME_LENGTH = 256;
	static const inline int MAX_COMPONENT_COUNT = 32;
	private:
	std::array<char,MAX_NAME_LENGTH> name;
	//std::string name = "Default Entity Name";
	bool Enabled = true;
	std::array<ComponentTypeID,MAX_COMPONENT_COUNT> components = {0};
	size_t component_count = 0;
	
	public:
	EntityData(SceneID s_id,ObjectID _id,const std::string& _Name = "") :SceneObject(s_id,_id) {
		SetName(_Name);
	}

	const decltype(name)& GetName() const {return name;}
	void SetName(const std::string& _name) {std::copy_n(_name.c_str(), _name.size()+1, name.begin());}
	bool GetEnabled() const {return Enabled;}
	void SetEnabled(bool s) {Enabled = s;}
	constexpr std::span<const ComponentTypeID> GetComponents() const {
		 std::span<const ComponentTypeID> span(components.data(),MAX_COMPONENT_COUNT);
		return span;}
		void RegisterComponent(ComponentTypeID type_id);
		void DeregisterComponent(ComponentTypeID type_id);
		bool Has_Component(ComponentTypeID type_id);
	
	

};

