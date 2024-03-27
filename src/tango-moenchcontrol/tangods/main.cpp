/*----- PROTECTED REGION ID(MoenchControl::main.cpp) ENABLED START -----*/
/* clang-format on */
//=============================================================================
//
// file :        main.cpp
//
// description : C++ source for the MoenchControl device server main.
//               The main rule is to initialise (and create) the Tango
//               system and to create the DServerClass singleton.
//               The main should be the same for every Tango device server.
//
// project :     Tango device for control of MOENCH detector
//
// This file is part of Tango device class.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
//
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================
#include <tango/tango.h>

// Check if crash reporting is used.
#if defined(ENABLE_CRASH_REPORT)
#  include <crashreporting/crash_report.h>
#else
#  define DECLARE_CRASH_HANDLER
#  define INSTALL_CRASH_HANDLER
#endif
#include "sls/Receiver.h"

DECLARE_CRASH_HANDLER

int main(int argc,char *argv[])
{
	INSTALL_CRASH_HANDLER
	Tango::Util *tg;
	try
	{
		// Initialise the device server
		//----------------------------------------
		tg = Tango::Util::init(argc,argv);

		// Create the device server singleton
		//	which will create everything
		//----------------------------------------
		tg->server_init(false);

		// Run the endless loop
		//----------------------------------------
		std::cout << "Ready to accept request" << std::endl;
		tg->server_run();
	}
	catch (std::bad_alloc &)
	{
		std::cout << "Can't allocate memory to store device object !!!" << std::endl;
		std::cout << "Exiting" << std::endl;
	}
	catch (CORBA::Exception &e)
	{
		Tango::Except::print_exception(e);

		std::cout << "Received a CORBA_Exception" << std::endl;
		std::cout << "Exiting" << std::endl;
	}

	if(tg)
	{
		tg->server_cleanup();
	}
	return(0);
}

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	MoenchControl::main.cpp
