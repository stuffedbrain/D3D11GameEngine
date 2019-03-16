#pragma once
#include "IWidget.h"

class InspectorWidget final : public IWidget
{
public:
	InspectorWidget(class Context* context);
	~InspectorWidget() = delete;

	void Render() override;
};