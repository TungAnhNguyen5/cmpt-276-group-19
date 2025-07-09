#ifndef SAILING_H
#define SAILING_H

#include <string>



//Sailing class used to interface with the file io for sailing class as well as create sailings.
//Also used to navigate through the manage sailings menu.
class Sailing 
{
    private:
        //sailingID is used to identify sailings. Formatted terminal-date-hour
        std::string sailingID;
        //vesselID represents the nme of the vessel used for the sailing
        std::string vesselID;
        //the total amount of space for regular vehicles on the vessel.
        float lcll;
        //the total amount of space for special vehicles on the vessel.
        float hcll;
        //the remaining space for regular vehicles on the vessel.
        float lrl;
        //the remaining space for special vehicles on the vessel.
        float hrl;
    
    public:
        //initializes a sailing object according to user input, returns false if already in database
        bool addSailing();
        //opens a sailing to be deleted and/or view the information saved on it
        void editSailing();
        //initializes this class as well as opens File I/O for Sailing class
        void initialize();
        //returns the value of the hrl variable
        int hrlRemaining() const;
        //returns the value of the lrl variable
        int lrlRemaining() const;
        //shuts down this class and closes File I/O for Sailing Class
        void shutdown(); 
        //decrements the hrl by the specified value
        void hrlUpdate(float f);
        //decrements lrl by the specified value
        void lrlUpdate(float f);
        //interfaces with File I/O for Sailing Class to run the display report menu
        static void displayReport();
}

#endif