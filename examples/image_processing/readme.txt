The examples/image_processing directory
 
This directory intends to be a repository of image processing 
algorithms tests, therefore all tests can be seen as examples 
of how to use the algorithms classes.
 
1 - Directory hierarchy:
 
image_processing
  |
  |-- bin - A place for the compiled tests binaries.
  |
  |-- resources - A place of data used by all the tests ( test images, i.e. ).
  |
  |-- souce - The source of all testes, organized by directories.
  
  
2 - Compiling all testes

  Windows : 
    Use the included solution project ( image_processing.sln ) at the top level 
    directory. A recursive compilation will be performed and the compiled 
    binaries will be generated inside the "bin"  directory, 
    from where each one can be executed.

  Linux: 
    Just use make at the top level directory. A recursive compilation will be 
    performed using the QT project files and the compiled binaries will be 
    generated inside the "bin"  directory, from where each one can be executed.

NOTE: Requirement : PDIAGN_DEBUG_MODE flag must be used when compiling
libPDI for use with tests.


