#include"Utility.h"

bool CreateConsole()
{
	FreeConsole();        
	if(AllocConsole()){
		int hCrt = _open_osfhandle((long)
		GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		*stdout = *(::_fdopen(hCrt, "w"));
		::setvbuf(stdout, NULL, _IONBF, 0);
		*stderr = *(::_fdopen(hCrt, "w"));
		::setvbuf(stderr, NULL, _IONBF, 0);
		
		return true;
    }
	else return false;
}