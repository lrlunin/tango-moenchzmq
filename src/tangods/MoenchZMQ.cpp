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
#include "data.hpp"
#include "../backend/FileWriter.hpp"
#include "../backend/HDFWriter.hpp"
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
	delete[] attr_file_name_read;
	delete[] attr_file_root_path_read;
	delete[] attr_normalize_read;
	delete[] attr_threshold_read;
	delete[] attr_counting_sigma_read;
	delete[] attr_live_period_read;
	delete[] attr_process_pedestal_read;
	delete[] attr_analog_img_read;
	delete[] attr_counting_img_read;
	delete[] attr_analog_img_pumped_read;
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
	set_state(Tango::INIT);
	get_device_property();

	FileWriter* fileWriter = new HDFWriter(0, SAVE_ROOT_PATH);
	zmq_listener_ptr = std::make_unique<ZMQListener>(ZMQ_RX_IP, ZMQ_RX_PORT);
	zmq_listener_ptr->comp_backend_ptr = std::make_unique<ComputationBackend>(fileWriter);


	attr_file_index_read = new Tango::DevULong[1];
	attr_file_name_read = new Tango::DevString[1];
	attr_file_root_path_read = new Tango::DevString[1];
	attr_acquired_frames_read = new Tango::DevLong[1];
	attr_normalize_read = new Tango::DevBoolean[1];
	attr_threshold_read = new Tango::DevDouble[1];
	attr_counting_sigma_read = new Tango::DevDouble[1];
	attr_live_period_read = new Tango::DevLong[1];
	attr_process_pedestal_read = new Tango::DevBoolean[1];
	attr_split_pumped_read = new Tango::DevBoolean[1];
	attr_analog_img_read = new Tango::DevFloat[400*400];
	attr_analog_img_pumped_read = new Tango::DevFloat[400*400];
	attr_counting_img_read = new Tango::DevFloat[400*400];
	attr_counting_img_pumped_read = new Tango::DevFloat[400*400];
	//	No longer if mandatory property not set.
	if (mandatoryNotDefined)
	return;
	*attr_live_period_read = 0;
	*attr_normalize_read = false;
	load_images_previews();
	
	
	set_state(Tango::ON);
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
	Tango::DbData dev_prop{Tango::DbDatum("ZMQ_RX_IP"), Tango::DbDatum("ZMQ_RX_PORT"), Tango::DbDatum("SAVE_ROOT_PATH")};
	get_db_device()->get_property(dev_prop);
	// if any of the properties is empty, mark the device as not initialized
	for (auto &prop : dev_prop)
	{
		if (prop.is_empty())
		{
			ERROR_STREAM << "Property " << prop.name << " not set" << std::endl;
			mandatoryNotDefined = true;
		}
	}
	dev_prop[0] >> ZMQ_RX_IP;
	dev_prop[1] >> ZMQ_RX_PORT;
	dev_prop[2] >> SAVE_ROOT_PATH;

	/*----- PROTECTED REGION ID(MoenchZMQ::get_device_property_after) ENABLED START -----*/
	/* clang-format on */
	//	Check device property data members init
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::get_device_property_after
}
void MoenchZMQ::load_images_previews(){
	std::copy(ANALOG_UNPUMPED, ANALOG_UNPUMPED + counting_imgAttrib::X_DATA_SIZE * counting_imgAttrib::Y_DATA_SIZE, attr_analog_img_read);
	std::copy(ANALOG_PUMPED, ANALOG_PUMPED + counting_imgAttrib::X_DATA_SIZE * counting_imgAttrib::Y_DATA_SIZE, attr_analog_img_pumped_read);
	std::copy(COUNTING_UNPUMPED, COUNTING_UNPUMPED + counting_imgAttrib::X_DATA_SIZE * counting_imgAttrib::Y_DATA_SIZE, attr_counting_img_read);
	std::copy(COUNTING_PUMPED, COUNTING_PUMPED + counting_imgAttrib::X_DATA_SIZE * counting_imgAttrib::Y_DATA_SIZE, attr_counting_img_pumped_read);
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
	*attr_file_index_read = zmq_listener_ptr->comp_backend_ptr->file_index;
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
	*attr_file_index_read = w_val;
	zmq_listener_ptr->comp_backend_ptr->file_index = w_val;
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
void MoenchZMQ::read_file_name(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_filename(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_filename) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	*attr_file_name_read = Tango::string_dup(zmq_listener_ptr->comp_backend_ptr->file_name);
	attr.set_value(attr_file_name_read);
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
void MoenchZMQ::write_file_name(Tango::WAttribute &attr)
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
	*attr_file_root_path_read = Tango::string_dup(zmq_listener_ptr->comp_backend_ptr->save_root_path);
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
	*attr_file_root_path_read = w_val;
	zmq_listener_ptr->comp_backend_ptr->save_root_path = w_val;
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
void MoenchZMQ::write_threshold(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::write_threshold(Tango::WAttribute &attr) entering... " << std::endl;
	//	Retrieve write value
	Tango::DevDouble	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(MoenchZMQ::write_threshold) ENABLED START -----*/
	/* clang-format on */
	*attr_threshold_read = w_val;
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::write_threshold
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
 *	Read attribute analog_img_pumped related method
 *
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Image max = 400 x 400
 */
//--------------------------------------------------------
void MoenchZMQ::read_analog_img_pumped(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_analog_img_pumped(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_analog_img_pumped) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_analog_img_pumped_read, analog_img_pumpedAttrib::X_DATA_SIZE, analog_img_pumpedAttrib::Y_DATA_SIZE);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_analog_img_pumped
}
//--------------------------------------------------------
/**
 *	Read attribute counting_img_pumped related method
 *
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Image max = 400 x 400
 */
//--------------------------------------------------------
void MoenchZMQ::read_counting_img_pumped(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_counting_img_pumped(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_counting_img_pumped) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	attr.set_value(attr_counting_img_pumped_read, counting_img_pumpedAttrib::X_DATA_SIZE, counting_img_pumpedAttrib::Y_DATA_SIZE);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_counting_img_pumped
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
	*attr_counting_sigma_read = w_val;
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
	*attr_live_period_read = w_val;
	zmq_listener_ptr->comp_backend_ptr->live_period = w_val;
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
	*attr_process_pedestal_read = zmq_listener_ptr->comp_backend_ptr->isPedestal;
	attr.set_value(attr_process_pedestal_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_process_pedestal
}
//--------------------------------------------------------
/**
 *	Read attribute split_pumped related method
 *
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_split_pumped(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_split_pumped(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_split_pumped) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	*attr_split_pumped_read = zmq_listener_ptr->comp_backend_ptr->isSplitPumped;
	attr.set_value(attr_split_pumped_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_split_pumped
}
//--------------------------------------------------------
/**
 *	Read attribute acquired_frames related method
 *
 *
 *	Data type:	Tango::DevLong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::read_acquired_frames(Tango::Attribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::read_acquired_frames(Tango::Attribute &attr) entering... " << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::read_acquired_frames) ENABLED START -----*/
	/* clang-format on */
	//	Set the attribute value
	*attr_acquired_frames_read = zmq_listener_ptr->received_frames_amount.load();
	attr.set_value(attr_acquired_frames_read);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_acquired_frames
}
//--------------------------------------------------------
//--------------------------------------------------------
/**
 *	Write attribute split_pumped related method
 *
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MoenchZMQ::write_split_pumped(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "MoenchZMQ::write_split_pumped(Tango::WAttribute &attr) entering... " << std::endl;
	//	Retrieve write value
	Tango::DevBoolean	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(MoenchZMQ::write_split_pumped) ENABLED START -----*/
	/* clang-format on */
	*attr_split_pumped_read = w_val;
	zmq_listener_ptr->comp_backend_ptr->isSplitPumped = w_val;
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::write_split_pumped
}
void MoenchZMQ::write_process_pedestal(Tango::WAttribute &attr)
{
	Tango::DevBoolean	w_val;
	attr.get_write_value(w_val);

	*attr_process_pedestal_read = w_val;
	zmq_listener_ptr->comp_backend_ptr->isPedestal = w_val;
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
	//push_change_event("analog_img");
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::read_analog_img
}
//----------------------------------------------------	----
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
	zmq_listener_ptr->start_receive();
	set_state(Tango::RUNNING);
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

	// wrap blocking function into lambda and then run a separate thread for it
	std::thread([&]{zmq_listener_ptr->stop_receive();
					copy_image_buffers();
					push_images_change();
					set_state(Tango::ON);
					}
				).detach();
	
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
	zmq_listener_ptr->comp_backend_ptr->resetPedestalAndRMS();
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::reset_pedestal
}
//--------------------------------------------------------
/**
 *	Command push_images_change related method
 *
 *
 */
