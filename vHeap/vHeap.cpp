#include"vHeap.h"
#include<Windows.h>
#include <stdexcept>
#include <iostream>
#include "nHeap/nHeapConfig.h"

enum memAddrStatus {
	ms_error,
	ms_commited,
	ms_reserved,
	ms_free,
	ms_unknown,
};

memAddrStatus CheckMemoryAddress(void* address) {
	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T result = VirtualQuery(address, &mbi, sizeof(mbi));

	if (result == 0) {
		return ms_error;
	}

	switch (mbi.State) {
	case MEM_COMMIT:
		return ms_commited;
		break;
	case MEM_RESERVE:
		return ms_reserved;
		break;
	case MEM_FREE:
		return ms_free;
		break;
	default:
		return ms_unknown;
		break;
	}
}


void vhp::v120::vHeap::setVarriable(void* var)
{
	memcpy(this->data, var, this->dt.typeSize);
}

vhp::v120::varType vhp::v120::vHeap::getVarType()
{
	return this->dt.var_t;
}

std::string vhp::v120::vHeap::getVarTypeName()
{
	return this->dt.typeName;
}

std::string vhp::v120::vHeap::getVarName()
{
	return this->varName;
}

size_t vhp::v120::vHeap::getVarSize()
{
	return this->dt.typeSize;
}

void* vhp::v120::vHeap::getVar()
{
	return this->data;
}

vhp::v120::dataType::dataType(varType vt, size_t size)
{
	if (size != 0) {
		this->typeSize = size;
		this->var_t = vt;
	}
	else {
		this->typeSize = varType_enum_to_size(vt);
		this->var_t = vt;
	}
	this->typeName = varType_enum_to_string(vt);
}

vhp::v120::vHeap_RuntimeClass* vhp::v120::_vhp;

void vhp::v120::init_vhp_runtime()
{
	_vhp = new vHeap_RuntimeClass();
}

void* vhp::v120::allocAsType(varType vt)
{
	return malloc(varType_enum_to_size(vt));
}

size_t vhp::v120::varType_enum_to_size(varType vt)
{
	switch (vt)
	{
	case t_flex:
		return vhp_unknown_data_size;
		break;
	case t_void:
		return vhp_unknown_data_size;
		break;
	case t_int8:
		return 1;
		break;
	case t_uint8:
		return 1;
		break;
	case t_int16:
		return 2;
		break;
	case t_uint16:
		return 2;
		break;
	case t_int32:
		return 4;
		break;
	case t_uint32:
		return 4;
		break;
	case t_int64:
		return 8;
		break;
	case t_uint64:
		return 8;
		break;
	case t_float:
		return 4;
		break;
	case t_double:
		return 8;
		break;
	case t_bool:
		return 1;
		break;
	case t_error:
		throw std::string("vhp.varType.error-->d'" + std::to_string(vt) + "'");
		return vhp_error_data_size;
		break;
	default:
		throw std::string("vhp.varType.error-->d'" + std::to_string(vt) + "'");
		return vhp_error_data_size;
		break;
	}
}

std::string vhp::v120::varType_enum_to_string(varType vt)
{
	switch (vt)
	{
	case t_flex:
		return "flex";
		break;
	case t_void:
		return "void";
		break;
	case t_int8:
		return "int8";
		break;
	case t_uint8:
		return "uint8";
		break;
	case t_int16:
		return "int16";
		break;
	case t_uint16:
		return "uint16";
		break;
	case t_int32:
		return "int32";
		break;
	case t_uint32:
		return "uint32";
		break;
	case t_int64:
		return "int64";
		break;
	case t_uint64:
		return "uint64";
		break;
	case t_float:
		return "float";
		break;
	case t_double:
		return "double";
		break;
	case t_bool:
		return "bool";
		break;
	case t_error:
		throw std::string("vhp.varType.error-->d'" + std::to_string(vt) + "'");
		return "error";
		break;
	default:
		break;
	}
}

