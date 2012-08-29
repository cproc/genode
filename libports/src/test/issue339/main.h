#include <fstream>
#include <util/string.h>
#include <timer_session/connection.h>
#include <base/printf.h>

using namespace std;

class FstreamTest 
{
    private :
        fstream stat_file;
        Timer::Connection timer;

    public : 
        void File_Open( string filename );


};