//--------------------------------------------------------
void MoenchZMQ::push_images_change()
{
	DEBUG_STREAM << "MoenchZMQ::push_images_change()  - " << device_name << std::endl;
	/*----- PROTECTED REGION ID(MoenchZMQ::push_images_change) ENABLED START -----*/
	/* clang-format on */
	push_change_event("analog_img", attr_analog_img_read, analog_imgAttrib::X_DATA_SIZE, analog_imgAttrib::Y_DATA_SIZE);
	push_change_event("counting_img", attr_counting_img_read, counting_imgAttrib::X_DATA_SIZE, counting_imgAttrib::Y_DATA_SIZE);
	push_change_event("analog_img_pumped", attr_analog_img_pumped_read, analog_img_pumpedAttrib::X_DATA_SIZE, analog_img_pumpedAttrib::Y_DATA_SIZE);
	push_change_event("counting_img_pumped", attr_counting_img_pumped_read, counting_img_pumpedAttrib::X_DATA_SIZE, counting_img_pumpedAttrib::Y_DATA_SIZE);
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQ::push_images_change
}
void MoenchZMQ::copy_image_buffers(){
	zmq_listener_ptr->comp_backend_ptr->analog_sum.copy_to_buffer<Tango::DevFloat*>(attr_analog_img_read);
	zmq_listener_ptr->comp_backend_ptr->analog_sum_pumped.copy_to_buffer<Tango::DevFloat*>(attr_analog_img_pumped_read);
	zmq_listener_ptr->comp_backend_ptr->counting_sum.copy_to_buffer<Tango::DevFloat*>(attr_counting_img_read);
	zmq_listener_ptr->comp_backend_ptr->counting_sum_pumped.copy_to_buffer<Tango::DevFloat*>(attr_counting_img_pumped_read);
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