vhp::v120::varType vhp::v120::varType_string_to_enum(std::string vtstr)
{
	varType result;
	if (vtstr == "flex") {
		return t_flex;
	}else if (vtstr == "void") {
		return t_void;
	}
	else if (vtstr == "int8") {
		return t_int8;
	}
	else if (vtstr == "uint8") {
		return t_uint8;
	}
	else if (vtstr == "int16") {
		return t_int16;
	}
	else if (vtstr == "uint16") {
		return t_uint16;
	}
	else if (vtstr == "int32") {
		return t_int32;
	}
	else if (vtstr == "uint32") {
		return t_uint32;
	}
	else if (vtstr == "int64") {
		return t_uint64;
	}
	else if (vtstr == "float") {
		return t_float;
	}
	else if (vtstr == "double") {
		return t_double;
	}
	else if (vtstr == "bool") {
		return t_bool;
	}
	else {
		throw std::string("vhp.varType.error-->'" + vtstr + "'");
		return t_error;
	}
}

std::string vhp::v120::vHeap_to_string(vHeap* t_vhp)
{
	switch (t_vhp->getVarType())
	{
	case t_flex:
		return "flex data cannot be displayed.";
		break;
	case t_void:
		return "void data cannot be displayed.";
		break;
	case t_int8:
		return std::to_string(*(int8_t*)t_vhp->getVar());
		break;
	case t_uint8:
		return std::to_string(*(uint8_t*)t_vhp->getVar());
		break;
	case t_int16:
		return std::to_string(*(int16_t*)t_vhp->getVar());
		break;
	case t_uint16:
		return std::to_string(*(uint16_t*)t_vhp->getVar());
		break;
	case t_int32:
		return std::to_string(*(int32_t*)t_vhp->getVar());
		break;
	case t_uint32:
		return std::to_string(*(uint32_t*)t_vhp->getVar());
		break;
	case t_int64:
		return std::to_string(*(int64_t*)t_vhp->getVar());
		break;
	case t_uint64:
		return std::to_string(*(uint64_t*)t_vhp->getVar());
		break;
	case t_float:
		return std::to_string(*(float*)t_vhp->getVar());
		break;
	case t_double:
		return std::to_string(*(double*)t_vhp->getVar());
		break;
	case t_bool:
		return std::to_string(*(bool*)t_vhp->getVar());
		break;
	default:
		throw std::string("vhp.data.display.error");
		return "unknown var type.";
		break;
	}
}

void vhp::v120::vHeap_setValueByString(std::string val, vHeap* t_vhp)
{
	try {
		switch (t_vhp->getVarType())
		{
		case t_flex:
			throw std::string("vhp.setValue.error-->Flex can't be set");
			break;
		case t_void:
			throw std::string("vhp.setValue.error-->Void can't be set");
			break;
		case t_int8:
			*(int8_t*)t_vhp->getVar() = (int8_t)std::stoll(val);
			break;
		case t_uint8:
			*(uint8_t*)t_vhp->getVar() = (uint8_t)std::stoull(val);
			break;
		case t_int16:
			*(int16_t*)t_vhp->getVar() = (int16_t)std::stoll(val);
			break;
		case t_uint16:
			*(uint16_t*)t_vhp->getVar() = (uint16_t)std::stoull(val);
			break;
		case t_int32:
			*(int32_t*)t_vhp->getVar() = (int32_t)std::stoll(val);
			break;
		case t_uint32:
			*(uint32_t*)t_vhp->getVar() = (uint32_t)std::stoull(val);
			break;
		case t_int64:
			*(int64_t*)t_vhp->getVar() = (int64_t)std::stoll(val);
			break;
		case t_uint64:
			*(uint64_t*)t_vhp->getVar() = (uint64_t)std::stoull(val);
			break;
		case t_float:
			*(float*)t_vhp->getVar() = (float)std::stof(val);
			break;
		case t_double:
			*(double*)t_vhp->getVar() = (double)std::stod(val);
			break;
		case t_bool:
			*(bool*)t_vhp->getVar() = (uint8_t)std::stoull(val);
			break;
		default:
			throw std::string("vhp.setValue.error-->unknown type");
			break;
		}
	}
	catch (const std::exception& e) {
		throw std::string("vhp.setValue.error-->" + std::string(e.what()));
	}
}

bool vhp::v120::vHeap_RuntimeClass::checkName(std::string name)
{
	for (size_t i = 0; i < heapStack.size(); i++) {
		if (heapStack.at(i).varName == name) {
			return false;
		}
	}
	return true;
}

