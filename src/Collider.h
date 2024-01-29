#ifndef COLLIDER_H
#define COLLIDER_H
#pragma once

#include <vector>
#include "Vector4i.h"
#include "Vector2i.h"
#include "Collision.h"
#include "Transform.h"

namespace Colliders {
	typedef int TypeID;
	inline const TypeID INVALID_TYPE = -1;
}

class Collider {
public:
	static constexpr Colliders::TypeID GetTypeID() {
		return Colliders::INVALID_TYPE;
	}
	virtual const Colliders::TypeID VirtualGetTypeID() {
		return GetTypeID();
	}
	static constexpr const char* ToString() {
		return str;
	}
	virtual const char* VirtualToString() {
		return ToString();
	}
private:
	static const char str[];
protected:
	// axis-aligned bounding box
	Vector4i aabb;
	virtual Collision DoNarrowCollision(Transform* transform, Collider* target, Transform* targetTransform);
	bool IsBroadColliding(Transform* transform, Collider* target, Transform* targetTransform);
public:
	bool doSolveCollision;
	Collision DoCollision(Transform* transform, Collider* target, Transform* targetTransform);
	const Vector4i& GetAABB() const;
	virtual std::ostream& Serialize(std::ostream& os);
	virtual std::istream& Deserialize(std::istream& is);
};

namespace Colliders {
	inline std::vector<Collider*(*)()> typeArray;
	template<typename T> class Register {
	public:
		const Colliders::TypeID typeID;
		Register() : typeID(typeArray.size()) {
			typeArray.push_back([]()->Collider* { return new T(); });
		}
	};
	inline Collider* New(TypeID id) {
		return typeArray[id]();
	}
	template<typename T> inline T* New() {
		return static_cast<T*>(New(T::GetTypeID()));
	}
	template<typename T> inline TypeID GetTypeID() {
		return T::GetTypeID();
	}
	inline TypeID GetTypeCount() {
		return typeArray.size();
	}
}

#define INIT_COLLIDER public:	\
static constexpr Colliders::TypeID GetTypeID() {	\
return typeID;	\
}	\
virtual const Colliders::TypeID VirtualGetTypeID() override {	\
return GetTypeID();	\
}	\
static constexpr const char* ToString() {	\
return str;	\
}	\
virtual const char* VirtualToString() override {	\
return ToString();	\
}	\
private:	\
static const Colliders::TypeID typeID;	\
static const char str[]

#define REGISTER_COLLIDER(c) static Colliders::Register<c> Registered_##c;	\
const Colliders::TypeID c::typeID = Registered_##c.typeID;	\
const char c::str[] = #c

#endif // !COLLIDER_H
