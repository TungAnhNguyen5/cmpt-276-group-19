#include "sailing.h"
#include "sailingFileIO.h"
#include <cstring>

int main()
{
    Sailing::initialize();
    Sailing::addSailing();
    //Sailing::getSailingFromIO("444-22-12").Sailing::editSailing();
    //Sailing::displayReport();
    // cout << s.Sailing::lrlRemaining() << '\n';
    // cout << s.Sailing::hrlRemaining();
    //Sailing::getSailingFromIO("232-22-12").Sailing::lrlUpdate(2);
    /* Sailing::addSailing();
     string input;
     cout<< "do\n";
     cin.clear();
     cin >> input;
     if (sailingFileIO::exists(input.c_str()))
     {
         Sailing::getSailingFromIO(input.c_str()).Sailing::editSailing();
     }*/
    Sailing::shutdown();
    return 0;
}