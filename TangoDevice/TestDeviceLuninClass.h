/*----- PROTECTED REGION ID(TestDeviceLuninClass.h) ENABLED START -----*/
/* clang-format on */
//=============================================================================
//
// file :        TestDeviceLuninClass.h
//
// description : Include for the TestDeviceLunin root class.
//               This class is the singleton class for
//                the TestDeviceLunin device class.
//               It contains all properties and methods which the
//               TestDeviceLunin requires only once e.g. the commands.
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


#ifndef TestDeviceLuninClass_H
#define TestDeviceLuninClass_H

#include <tango/tango.h>
#include "TestDeviceLunin.h"

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TestDeviceLuninClass.h


namespace TestDeviceLunin_ns
{
/*----- PROTECTED REGION ID(TestDeviceLuninClass::classes for dynamic creation) ENABLED START -----*/
/* clang-format on */

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TestDeviceLuninClass::classes for dynamic creation

//=========================================
//	Define classes for attributes
//=========================================
//	Attribute counter class definition
class counterAttrib: public Tango::Attr
{
public:
	counterAttrib():Attr("counter",
			Tango::DEV_FLOAT, Tango::READ_WRITE) {};
	~counterAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TestDeviceLunin *>(dev))->read_counter(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<TestDeviceLunin *>(dev))->write_counter(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TestDeviceLunin *>(dev))->is_counter_allowed(ty);}
};


/**
 *	The TestDeviceLuninClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  TestDeviceLuninClass : public Tango::DeviceClass
#else
class TestDeviceLuninClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(TestDeviceLuninClass::Additional DServer data members) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TestDeviceLuninClass::Additional DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
		//	Method prototypes
		static TestDeviceLuninClass *init(const char *);
		static TestDeviceLuninClass *instance();
		~TestDeviceLuninClass();
		Tango::DbDatum	get_class_property(std::string &);
		Tango::DbDatum	get_default_device_property(std::string &);
		Tango::DbDatum	get_default_class_property(std::string &);

	protected:
		TestDeviceLuninClass(std::string &);
		static TestDeviceLuninClass *_instance;
		void command_factory();
		void attribute_factory(std::vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		std::string get_cvstag();
		std::string get_cvsroot();

	private:
		void device_factory(TANGO_UNUSED(const Tango::DevVarStringArray *));
		void create_static_attribute_list(std::vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,std::vector<Tango::Attr *> &);
		std::vector<std::string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(std::vector<Tango::Attr *> &att_list, std::string attname);
};

}	//	End of namespace

#endif   //	TestDeviceLunin_H
