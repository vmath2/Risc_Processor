#include "systemc.h"
template<class addr_size, class data_size>
SC_MODULE(controller){

	sc_in<bool> clock;
	sc_in<data_size> operand1_input;
	sc_in<data_size> operand2_input;
	sc_in<data_size> dm_data_input;
	sc_in<sc_uint<16> > instruction_input;
	sc_in<data_size> alu_result_input;

	sc_out<bool> program_memory_read;
	sc_out<data_size> program_counter_out;

	sc_out<bool> register_file_read;
	sc_out<bool> register_file_write;
	sc_out<sc_uint<1> > immediate_flag;
    sc_out<addr_size> register_file_addr1_out;
	sc_out<addr_size> register_file_addr2_out;
	sc_out<data_size> register_file_data_out;

	sc_out<bool> enable_alu;
	sc_out<sc_uint<4> > alu_control_signal;
	sc_out<sc_uint<4> > cond_control_signal;
	sc_out<sc_uint<1> > immediate_control_signal;
	sc_out<data_size> operand1_output;
	sc_out<data_size> operand2_output;
	
	sc_out<bool> data_memory_read;
	sc_out<bool> data_memory_write;
	sc_out<addr_size> data_memory_addr_out;
	sc_out<data_size> data_memory_data_out;

	enum states {S0, S1, S2, S3, S4, S5, S6};

	 sc_signal<states> current_state;

std::vector<sc_uint<49> > q_instruction_control_signal;
		sc_uint<16> instruction_register;
		sc_uint<16> program_counter;

sc_uint<49>prepare_instruction_control_signal(sc_uint<4> alu_control_signal , sc_uint<1> immediate_flag , sc_uint<4> register_dest_cond , sc_uint<4> opCode_ext_immH , sc_uint<4> register_source_immL);
void controller_execution();
void fetch_instruction();
void decode_instruction();
void read_register_file();
void alu_execution();
void memory_access();
void write_register_file();
	
  SC_CTOR(controller){
	  program_counter = 0x0000;
	current_state.write(S0);
	SC_THREAD(controller_execution);
	sensitive<<clock;
  }
};
template<class addr_size, class data_size>
void controller<addr_size, data_size>:: controller_execution(){
	while(true){
		wait();
	if(clock.read() == 1) {

				switch(current_state.read()) {
					case S0 :
						current_state.write(S1);
						fetch_instruction();
						break;
					case S2 : 
		   	        	current_state.write(S3);
		   	        	read_register_file();
		   	        	fetch_instruction();
				        break;
					case S3 : 
		               	current_state.write(S4);
		               	alu_execution();
		   	        	decode_instruction();
			           	break;
		           	case S4 : 
		               	current_state.write(S5);
		               	memory_access();
			           	break;
					default:
						break;
				}
				
			} else {

				switch(current_state.read()) {
					case S1 :
						current_state.write(S2);
						decode_instruction();
						break;
					case S5 : 
		            	current_state.write(S6);
		            	write_register_file();
		            	current_state.write(S0);
			       		break;
					default:
						break;
				}

			}

		}
	}

