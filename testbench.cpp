#include<systemc.h>
#include<program_memory.cpp>
#include<data_memory.cpp> 
#include<register_file.cpp> 
#include<controller.cpp> 
#include<alu.cpp> 


template <class rf_address_size , class pm_address_size , class dm_address_size , class data_size , unsigned register_count , unsigned dm_count>
SC_MODULE(test_bench) {

		sc_signal<bool> clock , enable_alu , program_memory_read , register_file_read , register_file_write , data_memory_read , data_memory_write;
		sc_signal<sc_uint<1> > immediate_flag , immediate_control_signal;
		sc_signal<sc_uint<4> >alu_control_signal , cond_control_signal;
		sc_signal<rf_address_size > register_file_addr1_out , register_file_addr2_out;
		sc_signal<dm_address_size > data_memory_addr_out;
		sc_signal<data_size > instruction , rf_data1_out , rf_data2_out , rf_immdata_out , dm_data_input , data_memory_data_out , alu_result_input , program_counter_out , operand1_output , operand2_output ;

		register_file<rf_address_size , data_size , register_count>* RF;
		data_memory<dm_address_size , data_size , dm_count>* DM;
		program_memory<pm_address_size>* PM;
		alu_block<data_size>* ALU;
		controller<rf_address_size , data_size>* CONTROLLER;

		void clock_signal();

	SC_CTOR(test_bench){

		RF = new register_file<rf_address_size , data_size , register_count> ("Register File");
		DM = new data_memory<dm_address_size , data_size , dm_count> ("Date Memory");
		PM = new program_memory<pm_address_size> ("Program Memory");
		ALU = new alu_block<data_size> ("ALU Block");
		CONTROLLER = new controller<rf_address_size , data_size> ("Controller");


		CONTROLLER->clock(clock);

		DM->dm_read(data_memory_read);
		CONTROLLER->data_memory_read(data_memory_read);

		DM->address(data_memory_addr_out);
		CONTROLLER->data_memory_addr_out(data_memory_addr_out);

		DM->dm_write(data_memory_write);
		CONTROLLER->data_memory_write(data_memory_write);

		DM->data_in(data_memory_data_out);
		CONTROLLER->data_memory_data_out(data_memory_data_out);

		PM->instruction_out(instruction);
		CONTROLLER->instruction_input(instruction);

		RF->data_out1(rf_data1_out);
		CONTROLLER->operand1_input(rf_data1_out);

		RF->data_out2(rf_data2_out);
		CONTROLLER->operand2_input(rf_data2_out);

		DM->data_out(dm_data_input);
		CONTROLLER->dm_data_input(dm_data_input);

		CONTROLLER->alu_result_input(alu_result_input);
		ALU->result(alu_result_input);

		CONTROLLER->program_counter_out(program_counter_out);
		PM->pc_address(program_counter_out);

		CONTROLLER->program_memory_read(program_memory_read);
		PM->memory_read(program_memory_read);
		
		CONTROLLER->register_file_read(register_file_read);
		RF->rf_read(register_file_read);

		CONTROLLER->register_file_write(register_file_write);
		RF->rf_write(register_file_write);

		CONTROLLER->immediate_flag(immediate_flag);
		RF->immediate_data(immediate_flag);

		CONTROLLER->register_file_addr1_out(register_file_addr1_out);
		RF->address1(register_file_addr1_out);

		CONTROLLER->register_file_addr2_out(register_file_addr2_out);
		RF->address2(register_file_addr2_out);

		CONTROLLER->register_file_data_out(rf_immdata_out);
		RF->data(rf_immdata_out);

		CONTROLLER->enable_alu(enable_alu);
		ALU->enable_alu(enable_alu);

		CONTROLLER->immediate_control_signal(immediate_control_signal);
		ALU->immediate_control_signal(immediate_control_signal);

		CONTROLLER->alu_control_signal(alu_control_signal);
		ALU->alu_control_signal(alu_control_signal);

		CONTROLLER->cond_control_signal(cond_control_signal);
		ALU->cond_control_signal(cond_control_signal);

		CONTROLLER->operand1_output(operand1_output);
		ALU->operand_1(operand1_output);

		CONTROLLER->operand2_output(operand2_output);
		ALU->operand_2(operand2_output);

		SC_THREAD(clock_signal);
	}

	
};

template <class rf_address_size , class pm_address_size , class dm_address_size , class data_size , unsigned register_count , unsigned dm_count>
void test_bench<rf_address_size , pm_address_size , dm_address_size , data_size , register_count , dm_count>:: clock_signal() {
	while (true){
	    wait(1 , SC_NS);
		clock = false;
		wait(1 , SC_NS);
		clock = true;
	}
}

int sc_main(int argc , char* argv[]) {
	cout<<"@ "<<sc_time_stamp()<<"----------Start---------"<<endl<<endl<<endl;
	test_bench<sc_uint<4> , sc_uint<16> , sc_uint<4> , sc_uint<16> , 256 , 256> processor("RISC Processor");
	std::vector<sc_uint<16> >  assembly_code;
	assembly_code.push_back(0b1101000000000000); // MVI 0 , A
	assembly_code.push_back(0b1101000100000001); // MVI 1 , B
	assembly_code.push_back(0b1101001000001001); // MVI 9 , C
	assembly_code.push_back(0b0000000001010001); // ADD B , A
	assembly_code.push_back(0b0101000100000001); // ADI 1 , B
	assembly_code.push_back(0b1001001000000001); // SUBI 1 , C
	assembly_code.push_back(0b1011001000000000); // CMPI 0 , C
	assembly_code.push_back(0b0100000111000011); // JNZ 3
	assembly_code.push_back(0b0000000000000000); // NOP

	processor.PM->program_memory_write(assembly_code);
	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl<<endl<<endl;
	sc_start(600, SC_NS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl<<endl<<endl;
	return 0;
}
