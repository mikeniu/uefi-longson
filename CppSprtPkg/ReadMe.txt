1. In order to support C++ application, you need to do the following two things first :
  -1) Repalce the default compiler with "gcc.sh"
  -2) Replace the default link script with "GccBase.lds".

2. The C++ program entry should use the following form :

  INT32
  CppMain (
    IN int Argc,
    IN char **Argv
    )
  {

  }

