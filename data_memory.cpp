#include<systemc.h>

template <class address_size , class data_size , unsigned dm_count>
SC_MODULE(data_memory) {

	sc_in<address_size> address;
	sc_in<data_size> data_in;
	sc_in<bool> dm_read;
	sc_in<bool> dm_write;

	sc_out<data_size> data_out;
	data_size memory[dm_count];

	SC_CTOR(data_memory)
    {
    	SC_METHOD(data_memory_read);
		sensitive<<dm_read.pos();

		SC_METHOD(data_memory_write);
		sensitive<<dm_write.pos();
	}

	void data_memory_read();
	void data_memory_write();
};
template <class address_size , class data_size , unsigned dm_count>
void data_memory<address_size, data_size, dm_count>:: data_memory_read() {
	data_out.write(memory[address.read()]) ;
	cout<<"/**<--------------------------------------------:: DATA MEMORY LOG ::--------------------------------------------->**/"<<endl;
			cout<<"       Address Input : "<<address.read()<<endl;
			cout<<"       Value Output : "<<memory[address.read()]<<endl;
			cout<<"/**<--------------------------------------------:: DATA MEMORY LOG ::--------------------------------------------->**/"<<endl<<endl<<endl;
}
template <class address_size , class data_size , unsigned dm_count>
void data_memory<address_size, data_size, dm_count>:: data_memory_write() {
	memory[address.read()] = data_in.read();
	cout<<"/**<--------------------------------------------:: DATA MEMORY LOG ::--------------------------------------------->**/"<<endl;
			cout<<"       Address Input : "<<address.read()<<endl;
			cout<<"       Value Input : "<<data_in.read()<<endl;
			cout<<"/**<--------------------------------------------:: DATA MEMORY LOG ::--------------------------------------------->**/"<<endl<<endl<<endl;
}