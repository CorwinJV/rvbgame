/*-----------------------------------------------------------------------------------------------------------------------------------------*

    BitEngine DS is (c) 2006 - 2009 Bitfield GmbH, Berlin, Germany. All rights reserved. Unauthorized distribution of this file or the 
    accompanied libraries and source code is prohibited and punishable by law. For more information, please write to contact@bitfield.de.

    This file refers to confidential information that is (c) 2003-2006 NINTENDO. This file and the accompanied libraries and source code 
    may therefore only be used by properly licensed Nintendo DS developers.
    
    http://www.bitfield.de

 *-----------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef GLUTFRAMEWORK_FUNCTIONPOINTER_H_INCLUDED
#define GLUTFRAMEWORK_FUNCTIONPOINTER_H_INCLUDED

//-----------------------------------------------------------------------------------------------------------------------------------------
/*
	The following code is based on the article 

	"CALLBACKS IN C++ USING TEMPLATE FUNCTORS" 
	von Rich Hickey (1994)
	http://www.tutok.sk/fastgl/callback.html
*/

#include <string.h>
typedef unsigned short u16;

namespace BE
{
	
//-----------------------------------------------------------------------------------------------------------------------------------------
/**
	\class		CFunctionPointerBase
	
	\brief		base class for function pointers
	
	
	enthält bereits alle Datenmember, d.h. Pointer auf Objekt und Pointer auf Funktion
	enthält diese Daten in TYPLOSER Form, da der Besitzer des Pointers zur Kompilezeit nicht weiss,
	wohin der Pointer zur Laufzeit zeigen wird
*/

class CFunctionPointerBase
{
public:
	typedef void (CFunctionPointerBase::*_MemFunc)();	///< generische Definition eines Memberfunction-Pointers
	typedef void (*_Func)();							///< generische Definition eines (statischen) Function-Pointers
	
	union {
		const void *m_fpStaticFunction;					///< statischer Functionpointer
		char m_fpMemberFunction[sizeof(_MemFunc)];		///< Memberfunctionpointer - muss als char[] gespeichert werden, void* geht nicht
	};

	void* m_pxObject;									///< Pointer auf Objekt, auf dem Memberfunktion gerufen wird (kann 0 sein)

	/// default - ctor
	CFunctionPointerBase() :  m_pxObject(0) {}

