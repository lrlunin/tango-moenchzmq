/*----- PROTECTED REGION ID(MoenchZMQ.cpp) ENABLED START -----*/
/* clang-format on */
//=============================================================================
//
// file :        MoenchZMQ.cpp
//
// description : C++ source for the MoenchZMQ class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               MoenchZMQ are implemented in this file.
//
// project :     MoenchZMQ processor
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


#include "MoenchZMQ.h"
#include "MoenchZMQClass.h"
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	MoenchZMQ.cpp

/**
 *  MoenchZMQ class description:
 *    
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name    |  Method name
//================================================================
//  State           |  Inherited (no method)
//  Status          |  Inherited (no method)
//  start_receiver  |  start_receiver
//  stop_receiver   |  stop_receiver
//  abort_receiver  |  abort_receiver
//  reset_pedestal  |  reset_pedestal
//================================================================

//================================================================
//  Attributes managed are:
//================================================================
//  file_index        |  Tango::DevULong	Scalar
//  filename          |  Tango::DevString	Scalar
//  file_root_path    |  Tango::DevString	Scalar
//  normalize         |  Tango::DevBoolean	Scalar
//  threshold         |  Tango::DevDouble	Scalar
//  counting_sigma    |  Tango::DevDouble	Scalar
//  live_period       |  Tango::DevLong	Scalar
//  process_pedestal  |  Tango::DevBoolean	Scalar
//  analog_img        |  Tango::DevFloat	Image  ( max = 400 x 400)
//  counting_img      |  Tango::DevFloat	Image  ( max = 400 x 400)
//================================================================

namespace MoenchZMQ_ns
{
/*----- PROTECTED REGION ID(MoenchZMQ::namespace_starting) ENABLED START -----*/
/* clang-format on */
//	static initializations
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::MoenchZMQ()
 * Description:  Constructors for a Tango device
 *                implementing the classMoenchZMQ
 */
//--------------------------------------------------------
MoenchZMQ::MoenchZMQ(Tango::DeviceClass *cl, std::string &s)
 : TANGO_BASE_CLASS(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(MoenchZMQ::constructor_1) ENABLED START -----*/
	/* clang-format on */
	init_device();
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::constructor_1
}
//--------------------------------------------------------
MoenchZMQ::MoenchZMQ(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s)
{
	/*----- PROTECTED REGION ID(MoenchZMQ::constructor_2) ENABLED START -----*/
	/* clang-format on */
	init_device();
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::constructor_2
}
//--------------------------------------------------------
MoenchZMQ::MoenchZMQ(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d)
{
	/*----- PROTECTED REGION ID(MoenchZMQ::constructor_3) ENABLED START -----*/
	/* clang-format on */
	init_device();
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::constructor_3
}
//--------------------------------------------------------
MoenchZMQ::~MoenchZMQ()
{
	delete_device();
}

//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::delete_device()
 * Description:  will be called at device destruction or at init command
 */
//--------------------------------------------------------
void MoenchZMQ::delete_device()
{
	DEBUG_STREAM << "MoenchZMQ::delete_device() " << device_name << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::delete_device) ENABLED START -----*/
	/* clang-format on */
	//	Delete device allocated objects
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::delete_device
	delete[] attr_file_index_read;
	delete[] attr_filename_read;
	delete[] attr_file_root_path_read;
	delete[] attr_normalize_read;
	delete[] attr_threshold_read;
	delete[] attr_counting_sigma_read;
	delete[] attr_live_period_read;
	delete[] attr_process_pedestal_read;
	delete[] attr_analog_img_read;
	delete[] attr_counting_img_read;
}

//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::init_device()
 * Description:  will be called at device initialization.
 */
