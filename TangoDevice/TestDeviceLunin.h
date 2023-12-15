/*----- PROTECTED REGION ID(TestDeviceLunin.h) ENABLED START -----*/
/* clang-format on */
//=============================================================================
//
// file :        TestDeviceLunin.h
//
// description : Include file for the TestDeviceLunin class
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


#ifndef TestDeviceLunin_H
#define TestDeviceLunin_H

#include <tango/tango.h>

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TestDeviceLunin.h

#ifdef TANGO_LOG
	// cppTango after c934adea (Merge branch 'remove-cout-definition' into 'main', 2022-05-23)
	// nothing to do
#else
	// cppTango 9.3-backports and older
	#define TANGO_LOG       cout
	#define TANGO_LOG_INFO  cout2
	#define TANGO_LOG_DEBUG cout3
#endif // TANGO_LOG

/**
 *  TestDeviceLunin class description:
 *    LOL MEME
 */


namespace TestDeviceLunin_ns
{
/*----- PROTECTED REGION ID(TestDeviceLunin::Additional Class Declarations) ENABLED START -----*/
/* clang-format on */
//	Additional Class Declarations
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TestDeviceLunin::Additional Class Declarations

class TestDeviceLunin : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(TestDeviceLunin::Data Members) ENABLED START -----*/
/* clang-format on */
//	Add your own data members
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TestDeviceLunin::Data Members


//	Attribute data members
public:
	Tango::DevFloat	*attr_counter_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	TestDeviceLunin(Tango::DeviceClass *cl,std::string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	TestDeviceLunin(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	TestDeviceLunin(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~TestDeviceLunin();


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();


//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : TestDeviceLunin::read_attr_hardware()
	 * Description:  Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(std::vector<long> &attr_list);
	//--------------------------------------------------------
	/*
	 *	Method      : TestDeviceLunin::write_attr_hardware()
	 * Description:  Hardware writing for attributes.
	 */
	//--------------------------------------------------------
	virtual void write_attr_hardware(std::vector<long> &attr_list);

/**
 *	Attribute counter related methods
 *
 *
 *	Data type:  Tango::DevFloat
 *	Attr type:	Scalar
 */
	virtual void read_counter(Tango::Attribute &attr);
	virtual void write_counter(Tango::WAttribute &attr);
	virtual bool is_counter_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : TestDeviceLunin::add_dynamic_attributes()
	 * Description:  Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:


	//--------------------------------------------------------
	/**
	 *	Method      : TestDeviceLunin::add_dynamic_commands()
	 * Description:  Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(TestDeviceLunin::Additional Method prototypes) ENABLED START -----*/
/* clang-format on */
//	Additional Method prototypes
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TestDeviceLunin::Additional Method prototypes
};

/*----- PROTECTED REGION ID(TestDeviceLunin::Additional Classes Definitions) ENABLED START -----*/
/* clang-format on */
//	Additional Classes Definitions
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TestDeviceLunin::Additional Classes Definitions

}	//	End of namespace

#endif   //	TestDeviceLunin_H
