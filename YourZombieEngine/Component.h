#ifndef COMPONENT_H
#define COMPONENT_H
#pragma once

#include <vector>

class Component {
private:
protected:
public:
	/*Component();
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnCollision();
	virtual void OnRender();
	virtual void OnRenderDebug();
	virtual void OnDisable();
	virtual void OnEnable();
	~Component();*/
};

namespace Components {
	template<typename DerivedType> class Register {
	public:
		Register();
	};
}

#endif // !COMPONENT_H