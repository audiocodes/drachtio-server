/****************************************************************************
The MIT License (MIT)

Copyright (c) 2024, FirstFive8, Inc

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*******************************************************************************/
#define DRACHTIO_MAIN

#include "controller.hpp"
#include <stdlib.h>
#include <time.h>

using namespace drachtio ;

void handleSigHup( int signal ) {
	theOneAndOnlyController->handleSigHup( signal ) ;
}
void handleSigTerm( int signal ) {
  theOneAndOnlyController->handleSigTerm( signal ) ;  
}
void handleSigPipe( int signal ) {
  theOneAndOnlyController->handleSigPipe( signal ) ;  
}

int main( int argc, char *argv[] ) {
	srand(time(NULL));

	try {
		theOneAndOnlyController = new DrachtioController( argc, argv ) ;
		signal( SIGHUP, handleSigHup ) ;
    signal( SIGTERM, handleSigTerm ) ;
    signal( SIGPIPE, handleSigPipe ) ;
		theOneAndOnlyController->run() ;
	} catch( std::runtime_error& err ) {
		cerr << "Uncaught exception: " << err.what() << endl ;
	}
	return 0 ;
}

