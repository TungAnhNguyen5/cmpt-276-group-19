#include "sailingFileIO.h"

using namespace std;

const string FILE_NAME = "sailingData";
fstream sailingFileIO::file;

bool sailingFileIO::closeFile()
{
    file.close();
    return !file.is_open();
}

bool sailingFileIO::openFile()
{
    file.open(FILE_NAME, ios::in | ios::out | ios::binary);
    // if opening doesn't work, creates the file
    if (!file.is_open())
    {
        createFile();
        file.open(FILE_NAME, ios::in | ios::out | ios::binary);
    }
    return file.is_open();
}

void sailingFileIO::reset()
{
    file.clear();
    file.seekg(0);
}

Sailing sailingFileIO::getSailing(const char *sid)
{
    reset();

    Sailing toFind;
    while (file.read(reinterpret_cast<char *>(&toFind), sizeof(Sailing)))
    {
        if (strncmp(toFind.getSailingID(), sid, sizeof(toFind.getSailingID())) == 0)
        {
            return toFind;
        }
    }
    return toFind;
}

Sailing *sailingFileIO::getNextFive()
{
    Sailing *fiveSailings = new Sailing[5];

    for (int i = 0; i < 5 && !file.eof(); i++)
    {
        file.read(reinterpret_cast<char *>(&fiveSailings[i]), sizeof(Sailing));
    }

    return fiveSailings;
}

bool sailingFileIO::exists(const char *sid)
{
    reset();
    Sailing toFind;
    // iterates through the file until it finds the sailingID or reaches eof.
    while (file.read(reinterpret_cast<char *>(&toFind), sizeof(Sailing)))
    {
        if (strncmp(toFind.getSailingID(), sid, sizeof(toFind.getSailingID())) == 0)
        {
            return true;
        }
    }
    return false;
}

bool sailingFileIO::saveSailing(const Sailing s)
{
    if (file.is_open())
    {
        reset();
        Sailing temp;
        streampos pos;
        bool found = false;

        // searches for the sailing in the case of needing to overwrite
        while (file.read(reinterpret_cast<char *>(&temp), sizeof(Sailing)))
        {
            if (strncmp(temp.getSailingID(), s.getSailingID(), sizeof(temp.getSailingID())) == 0)
            {
                // if it finds it, saves the pos

                pos = file.tellg();
                found = true;
                break;
            }
        }

        if (found)
        {
            file.seekp(pos - static_cast<std::streamoff>(sizeof(Sailing)), ios::beg);
        }
        else
        {
            // if it can't find the sailing, saves at the end
            file.clear();
            file.seekp(0, ios::end);
        }
        file.write(reinterpret_cast<const char *>(&s), sizeof(Sailing));
        file.flush();
    }
    else
    {
        cout << "file not open";
        return false;
    }

    return true;
}

bool sailingFileIO::deleteSailing(const char *sid)
{
    if (file.is_open())
    {
        reset();
        Sailing temp;
        streampos pos;
        bool found = false;

        // searches for the sailing to be deleted
        while (file.read(reinterpret_cast<char *>(&temp), sizeof(Sailing)))
        {
            if (strncmp(temp.getSailingID(), sid, sizeof(temp.getSailingID())) == 0)
            {
                // if it finds it, saves the pos
                pos = file.tellg();
                found = true;
                break;
            }
        }

        //unable to delete a sailing if not found
        if (!found)
        {
            return false;
        }
        // rewrites the specified sailing with the last one
        Sailing s = getLast();
        reset();
        file.seekp(pos - static_cast<std::streamoff>(sizeof(Sailing)), ios::beg);
        file.write(reinterpret_cast<const char *>(&s), sizeof(Sailing));
        file.flush();

        // truncates the file
        truncateFile();
    }
    else
    {
        cout << "file not open";
        return false;
    }

    return true;
}

void sailingFileIO ::truncateFile()
{
    reset();
    int fd = open(FILE_NAME.c_str(), O_RDWR);
    Sailing temp;
    int numOfSailings = 0;
    while (file.read(reinterpret_cast<char *>(&temp), sizeof(Sailing)))
    {
        numOfSailings++;
    }
    ftruncate(fd, (numOfSailings - 1) * sizeof(Sailing));
    close(fd);
}

Sailing sailingFileIO::getLast()
{
    reset();
    Sailing last;
    while (file.read(reinterpret_cast<char *>(&last), sizeof(Sailing)))
    {
    }
    return last;
}

void sailingFileIO::createFile()
{
    ofstream f(FILE_NAME, ios::out | ios::binary);
    f.close();
}