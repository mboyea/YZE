#ifndef SCRIPT_H
#define SCRIPT_H
#pragma once

#include <set>
#include "Resource.h"
#include "Entities.h"
#include <vector>
#include <string>
#include "Collision.h"

namespace Scripts {
	typedef int TypeID;
	const TypeID INVALID_TYPE = -1;
}

class Script {
private:
	static const std::string name;
	bool didCallStart;
public:
	int depth;
	virtual Scripts::TypeID GetVirtualTypeID() {
		return Scripts::INVALID_TYPE;
	}
	static constexpr Scripts::TypeID GetTypeID() {
		return Scripts::INVALID_TYPE;
	}
	virtual std::string GetVirtualName() {
		return name;
	}
	static std::string GetName() {
		return name;
	}
	Script() : depth(0), didCallStart(false) {}
	virtual void Start(Entities::Index entity) {}
	void CallStart(Entities::Index entity) {
		if (!didCallStart) {
			Start(entity);
			didCallStart = true;
		}
	};
	virtual std::set<Resource>& GetRequiredResources(std::set<Resource>& resourcesOut);
	virtual void Update(Entities::Index entity) {}
	virtual void OnCollision(Entities::Index entity, Entities::Index rhs, Vector2i impulse) {}
	virtual void LateUpdate(Entities::Index entity) {}
	virtual void Render(Entities::Index entity) {}
	~Script() {}
};

namespace Scripts {
	inline std::vector<Script*(*)()> typeArray;
	template<typename T> class Register {
	public:
		const Scripts::TypeID typeID;
		Register() : typeID(typeArray.size()) {
			typeArray.push_back([]()->Script* { return new T(); });
		}
	};
	inline Script* NewScript(TypeID id) {
		return typeArray[id]();
	}
	template<typename T> T* NewScript() {
		return static_cast<T*>(NewScript(T::GetTypeID()));
	}
	template<typename T> TypeID TypeToID() {
		return T::GetTypeID();
	}
	inline TypeID GetTypeCount() {
		return typeArray.size();
	}
}

#define INIT_SCRIPT public:	\
virtual Scripts::TypeID GetVirtualTypeID() override {	\
return typeID;	\
}	\
static constexpr Scripts::TypeID GetTypeID() {	\
return typeID;	\
}	\
virtual std::string GetVirtualName() override {	\
return name;	\
}	\
static std::string GetName() {	\
return name;	\
}	\
private:	\
static const Scripts::TypeID typeID;	\
static const std::string name

#define REGISTER_SCRIPT(c) static Scripts::Register<##c> Registered_##c;	\
const Scripts::TypeID c::typeID = Registered_##c.typeID;	\
const std::string c::name = #c

#endif // !SCRIPT_H