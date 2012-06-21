#include <string>
#include <map>
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include "StringUtil.h"
using namespace std;
#include <inttypes.h>

typedef unsigned int uint64;
//messages are stored in files
// inboxes/<to>/<from>/<msgid>.txt
#define DS "/"


class FileIPMLoop
{
	public:
	map<string/*from*/,uint64/*msgidcur*/> inboxes;
	map<string/*to*/,uint64/*msgidcur*/> outboxes;
	
	string messageRoot;
	bool terminated;
	string name;
	int delayTime;	
	string inboxPath;
	
	FileIPMLoop(string _messageRoot,string _name,int _delayTimeInSeconds):messageRoot(_messageRoot),name(_name),delayTime(_delayTimeInSeconds){
		mkdirs(_messageRoot);
		mkdirs(_messageRoot+DS+_name);
		inboxPath=_messageRoot+DS+_name;
	}
	
	
	string formChannelName(const string& recipient,const string& sender=""){
		if(sender.length()==0)
		{
			return messageRoot+DS+recipient+DS+this->name;
		}else{
		
			return messageRoot+DS+recipient+DS+sender;
		}
	}
	
	string formMessageName(const string& channelName,uint64 msgid){
		return 	channelName+DS+StringUtil::str(msgid);
	}
	
	
	bool fexists(const string& path){
		struct stat sb;
		return stat(path.c_str(),&sb)==0;
	}
	
	void mkdirs(const string&path){
		
		if( fexists(path)){
			return;	
		}
		vector<string> components;
		StringUtil::split(path,DS,components);
		
		string curpath;
		for(int i=0;i<components.size();i++)
		{
			curpath+=components[i]+"/";
			if(!fexists(curpath)){
				//cerr<<"try to make "<<curpath<<endl;
				mkdir(curpath.c_str(),S_IRWXU | S_IRWXG | S_IRWXO);
			}
		}
		
		
	}

	
	uint64 sendMessage(const string& recipient,const string& msg)
	{	
		string channelName=formChannelName(recipient);
		uint64 msgid;
		map<string,uint64>::iterator i=outboxes.find(recipient);
		if(i==outboxes.end()){
			//new!
			mkdirs(channelName);
			msgid=1;
			pair<map<string,uint64>::iterator,bool> result=outboxes.insert(map<string,uint64>::value_type(recipient,1));	
			i=result.first;
		}else{
			
			msgid=i->second;	
		}
		
		
		
		string msgName=formMessageName(channelName,msgid);
		
		ofstream fout(msgName.c_str());
		fout<<msg; //<<endl;
		fout.close();
		
		
		return i->second++;  //increment msgid for next msg
		
	}
	void setDelay(int _delayTime){
		delayTime=_delayTime;
	}
	
	void getFileContent(const string& filename,string& dest)
	{	

		ifstream fil(filename.c_str());
		fil.seekg(0,ios::end);
		int length=fil.tellg();
		fil.seekg(0,ios::beg);
		char *buffer=new char[length+1];
		fil.read(buffer,length);
		buffer[length]='\0';
		dest=buffer;
		delete[] buffer;
		fil.close();
	}
	
	int listVisibleDir(const string& path,vector<string>& entries) {
			 entries.clear();
		  struct dirent *entry;
		  DIR *dp;
		 
		  dp = opendir(path.c_str());
		  if (dp == NULL) {
		    cerr<<"opendir: Path does not exist or could not be read."<<endl;
		    return -1;
		  }
		 
		 struct stat s;
		  while((entry = readdir(dp))){
		   
		    if(stat((path+DS+entry->d_name).c_str(),&s)==0 && s.st_mode& S_IFDIR){
		     if(entry->d_name[0]!='.'){
		   	 entries.push_back(entry->d_name);
		     }
		    }
		  }
		  closedir(dp);
		  return 0;
		}
		 
		 
		 
	void loop()
	{
		terminated=false;
		
		
		
		vector<string> senders;
		while(!terminated){
			//cerr<<"check inbox"<<endl;
			//check messages
			//TODO go to inbox folder, list folders, if num of folders > inboxes.size() scan folder and add them as msgid=1
			listVisibleDir(inboxPath,senders);
			
			if(senders.size()>inboxes.size()){
				//new sender!
				for(vector<string>::iterator i=senders.begin();i!=senders.end();i++){
					if(inboxes.find(*i)==inboxes.end()){
						inboxes.insert(map<string,uint64>::value_type(*i,1));	
					}	
				}	
			}
			
			for(map<string,uint64>::iterator i=inboxes.begin();i!=inboxes.end() && !terminated;i++)
			{
			
				//if receive a message evoke onReceivingMessage method
				string sender=i->first;
				string channelName=formChannelName(this->name,sender);
				vector<string> msgs;
				string msg;
				while(true){
					string msgName=formMessageName(channelName,i->second);
					
					if(!fexists(msgName))
						break;
					
					getFileContent(msgName, msg);
					if(msg[0]=='!')
					{
						//system stuff
						if(msg.substr(1,9)=="TERMINATE"){
							terminate();
							break;	
						}
					}else{
					
						msgs.push_back(msg);
					}
					
					
					i->second++;
					
				}
				
				if (msgs.size()>0 ) //&& !terminated
				{
					this->onReceivingMessage(sender, msgs);
				}
			
			}
			
			if(!terminated)
				sleep(delayTime);
		}
	}
	
	void terminate()
	{
		terminated=true;
	}
	
	virtual void onReceivingMessage(string sender,vector<string>& msgs)
	{
		for(vector<string>::iterator i=msgs.begin();i!=msgs.end();i++)
			cerr<<sender<<":"<<(*i)<<endl;
	}
};

