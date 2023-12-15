/*----- PROTECTED REGION ID(TestDeviceLuninStateMachine.cpp) ENABLED START -----*/
/* clang-format on */
//=============================================================================
//
// file :        TestDeviceLuninStateMachine.cpp
//
// description : State machine file for the TestDeviceLunin class
//
// project :     Lunin title
//
// This file is part of Tango device class.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the MIT licence.
//
//
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================

#include "TestDeviceLunin.h"

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TestDeviceLunin::TestDeviceLuninStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================


namespace TestDeviceLunin_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : TestDeviceLunin::is_counter_allowed()
 * Description:  Execution allowed for counter attribute
 */
//--------------------------------------------------------
bool TestDeviceLunin::is_counter_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for counter attribute in Write access.
	/*----- PROTECTED REGION ID(TestDeviceLunin::counterStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TestDeviceLunin::counterStateAllowed_WRITE

	//	Not any excluded states for counter attribute in read access.
	/*----- PROTECTED REGION ID(TestDeviceLunin::counterStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TestDeviceLunin::counterStateAllowed_READ
	return true;
}


//=================================================
//		Commands Allowed Methods
//=================================================


/*----- PROTECTED REGION ID(TestDeviceLunin::TestDeviceLuninStateAllowed.AdditionalMethods) ENABLED START -----*/
/* clang-format on */
//	Additional Methods
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TestDeviceLunin::TestDeviceLuninStateAllowed.AdditionalMethods

}	//	End of namespace
