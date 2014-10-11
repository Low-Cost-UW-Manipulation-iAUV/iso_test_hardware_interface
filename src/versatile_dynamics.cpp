/*
*		Versatile Dynamics Class
*		This class implements an IIR filter to model dynamics
*
*		The design is (heavily) inspired by Dr. Alexander Lenz' (BRL) Dynamics Class.
*
*		Author: Raphael Nagel
*		Date: 25/Sept/2014
*/

#include "test_robot/versatile_dynamics.hpp"
#include <cstdlib>
#include <stdio.h>
versatile_dynamics::versatile_dynamics(void){
	out.clear();
	in.clear();

}
///Creating the object while directly giving it the numerator b, denominator a and update period info.
versatile_dynamics::versatile_dynamics(std::vector<double> b_extern, std::vector<double> a_extern){
	if(b_extern.size() == a_extern.size()){
		vector_size = b_extern.size();

		b.resize(vector_size);
		a.resize(vector_size);
		b = b_extern;
		a = a_extern;

		out.resize(vector_size);
		in.resize(vector_size);

		unsigned int x;
		for(x = 0; x < vector_size; x++){
			in[x] = 0.0;
			out[x] = 0.0;
		}

	}
	printf("coefficent lists b and a are not of same length \n");

}

versatile_dynamics::~versatile_dynamics(void){}


int versatile_dynamics::reset(std::vector<double> b_extern, std::vector<double> a_extern){
	if(b_extern.size() == a_extern.size()){
		set_b(b_extern);
		set_a(a_extern);
		return EXIT_SUCCESS;
	}else{
		return EXIT_FAILURE;
	}
}

void versatile_dynamics::reset(void){
	a.clear();
	b.clear();

	out.clear();
	in.clear();	
}

void versatile_dynamics::set_a(std::vector<double> a_extern){
	unsigned int x;
	vector_size = a_extern.size();

	a.resize(vector_size);
	a = a_extern;

	out.resize(vector_size);
	for(x = 0; x < vector_size; x++){
		out[x] = 0.0;
	}	
}

void versatile_dynamics::set_b(std::vector<double> b_extern){
	unsigned int x;
	vector_size = b_extern.size();

	b.resize(vector_size);
	b = b_extern;

	in.resize(vector_size);
	for(x = 0; x < vector_size; x++){
		in[x] = 0.0;
	}	
}	




///If a controller is feeding the sytem and it is running at a lower frequency you can use this one.
int versatile_dynamics::put_in(double force, unsigned int num_of_periods_until_next){
	if(b.size() == a.size() ){
		unsigned int x;
		for(x = 0; x < num_of_periods_until_next ; x++){
			shift(force);
			IIR();
		}


		return EXIT_SUCCESS;
	}else{
		printf("coefficent lists b and a are not of same length \n");
		return EXIT_FAILURE;
	}
}

///put in a single force input
int versatile_dynamics::put_in(double force){
	if(b.size() == a.size() ){
		shift(force);
		IIR();

		return EXIT_SUCCESS;
	}else{
		printf("coefficent lists b and a are not of same length \n");
		return EXIT_FAILURE;
	}
}

void versatile_dynamics::get_out(double *force){
	*force = out[0];
}

double versatile_dynamics::get_out(void){
	return out[0];
}

void versatile_dynamics::shift(double force){
	unsigned int x;
	///Starting from the top shift the data down. We will lose the LS double
	for(x = (vector_size - 1); x > 0; x--){
		out[x] = out[x-1];
		in[x] = in[x-1];

	}
	out[0] = 0;
	in[0] = force;
}

///Run the IIR filter process
void versatile_dynamics::IIR(void){
	unsigned int x;

	for(x = 0; x < vector_size; x++){
		out[0] = out[0] +   b[x] * in[x] - a[x] * out[x] ;
	}
}





