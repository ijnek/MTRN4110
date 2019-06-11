#include "example.h"

void setup () {
	// put your setup code here, to run once:
	example::hello_world<2>::enable ();
	example::hello_world<5>::enable ();

	example::hello_world<2>::print (); // Print "Hello world\n" twice.
	example::hello_world<5>::print (); // Print "Hello world\n" another five times.
	// Total print seven times.
}

void loop () {
	// put your main code here, to run repeatedly:
}
