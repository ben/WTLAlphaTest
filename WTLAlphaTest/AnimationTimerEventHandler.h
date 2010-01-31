#include <boost/function.hpp>


class AnimTimerEventHandler : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IUIAnimationTimerEventHandler
{
public:
	typedef boost::function<void (void)> callback;

	DECLARE_NOT_AGGREGATABLE(AnimTimerEventHandler)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(AnimTimerEventHandler)
		COM_INTERFACE_ENTRY(IUIAnimationTimerEventHandler)
	END_COM_MAP()

	AnimTimerEventHandler()
		: m_callback()
	{
	}

	void Initialize(callback cb_I)
	{
		m_callback = cb_I;
	}

	// IUIAnimationTimerEventHandler

	__checkReturn HRESULT __stdcall OnPreUpdate()
	{
		return S_OK;
	}

	__checkReturn HRESULT __stdcall OnPostUpdate()
	{
		if (m_callback)
		{
			m_callback();
		}
		return S_OK;
	}

	__checkReturn HRESULT __stdcall OnRenderingTooSlow(UINT32 /* fps */)
	{
		return S_OK;
	}

private:

	callback m_callback;
};
