#include <string>
#include <map>
using namespace std;

//messages are stored in files
// inboxes/<to>/<from>/<msgid>.txt
#define DS "/"


class FileIPMLoop
{
	map<string/*from*/,uint64/*msgidcur*/> inboxes;
	map<string/*to*/,uint64/*msgidcur*/> outboxes;
	
	string messageRoot;
	bool terminated;
	int delayTime;
	string name;
	
	string formChannelName(const string& recipient,const string& sender=""){
		if(sender.length()==0)
		{
			return messageRoot+DS+recipient+DS+this->name;
		}else{
		
			return messageRoot+DS+recipient+DS+sender;
		}
	}
	
	uint64 sendMessage(const string& recipient,const string& msg)
	{
		map<string,uint64>::iterator i=outboxes.find(recipient);
		if(i==outboxes
	}
	void setDelay(int _delayTime){
		delayTime=_delayTime;
	}
	
	void loop()
	{
		terminated=false;
		while(!terminated){
			
			//check messages
			//TODO go to inbox folder, list folders, if num of folders > inboxes.size() scan folder and add them as msgid=1
			
			for(map<string,uint64>::iterator i=inboxes.begin();i!=inboxes.end();i++)
			{
			
				//if receive a message evoke onReceivingMessage method
				string sender=(*i)->first;
				string channelName=formChannelName(this->name,sender);
				vector<string> msgs;
				while(true){
					msgName=channelName+DS+StringUtil::str((*i)->second);
					
					if(!fexists(msgName))
						break;
					
					string msg=getFileContent(msgName);
					if(msg[0]=="!")
					{
						//system stuff
					}else{
					
						msgs.push_back(msg);
					}
					
					
					(*i)->second++;
					
				}
				
				if (msgs.size()>0)
				{
					this->onReceivingMessage(sender, msgs);
				}
			
			}
			
			sleep(delayTime);
		}
	}
	
	void terminate()
	{
		terminated=true;
	}
	
	abstract void onReceivingMessage(string sender,vector<string>& msgs)
	{
		for(vector<string>::iterator i=msgs.begin();i!=msgs.end();i++)
			cerr<<sender<<":"<<(*i)<<endl;
	}
};

