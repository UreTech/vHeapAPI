#pragma once
#ifndef vHeap_H
#define vHeap_H

#include <vector>
#include <string>

namespace vhp {
	namespace v120 {
		void init_vhp_runtime();

		typedef int vhpID;

		enum varType
		{
			t_flex,//flex
			t_void,
			t_int8,
			t_uint8,
			t_int16,
			t_uint16,
			t_int32,
			t_uint32,
			t_int64,
			t_uint64,
			t_float,
			t_double,
			t_bool,
			t_error,//error
		};

		void* allocAsType(varType vt);

		size_t varType_enum_to_size(varType vt);

		std::string varType_enum_to_string(varType vt);

		varType varType_string_to_enum(std::string vtstr);

		struct dataType {
		public:
			std::string typeName = "";
			size_t typeSize = 0;
			varType var_t = t_flex;
			dataType(varType vt, size_t size = 0);
		private:
			dataType() {}
			friend class vHeap_RuntimeClass;
			friend class vHeap;
		};

		class vHeap {
		private:
			uint8_t* data = nullptr;
			dataType dt;
			std::string varName = "NULL";
		public:
			vHeap() {}
			void setVarriable(void* var);
			varType getVarType();
			std::string getVarTypeName();
			std::string getVarName();
			size_t getVarSize();
			void* getVar();
			friend class vHeap_RuntimeClass;
		};

		std::string vHeap_to_string(vHeap* t_vhp);
		void vHeap_setValueByString(std::string val, vHeap* t_vhp);

		class vHeap_RuntimeClass {
		private:
			std::vector<vHeap> heapStack;
			bool checkName(std::string name);
		public:
			size_t addVar(void* var_ptr, std::string varName, dataType dt, bool useIndexFlag = true);
			size_t addNewVar(std::string varName, varType vt, void*& ptr, bool useIndexFlag = true);
			size_t addNewStringVar(std::string varName, std::string str, void*& ptr, bool useIndexFlag = true);
			vHeap* getVarByID(vhpID id);
			vHeap* getVarByName(std::string vn);
			void checkHeapStack();
			void freeVhp(vhpID id);
			std::string createHeapDump();
		};
		extern vHeap_RuntimeClass* _vhp;
	}
}

#endif // !vHeap_H