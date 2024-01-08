/*----- PROTECTED REGION ID(MoenchZMQClass.h) ENABLED START -----*/
/* clang-format on */
//=============================================================================
//
// file :        MoenchZMQClass.h
//
// description : Include for the MoenchZMQ root class.
//               This class is the singleton class for
//                the MoenchZMQ device class.
//               It contains all properties and methods which the
//               MoenchZMQ requires only once e.g. the commands.
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


#ifndef MoenchZMQClass_H
#define MoenchZMQClass_H

#include <tango/tango.h>
#include "MoenchZMQ.h"

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	MoenchZMQClass.h


namespace MoenchZMQ_ns
{
/*----- PROTECTED REGION ID(MoenchZMQClass::classes for dynamic creation) ENABLED START -----*/
/* clang-format on */

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	MoenchZMQClass::classes for dynamic creation

//=========================================
//	Define classes for attributes
//=========================================
//	Attribute file_index class definition
class file_indexAttrib: public Tango::Attr
{
public:
	file_indexAttrib():Attr("file_index",
			Tango::DEV_ULONG, Tango::READ_WRITE) {};
	~file_indexAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_file_index(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<MoenchZMQ *>(dev))->write_file_index(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_file_index_allowed(ty);}
};

//	Attribute filename class definition
class filenameAttrib: public Tango::Attr
{
public:
	filenameAttrib():Attr("filename",
			Tango::DEV_STRING, Tango::READ_WRITE) {};
	~filenameAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_filename(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<MoenchZMQ *>(dev))->write_filename(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_filename_allowed(ty);}
};

//	Attribute file_root_path class definition
class file_root_pathAttrib: public Tango::Attr
{
public:
	file_root_pathAttrib():Attr("file_root_path",
			Tango::DEV_STRING, Tango::READ_WRITE) {};
	~file_root_pathAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_file_root_path(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<MoenchZMQ *>(dev))->write_file_root_path(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_file_root_path_allowed(ty);}
};

//	Attribute normalize class definition
class normalizeAttrib: public Tango::Attr
{
public:
	normalizeAttrib():Attr("normalize",
			Tango::DEV_BOOLEAN, Tango::READ_WRITE) {};
	~normalizeAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_normalize(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<MoenchZMQ *>(dev))->write_normalize(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_normalize_allowed(ty);}
};

//	Attribute threshold class definition
class thresholdAttrib: public Tango::Attr
{
public:
	thresholdAttrib():Attr("threshold",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~thresholdAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_threshold(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_threshold_allowed(ty);}
};

//	Attribute counting_sigma class definition
class counting_sigmaAttrib: public Tango::Attr
{
public:
	counting_sigmaAttrib():Attr("counting_sigma",
			Tango::DEV_DOUBLE, Tango::READ_WRITE) {};
	~counting_sigmaAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_counting_sigma(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<MoenchZMQ *>(dev))->write_counting_sigma(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_counting_sigma_allowed(ty);}
};

//	Attribute live_period class definition
class live_periodAttrib: public Tango::Attr
{
public:
	live_periodAttrib():Attr("live_period",
			Tango::DEV_LONG, Tango::READ_WRITE) {};
	~live_periodAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_live_period(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<MoenchZMQ *>(dev))->write_live_period(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_live_period_allowed(ty);}
};

//	Attribute process_pedestal class definition
class process_pedestalAttrib: public Tango::Attr
{
public:
	process_pedestalAttrib():Attr("process_pedestal",
			Tango::DEV_BOOLEAN, Tango::READ) {};
	~process_pedestalAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_process_pedestal(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_process_pedestal_allowed(ty);}
};

//	Attribute analog_img class definition
class analog_imgAttrib: public Tango::ImageAttr
{
public:
	// Constants for analog_img attribute
	constexpr static long X_DATA_SIZE = 400;
	constexpr static long Y_DATA_SIZE = 400;
	analog_imgAttrib():ImageAttr("analog_img",
			Tango::DEV_FLOAT, Tango::READ, analog_imgAttrib::X_DATA_SIZE, analog_imgAttrib::Y_DATA_SIZE) {};
	~analog_imgAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_analog_img(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_analog_img_allowed(ty);}
};

//	Attribute counting_img class definition
class counting_imgAttrib: public Tango::ImageAttr
{
public:
	// Constants for counting_img attribute
	constexpr static long X_DATA_SIZE = 400;
	constexpr static long Y_DATA_SIZE = 400;
	counting_imgAttrib():ImageAttr("counting_img",
			Tango::DEV_FLOAT, Tango::READ, counting_imgAttrib::X_DATA_SIZE, counting_imgAttrib::Y_DATA_SIZE) {};
	~counting_imgAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<MoenchZMQ *>(dev))->read_counting_img(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<MoenchZMQ *>(dev))->is_counting_img_allowed(ty);}
};


//=========================================
//	Define classes for commands
//=========================================
//	Command start_receiver class definition
class start_receiverClass : public Tango::Command
{
public:
	start_receiverClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(cmd_name,in,out,in_desc,out_desc, level)	{};

	start_receiverClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(cmd_name,in,out)	{};
	~start_receiverClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<MoenchZMQ *>(dev))->is_start_receiver_allowed(any);}
};

//	Command stop_receiver class definition
class stop_receiverClass : public Tango::Command
{
public:
	stop_receiverClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(cmd_name,in,out,in_desc,out_desc, level)	{};

	stop_receiverClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(cmd_name,in,out)	{};
	~stop_receiverClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<MoenchZMQ *>(dev))->is_stop_receiver_allowed(any);}
};

//	Command abort_receiver class definition
class abort_receiverClass : public Tango::Command
{
public:
	abort_receiverClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(cmd_name,in,out,in_desc,out_desc, level)	{};

	abort_receiverClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(cmd_name,in,out)	{};
	~abort_receiverClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<MoenchZMQ *>(dev))->is_abort_receiver_allowed(any);}
};

//	Command reset_pedestal class definition
class reset_pedestalClass : public Tango::Command
{
public:
	reset_pedestalClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(cmd_name,in,out,in_desc,out_desc, level)	{};

	reset_pedestalClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(cmd_name,in,out)	{};
	~reset_pedestalClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<MoenchZMQ *>(dev))->is_reset_pedestal_allowed(any);}
};


/**
 *	The MoenchZMQClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  MoenchZMQClass : public Tango::DeviceClass
#else
class MoenchZMQClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(MoenchZMQClass::Additional DServer data members) ENABLED START -----*/
	/* clang-format on */
	//	Add your own code
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	MoenchZMQClass::Additional DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
		//	Method prototypes
		static MoenchZMQClass *init(const char *);
		static MoenchZMQClass *instance();
		~MoenchZMQClass();
		Tango::DbDatum	get_class_property(std::string &);
		Tango::DbDatum	get_default_device_property(std::string &);
		Tango::DbDatum	get_default_class_property(std::string &);

	protected:
		MoenchZMQClass(std::string &);
		static MoenchZMQClass *_instance;
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

#endif   //	MoenchZMQ_H