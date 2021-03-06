#pragma once
#include "Component.h"

class Transform final : public Component<Transform>
{
public:
	Transform(class Context* context, class GameObject* owner);
	~Transform();

	Transform(const Transform& rhs);
	Transform& operator=(const Transform&) = delete;

	void Update() override;

	//Local transform mutator and accessors
	void SetLocalScale(const Vector3 vec) { mLocalScale = vec; SetUpdateFlag(); }
	void SetLocalRotation(const Quaternion quat) { mLocalRotation = quat; SetUpdateFlag(); }
	void SetLocalPosition(const Vector3 vec) { mLocalPosition = vec; SetUpdateFlag(); }
	const Vector3& GetLocalScale() const { return mLocalScale; }
	const Quaternion& GetLocalRotation() const { return mLocalRotation; }
	const Vector3& GetLocalPosition() const { return mLocalPosition; }

	//World transform mutators and accessors
	void SetScale(const Vector3 vec);
	void SetRotation(const Quaternion quat);
	void SetPosition(const Vector3 vec);
	const Vector3 GetScale();
	const Quaternion GetRotation();
	const Vector3 GetPosition();

	//Matrix accessors
	const Matrix& GetWorldTransform() { Update(); return mWorldMatrix; }
	const Matrix& GetLocalTransform() { Update(); return mLocalMatrix; }

	//Transform accessors and mutators
	void RotateAxis(Vector3 axis, float angle);
	void RotateEuler(Vector3 angles);
	void SetEulerRotation(Vector3 angles);
	const Vector3 GetEulerRotation();
	void Translate(Vector3 translation);

	//Directional vectors
	const Vector3 GetForward() { return Vector3::Transform(Vector3(0, 0, 1), GetRotation()); }
	const Vector3 GetRight() { return Vector3::Transform(Vector3(1, 0, 0), GetRotation()); }
	const Vector3 GetUp() { return Vector3::Transform(Vector3(0, 1, 0), GetRotation()); }
	
	//Hierarchy accessors and mutators
	const bool HasParent() const { return mParent ? true : false; }
	const bool HasChild() const { return !mChildren.empty(); }
	void SetParent(Transform* parent);
	Transform* GetParent() const { return mParent; }
	const size_t GetChildCount() const { return mChildren.size(); }
	void AddChild(Transform* child) { child->SetParent(this); }
	Transform* GetChild(size_t index) const { return index < mChildren.size() ? mChildren[index] : nullptr;}

	const int GetTest() { return 0; }
	void SetTest(const int a) { return; }

private:
	void RemoveChild(Transform* child);
	void SetUpdateFlag();

private:
	Vector3		mLocalScale;
	Quaternion	mLocalRotation;
	Vector3		mLocalPosition;
	Matrix		mLocalMatrix;
	Matrix		mWorldMatrix;
	
	bool		mIsChanged;
	Transform*	mParent;
	std::vector<Transform*>	mChildren;
};