size_t vhp::v120::vHeap_RuntimeClass::addVar(void* var_ptr, std::string varName, dataType dt, bool useIndexFlag)
{
	vHeap tvhp;
	tvhp.data = (uint8_t*)var_ptr;
	tvhp.dt = dt;
	if (useIndexFlag) {
		tvhp.varName = varName + "_" + std::to_string(heapStack.size());
	}
	else {
		if (checkName(varName)) {
			tvhp.varName = varName;
		}
		else {
			throw std::string("vhp.add.error-->same name");
		}
	}
	this->heapStack.push_back(tvhp);
	return heapStack.size() - 1;
}

size_t vhp::v120::vHeap_RuntimeClass::addNewVar(std::string varName, varType vt, void*& ptr, bool useIndexFlag)
{
	vHeap tvhp;
	tvhp.data = (uint8_t*)allocAsType(vt);
	dataType dt;
	dt.typeName = varType_enum_to_string(vt);
	dt.typeSize = varType_enum_to_size(vt);
	dt.var_t = vt;
	tvhp.dt = dt;
	if (useIndexFlag) {
		tvhp.varName = varName + "_" + std::to_string(heapStack.size());
	}
	else {
		if (checkName(varName)) {
			tvhp.varName = varName;
		}
		else {
			throw std::string("vhp.add.error-->same name");
		}
	}
	this->heapStack.push_back(tvhp);
	ptr = tvhp.data;
	return  heapStack.size() - 1;
}

size_t vhp::v120::vHeap_RuntimeClass::addNewStringVar(std::string varName, std::string str, void*& ptr, bool useIndexFlag)
{
	vHeap tvhp;
	tvhp.data = (uint8_t*)malloc(str.size());
	dataType dt;
	dt.typeName = varType_enum_to_string(t_flex);
	dt.typeSize = str.size();
	dt.var_t = t_flex;
	tvhp.dt = dt;
	if (useIndexFlag) {
		tvhp.varName = varName + "_" + std::to_string(heapStack.size());
	}
	else {
		if (checkName(varName)) {
			tvhp.varName = varName;
		}
		else {
			throw std::string("vhp.add.error-->same name");
		}
	}
	this->heapStack.push_back(tvhp);
	ptr = tvhp.data;
	return  heapStack.size() - 1;
}

vhp::v120::vHeap* vhp::v120::vHeap_RuntimeClass::getVarByID(vhpID id)
{
	return &this->heapStack[id];
}

vhp::v120::vHeap* vhp::v120::vHeap_RuntimeClass::getVarByName(std::string vn)
{
	bool fail = false;
	vHeap* result = nullptr;
	for (size_t i = 0; i < heapStack.size(); i++) {
		if (heapStack.at(i).varName == vn) {
			fail = false;
			result = &heapStack.at(i);
			break;
		}
		else {
			fail = true;
		}
	}
	if (fail) {
		throw std::string("vhp.getVarByName.fail");
	}
	else {
		return result;
	}
}

void vhp::v120::vHeap_RuntimeClass::checkHeapStack()
{
	std::vector<void*> addrs;
	for (unsigned int i = 0; i < heapStack.size(); i++) {
		if (CheckMemoryAddress(heapStack[i].data) != ms_commited) {
			if (i < heapStack.size()) {
				heapStack.erase(heapStack.begin() + i);
				//i--;
			}
		}
		else {
			void* tptr = heapStack[i].data;
			for (unsigned int j = 0; j < addrs.size(); j++) {
				if (tptr == addrs[j]) {
					if (i < heapStack.size()) {
						heapStack.erase(heapStack.begin() + i);
						//i--;
					}
					continue;
				}
			}
			addrs.push_back(tptr);
		}
	}
}

void vhp::v120::vHeap_RuntimeClass::freeVhp(vhpID id)
{
	if (id < heapStack.size()) {
		free(heapStack[id].data);
		heapStack.erase(heapStack.begin() + id);
	}
}

std::string vhp::v120::vHeap_RuntimeClass::createHeapDump()
{
	this->checkHeapStack();
	std::string result = "Heap dump:(total vhp count: " + std::to_string(heapStack.size()) + ")\n";
	for (unsigned int i = 0; i < heapStack.size(); i++) {
		std::string tmpstr = "varName: " + heapStack[i].getVarName() + "\nvarTypeName: " + heapStack[i].getVarTypeName() + "\nvarSize: " + std::to_string(heapStack[i].getVarSize()) + "\nvarValue: " + vHeap_to_string(&heapStack[i]);
		result.append("**************************\n");
		result.append(tmpstr);
		result.push_back('\n');
	}
	result.append("**************************");
	return result;
}
