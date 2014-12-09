#ifndef FILEMANAGER_H
#define FILEMANAGER_h
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <fstream>
#include <map>
#include <unistd.h>
#include "Event.h"
#include "BlockingQueue.h"
using namespace std;
class FileManager
{
	public:
	FileManager(char* filePath);
	char* checkDirectory();
	int readMessage(Event& e);
	private:
	 int notifyResult;
	 int dirWatch,i,length;
	 char* path;
};
#endif
