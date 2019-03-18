#include "systemc.h"

template <class data_size>
SC_MODULE(alu_block) {
	   
	    sc_in<bool> enable_alu;
		sc_in<sc_uint<4> > alu_control_signal;
		sc_in<sc_uint<4> > cond_control_signal;
		sc_in<sc_uint<1> > immediate_control_signal;
		sc_in<data_size> operand_1;
		sc_in<data_size> operand_2;
		sc_out<data_size> result;
				
	sc_uint<8> PSR;
	sc_uint<1>  overflow_bit = 0x0008;
	sc_uint<16> zero_data = 0x0000;
	sc_uint<16> max_val =0xFFFF;
	sc_uint<1>  carry =0x0006;
	sc_uint<1>  zero_bit =0x0004;
	sc_uint<1>  negative_bit =0x0002;
		
	
		bool cond_check(std::string instruction);
		void execution();
		
	SC_CTOR(alu_block) {

		SC_METHOD(execution);

		sensitive<<enable_alu.pos();
		
	}
};
template <class data_size>
	bool alu_block<data_size>:: cond_check(std::string instruction){
			bool res = false;
			switch(cond_control_signal.read()) {
				case 0b0000: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : EQ"<<endl;
					if(PSR.range(zero_bit , zero_bit) == 1) {
						res = true;
					}
					break;
				case 0b0001: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : NE"<<endl;
					if(PSR.range(zero_bit , zero_bit) ==  0) {
						res = true;
					}
					break;
				case 0b0010: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : GE"<<endl;
					if(PSR.range(zero_bit , zero_bit) ==  1 && PSR.range(negative_bit , negative_bit) ==  1) {
						res = true;
					}
					break;
				case 0b0011: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : CS"<<endl;
					if(PSR.range(carry , carry) ==  1) {
						res = true;
					}
					break;
				case 0b0100: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : CC"<<endl;
					if(PSR.range(carry , carry) ==  0) {
						res = true;
					}
					break;
				case 0b0101: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : GT"<<endl;
					if(PSR.range(negative_bit , negative_bit) ==  1) {
						res = true;
					}
					break;
				case 0b0110: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : LE"<<endl;
					if(PSR.range(negative_bit , negative_bit) ==  0) {
						res = true;
					}
					break;
				case 0b0111: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : FS"<<endl;
					if(PSR.range(overflow_bit , overflow_bit) ==  1) {
						res = true;
					}
					break;
				case 0b1000: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : FC"<<endl;
					if(PSR.range(overflow_bit , overflow_bit) ==  0) {
						res = true;
					}
					break;
				case 0b1001: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : LT"<<endl;
					if(PSR.range(zero_bit , zero_bit) ==  0 && PSR.range(negative_bit , negative_bit) ==  0) {
						res = true;
					}
					break;
				case 0b1010: 
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : US"<<endl;
					res = true;
					break;
			}
			return res;
		
	}
	template <class data_size>
	void alu_block<data_size>:: execution(){
	
			sc_uint<16> op1 = operand_1.read();
			sc_uint<16> op2 = operand_2.read();
			data_size output;
			switch(alu_control_signal.read()) {
				case 0b0000 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = op1+op2;
					result.write(output);
					
					if(result >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}

					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : ADD/ADI"<<endl;
					cout<<"       Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
 					break;
				case 0b0001 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					if(op1 < op2) {
						PSR.range(carry , carry) = 1;
					} 
					output = abs(op1 - op2);
					result.write(output);
					if(result >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : SUB/SUBI"<<endl;
					cout<<"       Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break;
				case 0b0010 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					if(op1 < op2) {
						PSR.range(carry , carry) = 1;
					} 
					output = abs(op1 - op2);
					result.write(output);
					if(result >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					if(result == zero_data) {
						PSR.range(zero_bit , zero_bit) = 1;
					}
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : CMP/CMPI"<<endl;
					cout<<"       Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b0011 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = (op1 & op2);
					result.write(output);
					if(result >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : AND/ANDI"<<endl;
					cout<<"       Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b0100 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = (op1 | op2);
					result.write(output);
					if(result >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : OR/ORI"<<endl;
					cout<<"       Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b0101 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = op1 ^ op2;
					result.write(output);
					if(result >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : XOR/XORI"<<endl;
					cout<<"       Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b0110 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = op2;
					result.write(output);
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : MOV/MOVI"<<endl;
					cout<<"       Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b0111 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = op1 << op2;
					result.write(output);
					if(result >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : LSH/LSHI"<<endl;
					cout<<"       _Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b1000 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = op1 >> op2;
					result.write(op1 >> op2);
					if(result >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : ASH/ASHI"<<endl;
					cout<<"       _Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b1001 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = op2;
					result.write(output);
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : LUI/LOAD"<<endl;
					cout<<"       _Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b1010: 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = op1;
					result.write(result);
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : STOR"<<endl;
					cout<<"       _Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b1011 :
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					if(cond_check("       Instruction : Bcond")) {
						output = op1 + op2;
					}else {
						output = op2;
					}
					result.write(output);
					cout<<"       _Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break;
				case 0b1100 : 
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					if(cond_check("       Instruction : Jcond")) {
						output = op1;
					}else {
						output = op2;
					}
					result.write(output);
					cout<<"       _Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
				case 0b1101 :
					if(op1 >= max_val || op2 >= max_val) {
						PSR.range(overflow_bit , overflow_bit) = 1;
					}
					output = op2;
					result.write(output);
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl;
					cout<<"       Instruction : JAL"<<endl;
					cout<<"       _Immediate : "<<immediate_control_signal.read()<<endl;
					cout<<"       operand_1 :  "<<op1<<endl;
					cout<<"       operand_2 :  "<<op2<<endl;
					cout<<"       Result :  "<<output<<endl;
					cout<<"       PSR :  "<<PSR<<endl;
					cout<<"/**<--------------------------------------------------:: ALU LOG ::----------------------------------------------------------------->**/"<<endl<<endl<<endl;
					break; 
			}

	}
