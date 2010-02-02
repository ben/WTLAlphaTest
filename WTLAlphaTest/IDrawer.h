#include <vector>
#include <uianimation.h>
#include <atltypes.h>
#pragma once

class IDrawer
{
public:
	typedef std::vector< CComPtr<IUIAnimationVariable> > VarVector;

	virtual void Initialize(HWND hwnd_I)=0;
	virtual void Update(IUIAnimationVariable *alpha_I, const VarVector &vars_I)=0;
	virtual void UpdateSize(WTL::CRect rect_I)=0;
};