//--------------------------------------------------------
void MoenchZMQ::init_device()
{
	DEBUG_STREAM << "MoenchZMQ::init_device() create device " << device_name << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::init_device_before) ENABLED START -----*/
	/* clang-format on */
	//	Initialization before get_device_property() call
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::init_device_before


	//	Get the device properties from database
	get_device_property();

	attr_file_index_read = new Tango::DevULong[1];
	attr_filename_read = new Tango::DevString[1];
	attr_file_root_path_read = new Tango::DevString[1];
	attr_normalize_read = new Tango::DevBoolean[1];
	attr_threshold_read = new Tango::DevDouble[1];
	attr_counting_sigma_read = new Tango::DevDouble[1];
	attr_live_period_read = new Tango::DevLong[1];
	attr_process_pedestal_read = new Tango::DevBoolean[1];
	attr_analog_img_read = new Tango::DevFloat[400*400];
	attr_counting_img_read = new Tango::DevFloat[400*400];
	//	No longer if mandatory property not set.
	if (mandatoryNotDefined)
		return;

	/*----- PROTECTED REGION ID(MoenchZMQ::init_device) ENABLED START -----*/
	/* clang-format on */
	//	Initialize device
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::init_device
}

//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::get_device_property()
 * Description:  Read database to initialize property data members.
 */
//--------------------------------------------------------
void MoenchZMQ::get_device_property()
{
	/*----- PROTECTED REGION ID(MoenchZMQ::get_device_property_before) ENABLED START -----*/
	/* clang-format on */
	//	Initialize property data members
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::get_device_property_before

	mandatoryNotDefined = false;

	//	Read device properties from database.
	Tango::DbData	dev_prop;
	dev_prop.push_back(Tango::DbDatum("ZMQ_IP"));
	dev_prop.push_back(Tango::DbDatum("ZMQ_PORT"));

	//	is there at least one property to be read ?
	if (dev_prop.size()>0)
	{
		//	Call database and extract values
		if (Tango::Util::instance()->_UseDb==true)
			get_db_device()->get_property(dev_prop);

		//	get instance on MoenchZMQClass to get class property
		Tango::DbDatum	def_prop, cl_prop;
		MoenchZMQClass	*ds_class =
			(static_cast<MoenchZMQClass *>(get_device_class()));
		int	i = -1;

		//	Try to initialize ZMQ_IP from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  zMQ_IP;
		else {
			//	Try to initialize ZMQ_IP from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  zMQ_IP;
		}
		//	And try to extract ZMQ_IP value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  zMQ_IP;
		//	Property StartDsPath is mandatory, check if has been defined in database.
		check_mandatory_property(cl_prop, dev_prop[i]);

		//	Try to initialize ZMQ_PORT from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  zMQ_PORT;
		else {
			//	Try to initialize ZMQ_PORT from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  zMQ_PORT;
		}
		//	And try to extract ZMQ_PORT value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  zMQ_PORT;

	}

	/*----- PROTECTED REGION ID(MoenchZMQ::get_device_property_after) ENABLED START -----*/
	/* clang-format on */
	//	Check device property data members init
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::get_device_property_after
}
//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::check_mandatory_property()
 * Description:  For mandatory properties check if defined in database.
 */
//--------------------------------------------------------
void MoenchZMQ::check_mandatory_property(Tango::DbDatum &class_prop, Tango::DbDatum &dev_prop)
{
	//	Check if all properties are empty
	if (class_prop.is_empty() && dev_prop.is_empty())
	{
		TangoSys_OMemStream	tms;
		tms << std::endl <<"Property \'" << dev_prop.name;
		if (Tango::Util::instance()->_UseDb==true)
			tms << "\' is mandatory but not defined in database";
		else
			tms << "\' is mandatory but cannot be defined without database";
		append_status(tms.str());
		mandatoryNotDefined = true;
		/*----- PROTECTED REGION ID(MoenchZMQ::check_mandatory_property) ENABLED START -----*/
		/* clang-format on */
		std::cerr << tms.str() << " for " << device_name << std::endl;
		/* clang-format off */
		/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::check_mandatory_property
	}
}


//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::always_executed_hook()
 * Description:  method always executed before any command is executed
 */