template<class addr_size, class data_size>
void controller<addr_size, data_size>::fetch_instruction(){
	if(current_state.read() != S0 && instruction_register == 0) { 
				return;
			}

			program_counter_out.write(program_counter);
			program_memory_read.write(1);
			wait(10 , SC_PS);
			instruction_register = instruction_input.read();
			wait(10 , SC_PS);
			program_memory_read.write(0);
			if(instruction_register == 0) { 
				return;
			}
			program_counter += 1;
			cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl;
			cout<<"       Instruction : "<<instruction_register<<endl;
			cout<<"       Current Program Counter Value : "<<program_counter<<endl;
			cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl<<endl<<endl;

}
template<class addr_size, class data_size>
sc_uint<49> controller<addr_size, data_size>::prepare_instruction_control_signal(sc_uint<4> alu_control_signal , sc_uint<1> immediate_flag , sc_uint<4> register_dest_cond , sc_uint<4> opCode_ext_immH , sc_uint<4> register_source_immL){

            sc_uint<49> instruction_control_signal;
			instruction_control_signal.range(16 , 13) = alu_control_signal; 
			instruction_control_signal.range(12 , 12) = immediate_flag; 
			instruction_control_signal.range(11 , 8) = register_dest_cond;
			instruction_control_signal.range(7 , 4) = opCode_ext_immH; 
			instruction_control_signal.range(3 , 0) = register_source_immL;

			cout<<"       alu_control_signal : "<<alu_control_signal<<endl;
			cout<<"       immediate_flag : "<<immediate_flag<<endl;
			cout<<"       register_dest_cond : "<<register_dest_cond<<endl;
			cout<<"       opCode_ext_immH : "<<opCode_ext_immH<<endl;
			cout<<"       register_source_immL : "<<register_source_immL<<endl;

			return instruction_control_signal;
}
template<class addr_size, class data_size>
 void controller<addr_size, data_size>:: decode_instruction(){

 	if(instruction_register == 0) {
 		cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl;
		cout<<"       Stage : Instruction Decode"<<endl;
		cout<<"       Instruction Opcode : NOP"<<endl;
		cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl<<endl<<endl;
		return;
	}

			sc_uint<4> op_code = instruction_register.range(15 , 12);
			sc_uint<4> register_dest_cond = instruction_register.range(11 , 8);
			sc_uint<4> opCode_ext_immH = instruction_register.range(7 , 4);
			sc_uint<4> register_source_immL = instruction_register.range(3 , 0);
			sc_uint<49> instruction_control_signal;

			switch(op_code) {
				cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl;
			cout<<"       Instruction Opcode : "<<instruction_register<<endl;
				case 0b0000 : 
					switch(opCode_ext_immH) {
						case 0b0101 : 
						cout<<"       Instruction : ADD"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b0000 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b1001 : 
						cout<<"       Instruction : SUB"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b0001 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b1011 : 
						cout<<"       Instruction : CMP"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b0010 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b0001 : 
						cout<<"       Instruction : AND"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b0011 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b0010 : 
						cout<<"       Instruction : OR"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b0100 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b0011 : 
						cout<<"       Instruction : XOR"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b0101 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b1101 : 
						cout<<"       Instruction : MOV"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b0110 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
					}
					break;
				case 0b0101 : 
				cout<<"       Instruction : ADDI"<<endl;
					instruction_control_signal= prepare_instruction_control_signal(0b0000 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
					break;
				case 0b1001 : 
				cout<<"       Instruction : SUBI"<<endl;
					instruction_control_signal= prepare_instruction_control_signal(0b0001 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
					break;
				case 0b1011 : 
				cout<<"       Instruction : CMPI"<<endl;
					instruction_control_signal= prepare_instruction_control_signal(0b0010 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
					break;
				case 0b0001 : 
				cout<<"       Instruction : ANDI"<<endl;
					instruction_control_signal= prepare_instruction_control_signal(0b0011 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
					break;
				case 0b0010 : 
				cout<<"       Instruction : ORI"<<endl;
					instruction_control_signal= prepare_instruction_control_signal(0b0100 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
					break;
				case 0b0011 : 
				cout<<"       Instruction : XORI"<<endl;
					instruction_control_signal= prepare_instruction_control_signal(0b0101 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
					break;
				case 0b1101 : 
				cout<<"       Instruction : MOVI"<<endl;
					instruction_control_signal= prepare_instruction_control_signal(0b0110 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
					break;
				case 0b1000 : 
					switch(opCode_ext_immH) {
						case 0b0100 : 
						cout<<"       Instruction : LSH"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b0111 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b0000 : 
						cout<<"       Instruction : LSHI"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b0111 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b0110 : 
						cout<<"       Instruction : ASH"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b1000 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b0001 : 
						cout<<"       Instruction : ASHI"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b1000 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
					}
					break;
				case 0b1111 : 
				cout<<"       Instruction : LUI"<<endl;
					instruction_control_signal= prepare_instruction_control_signal(0b1001 , 1 , register_dest_cond , opCode_ext_immH , register_source_immL);
					break;
				case 0b0100 : 
					switch(opCode_ext_immH) {
						case 0b0000 : 
						cout<<"       Instruction : LOAD"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b1001 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b0100 : 
						cout<<"       Instruction : STOR"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b1010 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b1100 : 
						cout<<"       Instruction : Jcond"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b1100 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
						case 0b1000 : 
						cout<<"       Instruction : JAL"<<endl;
							instruction_control_signal= prepare_instruction_control_signal(0b1101 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
							break;
					}
					break;
				case 0b1100 : 
				cout<<"       Instruction : Bcond"<<endl;
					instruction_control_signal= prepare_instruction_control_signal(0b1011 , 0 , register_dest_cond , opCode_ext_immH , register_source_immL);
					break;
			}
			q_instruction_control_signal.push_back(instruction_control_signal);
			cout<<"       Instruction Control Signal : "<<instruction_control_signal<<endl;
			cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl<<endl<<endl;

			cout<<"@ "<<sc_time_stamp()<<"------End Decode Instruction--------"<<endl;

 }
 template<class addr_size, class data_size>
 void controller<addr_size, data_size>:: read_register_file(){

			sc_uint<48> current_instruct = q_instruction_control_signal.front();
			sc_uint<4> alu_signal = current_instruct.range(16 , 13);
			data_size operand1 , operand2;
			if(current_instruct.range(12 , 12)) { 
				register_file_addr1_out.write(current_instruct.range(11 , 8)); 
				sc_uint<16> temp_data = current_instruct.range(7,4); 
				temp_data = temp_data<<4;
				temp_data += current_instruct.range(3 , 0); 	
				register_file_data_out.write(temp_data);
				immediate_flag.write(1);
				register_file_read.write(true);
				wait(10 , SC_PS);
				operand1 = operand1_input.read();
				operand2 = operand2_input.read();
				wait(10 , SC_PS);
				register_file_read.write(false);
			}
			else {
				register_file_addr1_out.write(current_instruct.range(11 , 8));
				register_file_addr2_out.write(current_instruct.range(3 , 0)); 
				immediate_flag.write(0);
				register_file_read.write(true);
				wait(10 , SC_PS);
				operand1 = operand1_input.read();
				operand2 = operand2_input.read();
				wait(10 , SC_PS);
				register_file_read.write(false);
			}

			q_instruction_control_signal[0].range(32 , 17) = operand1;
			q_instruction_control_signal[0].range(48 , 33) = operand2;
			if(alu_signal == 0b1100 || alu_signal == 0b1011) { 
				q_instruction_control_signal[0].range(32 , 17) = current_instruct.range(3 , 0);
				q_instruction_control_signal[0].range(48 , 33) = program_counter; 
			}

			cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl;
			cout<<"       Instruction Control Signal : "<<q_instruction_control_signal[0]<<endl;
			cout<<"       Operand1 : "<<operand1<<endl;
			cout<<"       Operand2 : "<<operand2<<endl;
			cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl<<endl<<endl;


}
template<class addr_size, class data_size>
void controller<addr_size, data_size>::alu_execution(){

			sc_uint<49> current_instruct = q_instruction_control_signal.front();
			alu_control_signal.write(current_instruct.range(16 , 13));
			cond_control_signal.write(current_instruct.range(11 , 8));
			immediate_control_signal.write(current_instruct.range(12 , 12));
			operand1_output.write(current_instruct.range(32 , 17));
			operand2_output.write(current_instruct.range(48 , 33));
			enable_alu.write(1);
			wait(10 , SC_PS);
			data_size result = alu_result_input.read();
			wait(10 , SC_PS);
			enable_alu.write(0);

			q_instruction_control_signal[0].range(48 , 33) = result; 
cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl;
			cout<<"       Instruction Control Signal : "<<q_instruction_control_signal[0]<<endl;
			cout<<"       Result : "<<result<<endl;
			cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl<<endl<<endl;

}
template<class addr_size, class data_size>
 void controller<addr_size, data_size>::memory_access(){

 			if(!q_instruction_control_signal.size()) {
				return;
			}

			sc_uint<49> current_instruct = q_instruction_control_signal.front();
			sc_uint<4> alu_signal = current_instruct.range(16 , 13);
			if(alu_signal == 0b1001) { 
				data_memory_addr_out.write(current_instruct.range(48 , 33)); 
				data_memory_read.write(1);
				wait(10 , SC_PS);
				data_size result = dm_data_input.read();
				wait(10 , SC_PS);
				data_memory_read.write(0);
				current_instruct.range(48 , 33) = result;
			} else if(alu_signal ==  0b1010) { 
				data_memory_addr_out.write(current_instruct.range(32 , 17)); 
				data_memory_data_out.write(current_instruct.range(48 , 33)); 
				data_memory_write.write(1);
				wait(10 , SC_PS);
				wait(10 , SC_PS);
				data_memory_write.write(0);
			} else if(alu_signal == 0b1011 || alu_signal == 0b1100) { 
				program_counter = current_instruct.range(48 , 33);
			}
			cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl;
			cout<<"       Instruction Control Signal : "<<q_instruction_control_signal[0]<<endl;
			cout<<"/**<------------------------------------------------:: CONTROLLER LOG ::---------------------------------------------------->**/"<<endl<<endl<<endl;

}
 template<class addr_size, class data_size>
void controller<addr_size, data_size>:: write_register_file(){

			if(!q_instruction_control_signal.size()) {
				return;
			}

			sc_uint<49> current_instruct = q_instruction_control_signal.front();
			sc_uint<4> alu_signal = current_instruct.range(16 , 13);
			if(alu_signal != 0b0010 && alu_signal != 0b1010 && alu_signal != 0b1011 && alu_signal != 0b1100) { 
				register_file_addr1_out.write(current_instruct.range(11 , 8)); 
				register_file_data_out.write(current_instruct.range(48 , 33));
				register_file_write.write(1);
				wait(10 , SC_PS);
				wait(10 , SC_PS);
				register_file_write.write(0);
			}

			q_instruction_control_signal.erase(q_instruction_control_signal.begin());


}