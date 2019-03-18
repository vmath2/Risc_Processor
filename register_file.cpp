#include<systemc.h>

template <class address_size , class data_size , unsigned register_count>
SC_MODULE(register_file) {

		sc_in<address_size> address1;
		sc_in<address_size> address2;
		sc_in<data_size> data;
		sc_in<bool> rf_read;
		sc_in<bool> rf_write;
		sc_in<sc_uint<1> > immediate_data;

		sc_out<data_size> data_out1;
		sc_out<data_size> data_out2;

		sc_uint<16> const data_zero = 0x0000;
		data_size registers[register_count];
		data_size immediate_registers[register_count];

 		SC_CTOR(register_file){

			SC_METHOD(register_file_read);
			sensitive<<rf_read.pos();

			SC_METHOD(register_file_write);
			sensitive<<rf_write.pos();
		}

		void register_file_read();
		void register_file_write();
};
template <class address_size , class data_size , unsigned register_count>
void register_file<address_size,data_size,register_count>:: register_file_read () { 

			sc_uint<1> is_immediate = immediate_data.read();
			sc_uint<4> addr1 = address1.read();
			sc_uint<4> addr2 = address2.read();
			if(is_immediate) {
				immediate_registers[addr2] = data.read();
			}
			sc_uint<16> out1 = registers[addr1];
			sc_uint<16> out2 = is_immediate == 1 ? immediate_registers[addr2] : registers[addr2];

			data_out1.write(out1);
			data_out2.write(out2) ;

			cout<<"/**<-----------------------------------------:: REGISTER FILE ::------------------------------------>**/"<<endl;
			cout<<"       Address1 Input : "<<addr1<<endl;
			cout<<"       Address2 Input : "<<addr2<<endl;
			cout<<"       Is Immediate Value : "<<is_immediate<<endl;
			
			if(is_immediate) {
				cout<<"       Immediate Value : "<<data.read()<<endl;
			}

			cout<<"       Data1 Output : "<<out1<<endl;
			cout<<"       Data2 Output : "<<out2<<endl;
			cout<<"/**<-----------------------------------------:: REGISTER FILE ::------------------------------------>**/"<<endl<<endl<<endl;
			
}
template <class address_size , class data_size , unsigned register_count>
void register_file<address_size,data_size,register_count>:: register_file_write () { 
			
			registers[address1.read()] = data.read();

			cout<<"/**<-----------------------------------------:: REGISTER FILE ::------------------------------------>**/"<<endl;
			cout<<"       Address Input : "<<address1.read()<<endl;
			cout<<"       Value Input : "<<data.read()<<endl;
			cout<<"/**<-----------------------------------------:: REGISTER FILE ::------------------------------------>**/"<<endl<<endl<<endl;
			
}