//--------------------------------------------------------
void MoenchZMQ::always_executed_hook()
{
	DEBUG_STREAM << "MoenchZMQ::always_executed_hook()  " << device_name << std::endl;
	if (mandatoryNotDefined)
	{
		Tango::Except::throw_exception(
					(const char *)"PROPERTY_NOT_SET",
					get_status().c_str(),
					(const char *)"MoenchZMQ::always_executed_hook()");
	}
	/*----- PROTECTED REGION ID(MoenchZMQ::always_executed_hook) ENABLED START -----*/
	/* clang-format on */
	//	code always executed before all requests
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::read_attr_hardware()
 * Description:  Hardware acquisition for attributes
 */
//--------------------------------------------------------
void MoenchZMQ::read_attr_hardware(TANGO_UNUSED(std::vector<long> &attr_list))
{
	DEBUG_STREAM << "MoenchZMQ::read_attr_hardware(std::vector<long> &attr_list) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_attr_hardware) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_attr_hardware
}
//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::write_attr_hardware()
 * Description:  Hardware writing for attributes
 */
//--------------------------------------------------------
void MoenchZMQ::write_attr_hardware(TANGO_UNUSED(std::vector<long> &attr_list))
{
	DEBUG_STREAM << "MoenchZMQ::write_attr_hardware(std::vector<long> &attr_list) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::write_attr_hardware) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::write_attr_hardware
}

//--------------------------------------------------------
/**
 *	Read attribute file_index related method
 *
 *
 *	Data type:	Tango::DevULong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_file_index(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_file_index(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_file_index) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_file_index_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_file_index
}
//--------------------------------------------------------
/**
 *	Write attribute file_index related method
 *
 *
 *	Data type:	Tango::DevULong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::write_file_index(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::write_file_index(Tango::WAttribute &attr) entering... " << std::endl;
	//	Retrieve write value
	Tango::DevULong	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(MoenchZMQ::write_file_index) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::write_file_index
}
//--------------------------------------------------------
/**
 *	Read attribute filename related method
 *
 *
 *	Data type:	Tango::DevString
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_filename(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_filename(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_filename) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_filename_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_filename
}
//--------------------------------------------------------
/**
 *	Write attribute filename related method
 *
 *
 *	Data type:	Tango::DevString
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::write_filename(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::write_filename(Tango::WAttribute &attr) entering... " << std::endl;
	//	Retrieve write value
	Tango::DevString	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(MoenchZMQ::write_filename) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::write_filename
}
//--------------------------------------------------------
/**
 *	Read attribute file_root_path related method
 *
 *
 *	Data type:	Tango::DevString
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_file_root_path(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_file_root_path(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_file_root_path) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_file_root_path_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_file_root_path
}
//--------------------------------------------------------
/**
 *	Write attribute file_root_path related method
 *
 *
 *	Data type:	Tango::DevString
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::write_file_root_path(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::write_file_root_path(Tango::WAttribute &attr) entering... " << std::endl;
	//	Retrieve write value
	Tango::DevString	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(MoenchZMQ::write_file_root_path) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::write_file_root_path
}
//--------------------------------------------------------
/**
 *	Read attribute normalize related method
 *
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_normalize(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_normalize(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_normalize) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_normalize_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_normalize
}
//--------------------------------------------------------
/**
 *	Write attribute normalize related method
 *
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::write_normalize(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::write_normalize(Tango::WAttribute &attr) entering... " << std::endl;
	//	Retrieve write value
	Tango::DevBoolean	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(MoenchZMQ::write_normalize) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::write_normalize
}
//--------------------------------------------------------
/**
 *	Read attribute threshold related method
 *
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_threshold(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_threshold(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_threshold) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_threshold_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_threshold
}
//--------------------------------------------------------
/**
 *	Read attribute counting_sigma related method
 *
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_counting_sigma(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_counting_sigma(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_counting_sigma) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_counting_sigma_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_counting_sigma
}
//--------------------------------------------------------
/**
 *	Write attribute counting_sigma related method
 *
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::write_counting_sigma(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::write_counting_sigma(Tango::WAttribute &attr) entering... " << std::endl;
	//	Retrieve write value
	Tango::DevDouble	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(MoenchZMQ::write_counting_sigma) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::write_counting_sigma
}
//--------------------------------------------------------
/**
 *	Read attribute live_period related method
 *
 *
 *	Data type:	Tango::DevLong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_live_period(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_live_period(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_live_period) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_live_period_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_live_period
}
//--------------------------------------------------------
/**
 *	Write attribute live_period related method
 *
 *
 *	Data type:	Tango::DevLong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::write_live_period(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::write_live_period(Tango::WAttribute &attr) entering... " << std::endl;
	//	Retrieve write value
	Tango::DevLong	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(MoenchZMQ::write_live_period) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::write_live_period
}
//--------------------------------------------------------
/**
 *	Read attribute process_pedestal related method
 *
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_process_pedestal(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_process_pedestal(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_process_pedestal) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_process_pedestal_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_process_pedestal
}
//--------------------------------------------------------
/**
 *	Read attribute analog_img related method
 *
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Image max = 400 x 400
 */
//--------------------------------------------------------
void MoenchZMQ::read_analog_img(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_analog_img(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_analog_img) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_analog_img_read, analog_imgAttrib::X_DATA_SIZE, analog_imgAttrib::Y_DATA_SIZE);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_analog_img
}
//--------------------------------------------------------
/**
 *	Read attribute counting_img related method
 *
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Image max = 400 x 400
 */
