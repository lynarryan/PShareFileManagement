#include "filemanager.h"
FileManager::FileManager(char* filePath)
{
	struct stat dirCheck;
	this->path = filePath;
	notifyResult = inotify_init();// new map<char*,int>();

	//notifyResultFILE_PATH,inotify_init());
	i = 0;
	length = 0;
	if(notifyResult<0){
		cout<<"Error on creation of notify\n";
	}
	//assigning the inotify watcher
	if(!(stat(this->path,&dirCheck) ==0 && S_ISDIR(dirCheck.st_mode))){
		if((mkdir(this->path,S_IRWXU|S_IRWXG|S_IRWXO)) == -1){
			cout<<"Could not make directory\n";
		}
	}
	cout<<"Directory was made at " << filePath <<'\n';
//	cout<<inotify_add_watch(notifyResult,this->path,IN_ALL_EVENTS)<<'\n';
	if((dirWatch = inotify_add_watch(notifyResult,this->path,IN_ALL_EVENTS)) == -1){

		cout<<"Error creating notify on " <<this->path<<'\n';
	}
}	
char* FileManager::checkDirectory(){	
	char buffer [1024 * (sizeof(struct inotify_event) + 16)];
	string retVal="";
	char* temp;
	length = read(notifyResult,buffer,(1024*(sizeof(struct inotify_event)+16)));
	while(i<length){
		//Essentially building the payload object for the network message
		struct inotify_event *event = (struct inotify_event *) &buffer[i];
		retVal.append(event->name);
		retVal.append(",");
		if(event->mask & IN_CREATE || event->mask & IN_MOVED_TO){
			if(event->mask & IN_ISDIR){
				retVal.append("mkDir");
				
			}else{
				retVal.append("mkFile");
				
			}
		}else if(event->mask & IN_DELETE|| event->mask & IN_MOVED_FROM){
			if(event->mask & IN_ISDIR ){
				retVal.append("rmDir");
				
			}else{
				retVal.append("rmFile");	
			}
		}//else if(event->mask & IN_MODIFY){
		//		retVal.append("modFile");
		//}
		else{return "";}
	i =0;
	length =0;
	temp = &retVal[0];
	return temp;
	}

	return "";
}
int FileManager::readMessage(Event& e){
	int splitPoint = 0;
	string msg = static_cast<char*>(e.get_data());
	if((splitPoint =msg.find(","))==string::npos){
		return -1;
	}
	string result="";
	string name = msg.substr(0,splitPoint);
	string operation  = msg.substr(splitPoint+1);	
	cout<<name << " " << operation;	
	result.append(this->path);
	result.append("/");
	result.append(name);
	
	if(operation.find("mkDir")!= string::npos){
		
		char* dirName = &result[0];	
		 mkdir(dirName,S_IRWXU|S_IRWXG|S_IRWXO);
		//if((dirWatch = inotify_add_watch(notifyResult,dirName,IN_ALL_EVENTS))!=0){
		//	cout<<"Something Went Wrong\n";
		//}		
	}
	else if(operation.find("mkFile") != string::npos){
		char* fileName = &result[0];
		FILE *f = fopen(fileName,"w+");
		fclose(f);		
		
	}
	else if(operation.find("rmDir") != string::npos){ //Running into an issue where removing a direcotry needs to remove the specific watch to it
		char* dirPath = &result[0];
		remove(dirPath);
	//	inotify_rm_watch(notifyResult,dirPath);
	}
	else if(operation.find("rmFile")!= string::npos){
		char*filePath = &result[0];
		remove(filePath);
	
	}//else{
	//	FILE*f = fopen(
	//}
	return 0;
}
/*int main(){
	FileManager* manager = new FileManager();
		BlockingQueue<Event> eq;
		EventRegistrar* r = new EventRegistrar(eq);
		char*msg;
	while(1){
		if((msg =manager->checkDirectory())!=NULL){		
			Event* e = new Event(*r,msg,Event::Flags::NODE_MSG_RECEIVED);
			manager->readMessage(e);
		}
	}
}*/	