	/// ctor
	CFunctionPointerBase(void* pxObject, void* fpFunction, u16 iFunctionPointerSize) :  
		m_pxObject(pxObject)
	{
		if(pxObject)
		{
			memcpy(m_fpMemberFunction, fpFunction, iFunctionPointerSize);
		}
		else
		{
			m_fpStaticFunction = fpFunction;
		}
	}
} __declspec(align(4)); //__attribute__ ((aligned(4)));


//-----------------------------------------------------------------------------------------------------------------------------------------
// 0 arguments, no return value
//-----------------------------------------------------------------------------------------------------------------------------------------

class CFunctionPointer0 : public CFunctionPointerBase
{
public:
    typedef void (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer0() : m_fpTranslatorFunction(0) {}

	CFunctionPointer0(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	void operator()() const
	{
		m_fpTranslatorFunction(this);
	}
};

template<typename TObject, typename TFunction>
class CFunctionPointer0MemberFunctionTranslator : public CFunctionPointer0
{
public:
	CFunctionPointer0MemberFunctionTranslator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer0(pxObject, (void*) &function, sizeof(function), Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		(pObject->*memFunc)();
	}
};

template<typename TFunction>
class CFunctionPointer0StaticFunctionTranslator : public CFunctionPointer0
{
public:
	CFunctionPointer0StaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer0(0, function, 0, Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData)
	{
		(TFunction(pCallbackData->m_fpStaticFunction))();
	}
};

template<typename TObject>
inline
CFunctionPointer0MemberFunctionTranslator<TObject, void (TObject::*)()>
CreateFunctionPointer0(TObject* pxObject, void (TObject::*fpFunction)())
{
    return CFunctionPointer0MemberFunctionTranslator<TObject, void (TObject::*)()>(pxObject, fpFunction);
}

template<typename TObject>
inline
CFunctionPointer0MemberFunctionTranslator<TObject, void (TObject::*)() const>
CreateFunctionPointer0(TObject* pxObject, void (TObject::*fpFunction)() const)
{
    return CFunctionPointer0MemberFunctionTranslator<TObject, void (TObject::*)() const>(pxObject, fpFunction);
}

inline
CFunctionPointer0StaticFunctionTranslator<void (*)()>
CreateFunctionPointer0(void (*fpFunction)())
{
    return CFunctionPointer0StaticFunctionTranslator<void (*)()>(fpFunction);
}


//-----------------------------------------------------------------------------------------------------------------------------------------
// 0 arguments, with return value
//-----------------------------------------------------------------------------------------------------------------------------------------

template<typename TReturn>
class CFunctionPointer0R : public CFunctionPointerBase
{
public:
    typedef TReturn (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer0R() : m_fpTranslatorFunction(0)  {}

	CFunctionPointer0R(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	TReturn operator()() const
	{
		return m_fpTranslatorFunction(this);
	}
};

template<typename TReturn, typename TObject, typename TFunction>
class CFunctionPointer0RTranslator : public CFunctionPointer0R<TReturn>
{
public:
	CFunctionPointer0RTranslator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer0R<TReturn>(pxObject, (void*) &function, sizeof(function), Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		return (pObject->*memFunc)();
	}
};

template<typename TReturn, typename TFunction>
class CFunctionPointer0RStaticFunctionTranslator : public CFunctionPointer0R<TReturn>
{
public:
	CFunctionPointer0RStaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer0R<TReturn>(0, function, 0, Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData)
	{
		return (TFunction(pCallbackData->m_fpStaticFunction))();
	}
};

template<typename TObject, typename TReturn>
inline
CFunctionPointer0RTranslator<TReturn, TObject, TReturn (TObject::*)()>
CreateFunctionPointer0R(TObject* pxObject, TReturn (TObject::*fpFunction)())
{
    return CFunctionPointer0RTranslator<TReturn, TObject, TReturn (TObject::*)()>(pxObject, fpFunction);
}

template<typename TObject, typename TReturn>
inline
CFunctionPointer0RTranslator<TReturn, TObject, TReturn (TObject::*)() const>
CreateFunctionPointer0R(TObject* pxObject, TReturn (TObject::*fpFunction)() const)
{
    return CFunctionPointer0RTranslator<TReturn, TObject, TReturn (TObject::*)() const>(pxObject, fpFunction);
}

template<typename TReturn>
inline
CFunctionPointer0RStaticFunctionTranslator<TReturn, TReturn (*)()>
CreateFunctionPointer0R(TReturn (*fpFunction)())
{
    return CFunctionPointer0RStaticFunctionTranslator<TReturn, TReturn (*)()>(fpFunction);
}



//-----------------------------------------------------------------------------------------------------------------------------------------
// 1 argument, no return value
//-----------------------------------------------------------------------------------------------------------------------------------------


template<typename TArg0>
class CFunctionPointer1 : public CFunctionPointerBase
{
public:
    typedef void (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData, TArg0 arg0);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer1() : m_fpTranslatorFunction(0)  {}

	CFunctionPointer1(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	void operator()(TArg0 arg0) const
	{
		m_fpTranslatorFunction(this, arg0);
	}
};

template<typename TArg0, typename TObject, typename TFunction>
class CFunctionPointer1Translator : public CFunctionPointer1<TArg0>
{
public:
	CFunctionPointer1Translator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer1<TArg0>(pxObject, (void*) &function, sizeof(function), Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		(pObject->*memFunc)(arg0);
	}
};

template<typename TArg0, typename TFunction>
class CFunctionPointer1StaticFunctionTranslator : public CFunctionPointer1<TArg0>
{
public:
	CFunctionPointer1StaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer1<TArg0>(0, function, 0, Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0)
	{
		(TFunction(pCallbackData->m_fpStaticFunction))(arg0);
	}
};

template<typename TObject, typename TArg0>
inline
CFunctionPointer1Translator<TArg0, TObject, void (TObject::*)(TArg0)>
CreateFunctionPointer1(TObject* pxObject, void (TObject::*fpFunction)(TArg0))
{
    return CFunctionPointer1Translator<TArg0, TObject, void (TObject::*)(TArg0)>(pxObject, fpFunction);
}

template<typename TObject, typename TArg0>
inline
CFunctionPointer1Translator<TArg0, TObject, void (TObject::*)(TArg0) const>
CreateFunctionPointer1(TObject* pxObject, void (TObject::*fpFunction)(TArg0) const)
{
    return CFunctionPointer1Translator<TArg0, TObject, void (TObject::*)(TArg0) const>(pxObject, fpFunction);
}

template<typename TArg0>
inline
CFunctionPointer1StaticFunctionTranslator<TArg0, void (*)(TArg0)>
CreateFunctionPointer1(void (*fpFunction)(TArg0))
{
    return CFunctionPointer1StaticFunctionTranslator<TArg0, void (*)(TArg0)>(fpFunction);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// 1 argument, with return value
//-----------------------------------------------------------------------------------------------------------------------------------------

template<typename TReturn, typename TArg0>
class CFunctionPointer1R : public CFunctionPointerBase
{
public:
    typedef TReturn (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData, TArg0 arg0);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer1R() : m_fpTranslatorFunction(0)  {}

	CFunctionPointer1R(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	TReturn operator()(TArg0 arg0) const
	{
		return m_fpTranslatorFunction(this, arg0);
	}
};

template<typename TReturn, typename TArg0, typename TObject, typename TFunction>
class CFunctionPointer1RTranslator : public CFunctionPointer1R<TReturn, TArg0>
{
public:
	CFunctionPointer1RTranslator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer1R<TReturn, TArg0>(pxObject, (void*) &function, sizeof(function), Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		return (pObject->*memFunc)(arg0);
	}
};

template<typename TReturn, typename TArg0, typename TFunction>
class CFunctionPointer1RStaticFunctionTranslator : public CFunctionPointer1R<TReturn, TArg0>
{
public:
	CFunctionPointer1RStaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer1R<TReturn, TArg0>(0, function, 0, Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0)
	{
		return (TFunction(pCallbackData->m_fpStaticFunction))(arg0);
	}
};

template<typename TObject, typename TReturn, typename TArg0>
inline
CFunctionPointer1RTranslator<TReturn, TArg0, TObject, TReturn (TObject::*)(TArg0)>
CreateFunctionPointer1R(TObject* pxObject, TReturn (TObject::*fpFunction)(TArg0))
{
    return CFunctionPointer1RTranslator<TReturn, TArg0, TObject, TReturn (TObject::*)(TArg0)>(pxObject, fpFunction);
}

template<typename TObject, typename TReturn, typename TArg0>
inline
CFunctionPointer1RTranslator<TReturn, TArg0, TObject, TReturn (TObject::*)(TArg0) const>
CreateFunctionPointer1R(TObject* pxObject, TReturn (TObject::*fpFunction)(TArg0) const)
{
    return CFunctionPointer1RTranslator<TReturn, TArg0, TObject, TReturn (TObject::*)(TArg0) const>(pxObject, fpFunction);
}

template<typename TReturn, typename TArg0>
inline
CFunctionPointer1RStaticFunctionTranslator<TReturn, TArg0, TReturn (*)(TArg0)>
CreateFunctionPointer1R(TReturn (*fpFunction)(TArg0))
{
    return CFunctionPointer1RStaticFunctionTranslator<TReturn, TArg0, TReturn (*)(TArg0)>(fpFunction);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// 2 argument, no return value
//-----------------------------------------------------------------------------------------------------------------------------------------

template<typename TArg0, typename TArg1>
class CFunctionPointer2 : public CFunctionPointerBase
{
public:
    typedef void (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer2() : m_fpTranslatorFunction(0)  {}

	CFunctionPointer2(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	void operator()(TArg0 arg0, TArg1 arg1) const
	{
		m_fpTranslatorFunction(this, arg0, arg1);
	}
};

template<typename TArg0, typename TArg1, typename TObject, typename TFunction>
class CFunctionPointer2Translator : public CFunctionPointer2<TArg0, TArg1>
{
public:
	CFunctionPointer2Translator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer2<TArg0, TArg1>(pxObject, (void*) &function, sizeof(function), Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		(pObject->*memFunc)(arg0, arg1);
	}
};

template<typename TArg0, typename TArg1, typename TFunction>
class CFunctionPointer2StaticFunctionTranslator : public CFunctionPointer2<TArg0, TArg1>
{
public:
	CFunctionPointer2StaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer2<TArg0, TArg1>(0, function, 0, Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1)
	{
		(TFunction(pCallbackData->m_fpStaticFunction))(arg0, arg1);
	}
};

template<typename TObject, typename TArg0, typename TArg1>
inline
CFunctionPointer2Translator<TArg0, TArg1, TObject, void (TObject::*)(TArg0, TArg1)>
CreateFunctionPointer2(TObject* pxObject, void (TObject::*fpFunction)(TArg0, TArg1))
{
    return CFunctionPointer2Translator<TArg0, TArg1, TObject, void (TObject::*)(TArg0, TArg1)>(pxObject, fpFunction);
}

template<typename TObject, typename TArg0, typename TArg1>
inline
CFunctionPointer2Translator<TArg0, TArg1, TObject, void (TObject::*)(TArg0, TArg1) const>
CreateFunctionPointer2(TObject* pxObject, void (TObject::*fpFunction)(TArg0, TArg1) const)
{
    return CFunctionPointer2Translator<TArg0, TArg1, TObject, void (TObject::*)(TArg0, TArg1) const>(pxObject, fpFunction);
}

template<typename TArg0, typename TArg1>
inline
CFunctionPointer2StaticFunctionTranslator<TArg0, TArg1, void (*)(TArg0, TArg1)>
CreateFunctionPointer2(void (*fpFunction)(TArg0, TArg1))
{
    return CFunctionPointer2StaticFunctionTranslator<TArg0, TArg1, void (*)(TArg0, TArg1)>(fpFunction);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// 2 arguments, with return value
//-----------------------------------------------------------------------------------------------------------------------------------------

template<typename TReturn, typename TArg0, typename TArg1>
class CFunctionPointer2R : public CFunctionPointerBase
{
public:
    typedef TReturn (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer2R() : m_fpTranslatorFunction(0)  {}

	CFunctionPointer2R(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	TReturn operator()(TArg0 arg0, TArg1 arg1) const
	{
		return m_fpTranslatorFunction(this, arg0, arg1);
	}
};

template<typename TReturn, typename TArg0, typename TArg1, typename TObject, typename TFunction>
class CFunctionPointer2RTranslator : public CFunctionPointer2R<TReturn, TArg0, TArg1>
{
public:
	CFunctionPointer2RTranslator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer2R<TReturn, TArg0, TArg1>(pxObject, (void*) &function, sizeof(function), Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		return (pObject->*memFunc)(arg0, arg1);
	}
};

template<typename TReturn, typename TArg0, typename TArg1, typename TFunction>
class CFunctionPointer2RStaticFunctionTranslator : public CFunctionPointer2R<TReturn, TArg0, TArg1>
{
public:
	CFunctionPointer2RStaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer2R<TReturn, TArg0, TArg1>(0, function, 0, Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1)
	{
		return (TFunction(pCallbackData->m_fpStaticFunction))(arg0, arg1);
	}
};

template<typename TObject, typename TReturn, typename TArg0, typename TArg1>
inline
CFunctionPointer2RTranslator<TReturn, TArg0, TArg1, TObject, TReturn (TObject::*)(TArg0, TArg1)>
CreateFunctionPointer2R(TObject* pxObject, TReturn (TObject::*fpFunction)(TArg0, TArg1))
{
    return CFunctionPointer2RTranslator<TReturn, TArg0, TArg1, TObject, TReturn (TObject::*)(TArg0, TArg1)>(pxObject, fpFunction);
}

template<typename TObject, typename TReturn, typename TArg0, typename TArg1>
inline
CFunctionPointer2RTranslator<TReturn, TArg0, TArg1, TObject, TReturn (TObject::*)(TArg0, TArg1) const>
CreateFunctionPointer2R(TObject* pxObject, TReturn (TObject::*fpFunction)(TArg0, TArg1) const)
{
    return CFunctionPointer2RTranslator<TReturn, TArg0, TArg1, TObject, TReturn (TObject::*)(TArg0, TArg1) const>(pxObject, fpFunction);
}

template<typename TReturn, typename TArg0, typename TArg1>
inline
CFunctionPointer2RStaticFunctionTranslator<TReturn, TArg0, TArg1, TReturn (*)(TArg0, TArg1)>
CreateFunctionPointer2R(TReturn (*fpFunction)(TArg0, TArg1))
{
    return CFunctionPointer2RStaticFunctionTranslator<TReturn, TArg0, TArg1, TReturn (*)(TArg0, TArg1)>(fpFunction);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// 3 argument, no return value
//-----------------------------------------------------------------------------------------------------------------------------------------

template<typename TArg0, typename TArg1, typename TArg2>
class CFunctionPointer3 : public CFunctionPointerBase
{
public:
    typedef void (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer3() : m_fpTranslatorFunction(0)  {}

	CFunctionPointer3(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	void operator()(TArg0 arg0, TArg1 arg1, TArg2 arg2) const
	{
		m_fpTranslatorFunction(this, arg0, arg1, arg2);
	}
};

template<typename TArg0, typename TArg1, typename TArg2, typename TObject, typename TFunction>
class CFunctionPointer3Translator : public CFunctionPointer3<TArg0, TArg1, TArg2>
{
public:
	CFunctionPointer3Translator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer3<TArg0, TArg1, TArg2>(pxObject, (void*) &function, sizeof(function), Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		(pObject->*memFunc)(arg0, arg1, arg2);
	}
};

template<typename TArg0, typename TArg1, typename TArg2, typename TFunction>
class CFunctionPointer3StaticFunctionTranslator : public CFunctionPointer3<TArg0, TArg1, TArg2>
{
public:
	CFunctionPointer3StaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer3<TArg0, TArg1, TArg2>(0, function, 0, Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2)
	{
		(TFunction(pCallbackData->m_fpStaticFunction))(arg0, arg1, arg2);
	}
};

template<typename TObject, typename TArg0, typename TArg1, typename TArg2>
inline
CFunctionPointer3Translator<TArg0, TArg1, TArg2, TObject, void (TObject::*)(TArg0, TArg1, TArg2)>
CreateFunctionPointer3(TObject* pxObject, void (TObject::*fpFunction)(TArg0, TArg1, TArg2))
{
    return CFunctionPointer3Translator<TArg0, TArg1, TArg2, TObject, void (TObject::*)(TArg0, TArg1, TArg2)>(pxObject, fpFunction);
}

template<typename TObject, typename TArg0, typename TArg1, typename TArg2>
inline
CFunctionPointer3Translator<TArg0, TArg1, TArg2, TObject, void (TObject::*)(TArg0, TArg1, TArg2) const>
CreateFunctionPointer3(TObject* pxObject, void (TObject::*fpFunction)(TArg0, TArg1, TArg2) const)
{
    return CFunctionPointer3Translator<TArg0, TArg1, TArg2, TObject, void (TObject::*)(TArg0, TArg1, TArg2) const>(pxObject, fpFunction);
}

template<typename TArg0, typename TArg1, typename TArg2>
inline
CFunctionPointer3StaticFunctionTranslator<TArg0, TArg1, TArg2, void (*)(TArg0, TArg1, TArg2)>
CreateFunctionPointer3(void (*fpFunction)(TArg0, TArg1, TArg2))
{
    return CFunctionPointer3StaticFunctionTranslator<TArg0, TArg1, TArg2, void (*)(TArg0, TArg1, TArg2)>(fpFunction);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// 3 arguments, with return value
//-----------------------------------------------------------------------------------------------------------------------------------------

template<typename TReturn, typename TArg0, typename TArg1, typename TArg2>
class CFunctionPointer3R : public CFunctionPointerBase
{
public:
    typedef TReturn (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer3R() : m_fpTranslatorFunction(0)  {}

	CFunctionPointer3R(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	TReturn operator()(TArg0 arg0, TArg1 arg1, TArg2 arg2) const
	{
		return m_fpTranslatorFunction(this, arg0, arg1, arg2);
	}
};

template<typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TObject, typename TFunction>
class CFunctionPointer3RTranslator : public CFunctionPointer3R<TReturn, TArg0, TArg1, TArg2>
{
public:
	CFunctionPointer3RTranslator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer3R<TReturn, TArg0, TArg1, TArg2>(pxObject, (void*) &function, sizeof(function), Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		return (pObject->*memFunc)(arg0, arg1, arg2);
	}
};

template<typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TFunction>
class CFunctionPointer3RStaticFunctionTranslator : public CFunctionPointer3R<TReturn, TArg0, TArg1, TArg2>
{
public:
	CFunctionPointer3RStaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer3R<TReturn, TArg0, TArg1, TArg2>(0, function, 0, Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2)
	{
		return (TFunction(pCallbackData->m_fpStaticFunction))(arg0, arg1, arg2);
	}
};

template<typename TObject, typename TReturn, typename TArg0, typename TArg1, typename TArg2>
inline
CFunctionPointer3RTranslator<TReturn, TArg0, TArg1, TArg2, TObject, TReturn (TObject::*)(TArg0, TArg1, TArg2)>
CreateFunctionPointer3R(TObject* pxObject, TReturn (TObject::*fpFunction)(TArg0, TArg1, TArg2))
{
    return CFunctionPointer3RTranslator<TReturn, TArg0, TArg1, TArg2, TObject, TReturn (TObject::*)(TArg0, TArg1, TArg2)>(pxObject, fpFunction);
}

template<typename TObject, typename TReturn, typename TArg0, typename TArg1, typename TArg2>
inline
CFunctionPointer3RTranslator<TReturn, TArg0, TArg1, TArg2, TObject, TReturn (TObject::*)(TArg0, TArg1, TArg2) const>
CreateFunctionPointer3R(TObject* pxObject, TReturn (TObject::*fpFunction)(TArg0, TArg1, TArg2) const)
{
    return CFunctionPointer3RTranslator<TReturn, TArg0, TArg1, TArg2, TObject, TReturn (TObject::*)(TArg0, TArg1, TArg2) const>(pxObject, fpFunction);
}

template<typename TReturn, typename TArg0, typename TArg1, typename TArg2>
inline
CFunctionPointer3RStaticFunctionTranslator<TReturn, TArg0, TArg1, TArg2, TReturn (*)(TArg0, TArg1, TArg2)>
CreateFunctionPointer3R(TReturn (*fpFunction)(TArg0, TArg1, TArg2))
{
    return CFunctionPointer3RStaticFunctionTranslator<TReturn, TArg0, TArg1, TArg2, TReturn (*)(TArg0, TArg1, TArg2)>(fpFunction);
}


//-----------------------------------------------------------------------------------------------------------------------------------------
// 4 arguments, no return value
//-----------------------------------------------------------------------------------------------------------------------------------------

template<typename TArg0, typename TArg1, typename TArg2, typename TArg3>
class CFunctionPointer4 : public CFunctionPointerBase
{
public:
    typedef void (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2, TArg3 arg3);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer4() : m_fpTranslatorFunction(0)  {}

	CFunctionPointer4(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	void operator()(TArg0 arg0, TArg1 arg1, TArg2 arg2, TArg3 arg3) const
	{
		m_fpTranslatorFunction(this, arg0, arg1, arg2, arg3);
	}
};

template<typename TArg0, typename TArg1, typename TArg2, typename TArg3, typename TObject, typename TFunction>
class CFunctionPointer4Translator : public CFunctionPointer4<TArg0, TArg1, TArg2, TArg3>
{
public:
	CFunctionPointer4Translator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer4<TArg0, TArg1, TArg2, TArg3>(pxObject, (void*) &function, sizeof(function), Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2, TArg3 arg3)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		(pObject->*memFunc)(arg0, arg1, arg2, arg3);
	}
};

template<typename TArg0, typename TArg1, typename TArg2, typename TArg3, typename TFunction>
class CFunctionPointer4StaticFunctionTranslator : public CFunctionPointer4<TArg0, TArg1, TArg2, TArg3>
{
public:
	CFunctionPointer4StaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer4<TArg0, TArg1, TArg2, TArg3>(0, function, 0, Call) {}

    static void Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2, TArg3 arg3)
	{
		(TFunction(pCallbackData->m_fpStaticFunction))(arg0, arg1, arg2, arg3);
	}
};

template<typename TObject, typename TArg0, typename TArg1, typename TArg2, typename TArg3>
inline
CFunctionPointer4Translator<TArg0, TArg1, TArg2, TArg3, TObject, void (TObject::*)(TArg0, TArg1, TArg2, TArg3)>
CreateFunctionPointer4(TObject* pxObject, void (TObject::*fpFunction)(TArg0, TArg1, TArg2, TArg3))
{
    return CFunctionPointer4Translator<TArg0, TArg1, TArg2, TArg3, TObject, void (TObject::*)(TArg0, TArg1, TArg2, TArg3)>(pxObject, fpFunction);
}

template<typename TObject, typename TArg0, typename TArg1, typename TArg2, typename TArg3>
inline
CFunctionPointer4Translator<TArg0, TArg1, TArg2, TArg3, TObject, void (TObject::*)(TArg0, TArg1, TArg2, TArg3) const>
CreateFunctionPointer4(TObject* pxObject, void (TObject::*fpFunction)(TArg0, TArg1, TArg2, TArg3) const)
{
    return CFunctionPointer4Translator<TArg0, TArg1, TArg2, TArg3, TObject, void (TObject::*)(TArg0, TArg1, TArg2, TArg3) const>(pxObject, fpFunction);
}

template<typename TArg0, typename TArg1, typename TArg2, typename TArg3>
inline
CFunctionPointer4StaticFunctionTranslator<TArg0, TArg1, TArg2, TArg3, void (*)(TArg0, TArg1, TArg2, TArg3)>
CreateFunctionPointer4(void (*fpFunction)(TArg0, TArg1, TArg2, TArg3))
{
    return CFunctionPointer4StaticFunctionTranslator<TArg0, TArg1, TArg2, TArg3, void (*)(TArg0, TArg1, TArg2, TArg3)>(fpFunction);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// 4 arguments, with return value
//-----------------------------------------------------------------------------------------------------------------------------------------

template<typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3>
class CFunctionPointer4R : public CFunctionPointerBase
{
public:
    typedef TReturn (* TTranslatorFunctionPointer)(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2, TArg3 arg3);
    TTranslatorFunctionPointer m_fpTranslatorFunction;

	CFunctionPointer4R() : m_fpTranslatorFunction(0)  {}

	CFunctionPointer4R(void* pxObject, void* fpFunction, u16 iFunctionPointerSize, TTranslatorFunctionPointer pxTranslatorFunction) : 
		CFunctionPointerBase(pxObject, fpFunction, iFunctionPointerSize),
		m_fpTranslatorFunction(pxTranslatorFunction) {}

	operator bool() const
	{
		return m_fpTranslatorFunction != 0;
	}

	TReturn operator()(TArg0 arg0, TArg1 arg1, TArg2 arg2, TArg3 arg3) const
	{
		return m_fpTranslatorFunction(this, arg0, arg1, arg2, arg3);
	}
};

template<typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3, typename TObject, typename TFunction>
class CFunctionPointer4RTranslator : public CFunctionPointer4R<TReturn, TArg0, TArg1, TArg2, TArg3>
{
public:
	CFunctionPointer4RTranslator(TObject* pxObject, const TFunction& function) : 
	  CFunctionPointer4R<TReturn, TArg0, TArg1, TArg2, TArg3>(pxObject, (void*) &function, sizeof(function), Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2, TArg3 arg3)
	{
		TObject* pObject = (TObject*) pCallbackData->m_pxObject;
		TFunction &memFunc(*(TFunction*)(void *)(pCallbackData->m_fpMemberFunction));
		return (pObject->*memFunc)(arg0, arg1, arg2, arg3);
	}
};

template<typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3, typename TFunction>
class CFunctionPointer4RStaticFunctionTranslator : public CFunctionPointer4R<TReturn, TArg0, TArg1, TArg2, TArg3>
{
public:
	CFunctionPointer4RStaticFunctionTranslator(const TFunction& function) : 
	  CFunctionPointer4R<TReturn, TArg0, TArg1, TArg2, TArg3>(0, function, 0, Call) {}

    static TReturn Call(const CFunctionPointerBase* pCallbackData, TArg0 arg0, TArg1 arg1, TArg2 arg2, TArg3 arg3)
	{
		return (TFunction(pCallbackData->m_fpStaticFunction))(arg0, arg1, arg2, arg3);
	}
};

template<typename TObject, typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3>
inline
CFunctionPointer4RTranslator<TReturn, TArg0, TArg1, TArg2, TArg3, TObject, TReturn (TObject::*)(TArg0, TArg1, TArg2, TArg3)>
CreateFunctionPointer4R(TObject* pxObject, TReturn (TObject::*fpFunction)(TArg0, TArg1, TArg2, TArg3))
{
    return CFunctionPointer4RTranslator<TReturn, TArg0, TArg1, TArg2, TArg3, TObject, TReturn (TObject::*)(TArg0, TArg1, TArg2, TArg3)>(pxObject, fpFunction);
}

template<typename TObject, typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3>
inline
CFunctionPointer4RTranslator<TReturn, TArg0, TArg1, TArg2, TArg3, TObject, TReturn (TObject::*)(TArg0, TArg1, TArg2, TArg3) const>
CreateFunctionPointer4R(TObject* pxObject, TReturn (TObject::*fpFunction)(TArg0, TArg1, TArg2, TArg3) const)
{
    return CFunctionPointer4RTranslator<TReturn, TArg0, TArg1, TArg2, TArg3, TObject, TReturn (TObject::*)(TArg0, TArg1, TArg2, TArg3) const>(pxObject, fpFunction);
}

template<typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3>
inline
CFunctionPointer4RStaticFunctionTranslator<TReturn, TArg0, TArg1, TArg2, TArg3, TReturn (*)(TArg0, TArg1, TArg2, TArg3)>
CreateFunctionPointer4R(TReturn (*fpFunction)(TArg0, TArg1, TArg2, TArg3))
{
    return CFunctionPointer4RStaticFunctionTranslator<TReturn, TArg0, TArg1, TArg2, TArg3, TReturn (*)(TArg0, TArg1, TArg2, TArg3)>(fpFunction);
}


//-----------------------------------------------------------------------------------------------------------------------------------------

} // namespace BE


//#include "FunctionPointer.inl"

#endif // BASELIB_FUNCTIONPOINTER_H_INCLUDED