//--------------------------------------------------------
void MoenchZMQ::read_counting_img(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_counting_img(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_counting_img) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_counting_img_read, counting_imgAttrib::X_DATA_SIZE, counting_imgAttrib::Y_DATA_SIZE);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_counting_img
}

//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::add_dynamic_attributes()
 * Description:  Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void MoenchZMQ::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(MoenchZMQ::add_dynamic_attributes) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code to create and add dynamic attributes if any
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command start_receiver related method
 *
 *
 */
//--------------------------------------------------------
void MoenchZMQ::start_receiver()
{
	DEBUG_STREAM << "MoenchZMQ::start_receiver()  - " << device_name << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::start_receiver) ENABLED START -----*/
	/* clang-format on */

	//	Add your own code

	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::start_receiver
}
//--------------------------------------------------------
/**
 *	Command stop_receiver related method
 *
 *
 */
//--------------------------------------------------------
void MoenchZMQ::stop_receiver()
{
	DEBUG_STREAM << "MoenchZMQ::stop_receiver()  - " << device_name << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::stop_receiver) ENABLED START -----*/
	/* clang-format on */

	//	Add your own code

	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::stop_receiver
}
//--------------------------------------------------------
/**
 *	Command abort_receiver related method
 *
 *
 */
//--------------------------------------------------------
void MoenchZMQ::abort_receiver()
{
	DEBUG_STREAM << "MoenchZMQ::abort_receiver()  - " << device_name << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::abort_receiver) ENABLED START -----*/
	/* clang-format on */

	//	Add your own code

	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::abort_receiver
}
//--------------------------------------------------------
/**
 *	Command reset_pedestal related method
 *
 *
 */
//--------------------------------------------------------
void MoenchZMQ::reset_pedestal()
{
	DEBUG_STREAM << "MoenchZMQ::reset_pedestal()  - " << device_name << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::reset_pedestal) ENABLED START -----*/
	/* clang-format on */

	//	Add your own code

	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::reset_pedestal
}
//--------------------------------------------------------
/**
 *	Method      : MoenchZMQ::add_dynamic_commands()
 * Description:  Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void MoenchZMQ::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(MoenchZMQ::add_dynamic_commands) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code to create and add dynamic commands if any
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::add_dynamic_commands
}

/*----- PROTECTED REGION ID(MoenchZMQ::namespace_ending) ENABLED START -----*/
/* clang-format on */
//	Additional Methods
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::namespace_ending
} //	namespace
