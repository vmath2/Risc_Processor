#include<systemc.h>

template <class address_size>
SC_MODULE(program_memory) {

	sc_in<address_size> pc_address;
	sc_in<bool> memory_read;

	sc_out<sc_uint<16> > instruction_out;
	std::vector<address_size> program_store;

	SC_CTOR(program_memory)
    {
    	SC_METHOD(program_memory_read);
		sensitive<<memory_read.pos();
	}

	void program_memory_read();
	void program_memory_write(std::vector<sc_uint<16> >  program_load);
};
template<class address_size>
void program_memory<address_size>:: program_memory_write(std::vector<sc_uint<16> >  program_load) {

		sc_uint<5> length = program_load.size();
		for(sc_uint<5> i ; i < length ; i++) {
			program_store.push_back(program_load[i]);
				cout<<"/**<------------------------------------:: PROGRAM MEMORY LOG ::--------------------------------------->**/"<<endl;
				cout<<"       Address Input : "<<i<<endl;
				cout<<"       Instruction Output : "<<program_load[i]<<endl;
				cout<<"/**<------------------------------------:: PROGRAM MEMORY LOG ::--------------------------------------->**/"<<endl<<endl<<endl;
		}
	}
template<class address_size>
void program_memory<address_size>:: program_memory_read() { 

		address_size address = pc_address.read();
		if(address <program_store.size()){
		instruction_out.write(program_store[address]);
		cout<<"/**<------------------------------------:: PROGRAM MEMORY LOG ::--------------------------------------->**/"<<endl;
				cout<<"       Address Input : "<<address<<endl;
				cout<<"       Instruction Output : "<<program_store[address]<<endl;
				cout<<"/**<------------------------------------:: PROGRAM MEMORY LOG ::--------------------------------------->**/"<<endl<<endl<<endl;
	}else{
		cout<<"/**<------------------------------------:: PROGRAM MEMORY LOG ::--------------------------------------->**/"<<endl;
				cout<<"       Address Input : "<<address<<endl;
				cout<<"       Instruction Output : NOP Found"<<endl;
				cout<<"/**<------------------------------------:: PROGRAM MEMORY LOG ::--------------------------------------->**/"<<endl<<endl<<endl;
		
	}
	
}
