/**
* iPhoneInterface - an iPhone command shell for use with iTunesMobileDevice.dll
*
* Authors: geohot, ixtli, nightwatch, warren, ziel
*          Mac support by nall and Operator
*
* $Id: main.cpp 31 2007-07-09 09:05:45Z iphonedev $
**/

#if defined(WIN32)
#include <CoreFoundation.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <dlfcn.h>
#endif

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
#include "../Common/MobileDevice.h"
#include <vector>

#if !defined(WIN32)
#define __cdecl
#endif

//bump middle # when you add any new features, right # when you fix bugs
#define VERSION "0.3.3"

using namespace std;

typedef int (__cdecl * cmdsend)(am_recovery_device *,void *);
cmdsend wsendCommandToDevice;
int shell();

int sessionid;
am_device *iPhone;
bool run;
bool connected,rconnected;

afc_connection *hAFC;

string shellpwd="/";

/*
Shell binary search tree functions
*/
typedef int (*funcptr)(string *arg);
struct node
{
	node *rchild;
	node *lchild;
	//node *partent;
	string *val;
	funcptr fxn;

	node(char *value)
	{
		val = new string(value);
	}
	node(char *value, funcptr funct)
	{
		val = new string(value);
		fxn = funct;
		rchild = NULL;
		lchild = NULL;
	}
	~node()
	{
		delete val;
		if(rchild){delete rchild;}
		if(lchild){delete lchild;}
	}
};

void insertnode(node *head, node *n)
{
	if( !(n->val->compare(*(head->val))) )
	{	
		if( !(head->lchild) )
		{
			head->lchild = n;
			n->lchild = NULL;
			n->rchild = NULL;
		} else {
			insertnode(head->lchild, n);
		}
	} else {
		if ( !(head->rchild) )
		{
			head->rchild = n;
			n->lchild = NULL;
			n->rchild = NULL;
		} else {
			insertnode(head->rchild, n);
		}
	}
}

node *getNodeByKey(string *val, node *head)
{
	if( head->val->compare(*val) == 0 )
	{
		//cout << "Found key " << *(val) << endl;
		return head;
	}

	if( head->lchild != NULL && !(val->compare(*(head->val))) )
	{
		return getNodeByKey(val, head->lchild);

	} else if (head->rchild != NULL ) {
		return getNodeByKey(val, head->rchild);

	} else {
		return NULL;
	}
	return NULL;
}

funcptr getFxnPointer(string *val, node *head)
{
	node *result = getNodeByKey(val, head);
	if (!result) return NULL;
	return result->fxn;
}

void searchIncompleteString(string *searchString, vector<node *> *results, node *head)
{

	if( 	searchString->length() < head->val->length() || 
		searchString->compare( *(head->val) ) == 0	)
	{
		if( searchString->compare(head->val->substr(0,searchString->length())) == 0 )
		{
			results->push_back(head);
		}
	}

	if(head->lchild){searchIncompleteString(searchString, results, head->lchild);}
	if(head->rchild){searchIncompleteString(searchString, results, head->rchild);}
}

void describe255(CFTypeRef tested) { 
	char buffer[256]; 
	CFIndex got; 
	CFStringRef description = CFCopyDescription(tested); 
	CFStringGetBytes(description, 
		CFRangeMake(0, CFStringGetLength(description)), 
		CFStringGetSystemEncoding(), '?', TRUE, (UInt8 *)buffer, 255, &got); 
	buffer[got] = (char)0; 
	fprintf(stdout, "%s\n", buffer); 
	CFRelease(description); 
} 

//This is mad ghetto
//use CFShow instead?
void GhettoCFStringPrint(void *str) {
	unsigned char* gstr = (unsigned char*)str;
	int a=9;
	while(gstr && gstr[a]!=0) {
		cout << gstr[a]; a++;
	}
	cout << endl; 
}

void hexdump(void *memloc, int len) {
	unsigned char* hexdump = (unsigned char*)memloc;

	for(int a=0; a<len; a++) {
		if((a-8)%16==0) cout << " ";
		if(a%16==0&&a!=0) cout << endl;
		if(hexdump[a]<16) cout << "0";
		printf("%x ", hexdump[a]);
	} 
	cout << endl;
}

bool connect() {   
	if(AMDeviceConnect(iPhone)) { 
		int connid;
		cout << "can't connect, trying in recovery mode..." << endl;
		connid = AMDeviceGetConnectionID(iPhone);
		cout << "Connection ID = " << connid << endl;
		void* cfunknown=AMRestoreModeDeviceCreate(0,connid,0);
		hexdump(cfunknown,32);
		describe255(cfunknown);
		return false;
	}
	if(!AMDeviceIsPaired(iPhone)) { cout << "failed, Pairing Issue" << endl; return false; }
	if(AMDeviceValidatePairing(iPhone)!=0) { cout << "failed, Pairing NOT Valid" << endl; return false; }
	if(AMDeviceStartSession(iPhone))  { cout << "failed, Session NOT Started" << endl; return false; }

	cout << "established." << endl;

	return true;
}

// list directory @ pwd
void ls(afc_connection* hAFC, string pwd) {

	afc_directory *hAFCDir;

	if (0 == AFCDirectoryOpen(hAFC, (char *)pwd.c_str(), &hAFCDir)){
		char *buffer = NULL;
		AFCDirectoryRead(hAFC, hAFCDir, &buffer);

		while(buffer!=NULL) {
			printf("%s\n", buffer);
			AFCDirectoryRead(hAFC, hAFCDir, &buffer);
		}
		AFCDirectoryClose(hAFC, hAFCDir);
	} else {
		printf("%s: No such file or directory\n", pwd.c_str());
	}

}

void mkdir(afc_connection *hAFC, string dir) {
	AFCDirectoryCreate(hAFC,(char *)dir.c_str());
}

void rmdir(afc_connection *hAFC, string *dir) {
	if(dir->c_str()[0] != '/') // absolute path
		if (shellpwd.c_str()[shellpwd.length()-1] == '/')
			*dir = shellpwd+*dir;
		else
			*dir = shellpwd+"/"+*dir;

	AFCRemovePath(hAFC,(char *)dir->c_str());
}

bool dirExists(afc_connection *hAFC, char *path) {	
	afc_directory *hAFCDir;

	if (0 == AFCDirectoryOpen(hAFC, path, &hAFCDir)) {
		AFCDirectoryClose(hAFC, hAFCDir);
		return true;
	}
	return false;
}

void device_notification_callback(am_device_notification_callback_info *info) {
	iPhone=info->dev;
	if(info->msg==ADNCI_MSG_CONNECTED) {
		connected=connect();
#if defined(__APPLE__)
		const int retval = shell();
		exit(retval);
#endif
	} else if(info->msg==ADNCI_MSG_DISCONNECTED) {
		cout << endl << "*******************DISCONNECTED*******************" << endl;
		run=false;
	}
}

void dfu_connect_callback(am_recovery_device *rdev) {
	cout << "Detected a DFU connection: How did you do this?" << endl;
}

void dfu_disconnect_callback(am_recovery_device *rdev) {
	cout << "Detected a DFU disconnection: How did you do this?" << endl;
}

void recovery_progress_callback() {           //from nightwatch, what gets done here?
	fprintf(stderr, "Recovery progress callback...\n");
}

int sendCommandToDevice(am_recovery_device *rdev, void * cfs) {  //wrapper for private dll function
	int retval = 0;
#if defined (WIN32)
	asm("movl %3, %%esi\n\tpush %1\n\tcall *%0\n\tmovl %%eax, %2"
		:
	:"m"(wsendCommandToDevice),  "m"(cfs), "m"(retval), "m"(rdev)
		:); //"%esi"
#elif defined (__APPLE__)
	retval = wsendCommandToDevice(rdev, cfs);
#else
	cout << "sendCommandToDevice is not implmented on your platform." << endl;
#endif
	return retval;
}

void recovery_connect_callback(am_recovery_device *rdev) {
	string cline;
	cout << "Logging in restore.log: " << AMRestoreEnableFileLogging("restore.log") << endl;
	cout << "I NEED A WAY TO EXIT THIS MODE" << endl;
	cout << "To exit, you need to do a full restore" << endl;

	rconnected=true;
	while(rconnected) {
		cout << "recovery# ";
		getline(cin, cline);
		if(cline!="")
			cout << cline << ": " << sendCommandToDevice(rdev, (void *)CFStringCreateWithCString(NULL, cline.c_str(), cline.length()) ) << endl; 
	}

	/*//from nightwatch
	CFMutableDictionaryRef opts;
	opts = AMRestoreCreateDefaultOptions(kCFAllocatorDefault);
	CFDictionarySetValue(opts, CFSTR("RestoreBundlePath"), CFSTR("c:/phonedmg/"));
	describe255(opts);
	cout << "Perform restore: " << AMRestorePerformRecoveryModeRestore(rdev, opts, (void *)recovery_progress_callback, NULL) << endl; 
	*/
}

void recovery_disconnect_callback(am_recovery_device *rdev) {
	cout << endl << "DisconnectRecovery" << endl;
	rconnected=false;
}

void cd_apply(string *changeto)
{
	// this method assumes that changeto is
	// NOT an absolute path and that it ends with a '/'
	string::size_type length = changeto->find("/",0);
	string *token = new string( changeto->substr(0, length) );
	
	//check to see if it's '..'
	if( *token == ".." && shellpwd.length() > 1 )
	{
		shellpwd = shellpwd.substr(0, shellpwd.rfind("/", shellpwd.length()-2) + 1);
		
	} else if  (*token == ".")  { 
		// do nothing?
		
	} else {
		// add token and check to see if dir exists
		
		string *temp = new string(shellpwd + *token + "/");
		if   (dirExists(hAFC, (char *)temp->c_str()))
			shellpwd = temp->c_str();
		else
			cout << "Path " << *temp << " does not exist." << endl;
		delete temp;
	}
	
	delete token;
	
	//if there is another token in changeto
	//recurse on it
	if( changeto->length() > (length + 1) )
	{
		*changeto = changeto->substr(length + 1, changeto->length() - 1);
		cd_apply(changeto);
	}
}

int main(int argc, char *argv[]) 
{
	am_device_notification *notif;
	int ret;
	run = true;
	connected= false;

	if(argc==1)	{
		cout << "iPhoneInterface v" << VERSION << " built on " << __DATE__ << endl << endl;
	}

#if defined(WIN32)
	//get sendCommandToDevice function pointer from dll
	HMODULE hGetProcIDDLL;
	hGetProcIDDLL = GetModuleHandle("iTunesMobileDevice.dll");
	//int __usercall sendCommandToDevice<eax>(am_recovery_device *,void *);         //pass the recovery dev+cfstring of command
	//the address of sendCommandToDevice is 10009290
	//the address of AMRestorePerformRecoveryModeRestore is 10009F30
	if (!hGetProcIDDLL)
		cout << "Could not find dll in memory" << endl;
	else
		wsendCommandToDevice=cmdsend((void *)((char *)GetProcAddress(hGetProcIDDLL, "AMRestorePerformRecoveryModeRestore")-0x10009F30+0x10009290));
#elif defined(__APPLE__)
	void* handle = dlopen("/System/Library/PrivateFrameworks/MobileDevice.framework/MobileDevice", RTLD_LOCAL);
	if(handle == 0) {
		cout << "Error during dlopen: " << dlerror() << endl;
		exit(EXIT_FAILURE);
	}

	// Want _AMRUSBDeviceSendDeviceRequest, but it's a local symbol in MobileDevice.
	// The closest global is AMSGetErrorReasonForErrorCode. Look it up, and add the
	// known offset. This is very fragile and hopefully can be removed soon. That
	// said, it's also how the windows side of the house does it.
	//
	// nm MobileDevice
	// $3c3a2262 t __AMRUSBDeviceSendDeviceRequest
	// ...
	// $3c396180 T __AMSGetErrorReasonForErrorCode
	// 
	// Get the global signal, and then load from the offset
	// $3c3a2262 - $3c396180 = $c0e2

	void* knownSymbol = dlsym(handle, "_AMSGetErrorReasonForErrorCode");
	if(knownSymbol == 0) {
		cout << "Error looking up global symbolduring dlsym: " << dlerror() << endl;
		exit(EXIT_FAILURE);
	} else {
		wsendCommandToDevice = (cmdsend)((char*)knownSymbol + 0xC0E2);
	}
#endif

	ret = AMDeviceNotificationSubscribe(device_notification_callback, 0, 0, 0, &notif);
	if (ret != 0) {
		cout << "Problem registering main callback: " << ret << endl;
		return EXIT_FAILURE;
	}
	ret = AMRestoreRegisterForDeviceNotifications(dfu_connect_callback, recovery_connect_callback, dfu_disconnect_callback, recovery_disconnect_callback, 0, NULL);
	if (ret != 0) {
		cout << "Problem registering recovery callback: " << ret << endl;
		return EXIT_FAILURE;
	}


	
	cout << "Waiting for phone... " << flush;
#if defined(__APPLE__)
	CFRunLoopRun();
#else
	shell();
#endif
}

//sh functions - return EXIT_FAILURE to break and still clean up
int sh_help(string *arg)
{
	if(*arg == "NULL") {
		cout << "help           -   this, also help startservice and help readvalue" << endl;
		cout << "ls             -   list directories" << endl;
		cout << "cd             -   change directory" << endl;
		cout << "mkdir          -   make directory" << endl;
		cout << "rmdir          -   remove directory" << endl;
		cout << "deviceinfo     -   get device info" << endl;
		cout << "fileinfo       -   get file info" << endl;
		cout << "readvalue      -   read a value" << endl;
		//cout << "writevalue     -   write a value" << endl;
		cout << "activate       -   activate iPhone with plist" << endl;
		cout << "deactivate     -   deactivate iPhone" << endl;
		cout << "startservice   -   start service on iPhone" << endl;
		cout << "enterrecovery  -   Enter recovery Mode **WARNING: YOU'LL NEED TO RESTORE**" << endl;
		cout << "quit           -   exit shell" << endl;
	} else if (*arg == "startservice") {
		cout << "com.apple.afc                      (WILL CRASH PROGRAM)" << endl;
		cout << "com.apple.crashreportcopy" << endl;
		cout << "com.apple.mobile.debug_image_mount" << endl;
		cout << "com.apple.mobile.notification_proxy" << endl;
		cout << "com.apple.mobile.software_update   (WILL CRASH PHONE)" << endl;
		cout << "com.apple.mobile.system_profiler" << endl;
		cout << "com.apple.mobilebackup" << endl;
		cout << "com.apple.mobilesync" << endl;
		cout << "com.apple.syslog_relay" << endl;
		//broken services
		//  com.apple.purpletestr
		//  com.apple.screenshottr
	} else if (*arg == "readvalue") {
		cout << "NAME                         TYPE         WRITABLE" << endl;
		cout << "ActivationState              CFString     NO" << endl;
		cout << "DeviceClass                  CFString     NO" << endl;
		cout << "DeviceName                   CFString     YES" << endl;
		cout << "ActivationStateAcknowledged  bool         YES" << endl;
	}
	
	return EXIT_SUCCESS;
	
}

int sh_activate(string *arg)
{
	if(*arg=="NULL") {
		cout << "activate: please supply a plist filename" << endl;
	} else {
#if defined (__APPLE__) 
		// Create a stream from the file given and open it
		CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
			CFStringCreateWithCString(kCFAllocatorDefault, arg->c_str(), kCFStringEncodingASCII),
			kCFURLPOSIXPathStyle,
			false);
		CFReadStreamRef stream = CFReadStreamCreateWithFile(kCFAllocatorDefault, url);
        Boolean opened = CFReadStreamOpen(stream);
        if(opened == FALSE)
        {
            cout << "Error cannot open " << *arg << " for reading " << endl;
            exit(EXIT_FAILURE);
        }

		// Create a Dictionary based on the data in the stream
		CFStringRef errString;
		CFPropertyListFormat format;
		CFDictionaryRef activationDict = (CFDictionaryRef) CFPropertyListCreateFromStream(
			kCFAllocatorDefault,
			stream,
			0, /* read to the end of the stream */
			kCFPropertyListMutableContainersAndLeaves,
			&format,
			&errString);

		if(errString != NULL) {
			const char* ss =  CFStringGetCStringPtr(errString, kCFStringEncodingASCII);
			cout << "Error allocating property list: " << ss << endl;
			exit(EXIT_FAILURE);
		}

		// Query the dictionary for its activation record
		CFMutableDictionaryRef activationRec = (CFMutableDictionaryRef)CFDictionaryGetValue(
			activationDict, CFSTR("ActivationRecord"));

		if(activationRec == NULL) {
			cout << "Error accessing activation record" << endl;
			exit(EXIT_FAILURE);
		}

		// Call the Activation API and display results
		const int err = AMDeviceActivate(iPhone, activationRec);
		cout << "activate: " << err << " ";
		GhettoCFStringPrint(AMDeviceCopyValue(iPhone, 0, CFSTR("ActivationState")));
#else
		cout << "Activation currently does not support your platform." << endl;
#endif
	}
	
	return EXIT_SUCCESS;

}

int sh_deactivate(string *arg)
{
	cout << "deactivate: " << AMDeviceDeactivate(iPhone) << " ";
	__CFString * result = AMDeviceCopyValue(iPhone, 0, CFSTR("ActivationState"));
	if (result)
		GhettoCFStringPrint(result);
	else
		cout << "Error reading ActivationState from iPhone" << endl;
}

int sh_readvalue(string *arg)
{
	__CFString * result = AMDeviceCopyValue(iPhone, 0, CFStringCreateWithCString(NULL, arg->c_str(), arg->length()));
	if (result)
		GhettoCFStringPrint(result);
	else
		cout << "Error reading value '" << *arg << "' from iPhone" << endl;
	
	return EXIT_SUCCESS;
}

int sh_enterrecovery(string *arg)
{
	cout << "AMDeviceEnterRecovery: " << AMDeviceEnterRecovery(iPhone) << endl;
	return EXIT_SUCCESS;
}

int sh_startservice(string *arg)
{
	if(AMDeviceStartService(iPhone, CFStringCreateWithCString(NULL, arg->c_str(), kCFStringEncodingASCII) , &hAFC, NULL) == 0)
		cout << "AMDeviceStartService: Service started" << endl;
	else
		cout << "AMDeviceStartService: Service not found" << endl;
	return EXIT_SUCCESS;
}

int sh_deviceinfo(string *arg)
{
	afc_device_info *devinfo;
	cout << "deviceinfo: " << AFCDeviceInfoOpen(hAFC,&devinfo) << endl;               
	hexdump((void *)devinfo, 64);
	return EXIT_SUCCESS;
}

int sh_fileinfo(string *arg)
{
	
	int ret;
	
	if(*arg != "NULL") {
		if(arg->c_str()[0] != '/') // absolute path
			if (shellpwd.c_str()[shellpwd.length()-1] == '/')
				*arg = shellpwd+*arg;
			else
				*arg = shellpwd+"/"+*arg;
		afc_dictionary *resultDict;
		ret = AFCFileInfoOpen(hAFC, (char *)arg->c_str(), &resultDict);
		if(ret == 0) {
			hexdump(&resultDict, 64);
			cout << endl;
			hexdump(resultDict, 64);
		} else if(ret == 8)
			cout << "fileinfo: " << *arg << ": No such file or directory" << endl;
		else
			cout << "UNKNOWN ERROR (" << ret << ")" << endl;
	} else 
		cout << "fileinfo: please specify a file or directory" << endl;
	
	return EXIT_SUCCESS;
}

int putFile(afc_connection *hAFC, string *arg)
{

	int size,ret;
	afc_file_ref rAFC;
	char *data;

	ifstream file (arg->c_str(),ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		data = new char [size];
		file.seekg (0, ios::beg);
		file.read (data, size);
		file.close();

	}	
	if(*arg != "NULL") {
                if(arg->c_str()[0] != '/') // absolute path
                        if (shellpwd.c_str()[shellpwd.length()-1] == '/')
                                *arg = shellpwd+*arg;
                        else
                                *arg = shellpwd+"/"+*arg;

		ret = AFCFileRefOpen(hAFC, (char *)arg->c_str(), 3, 0, &rAFC);
		if (ret != 0) {
			cout << "Problem with AFCFileRefOpen: " << ret << endl;
			return EXIT_SUCCESS;
		}

		ret = AFCFileRefWrite(hAFC, rAFC, data, size);
		if (ret != 0) {
			cout << "Problem with AFCFileRefWrite: " << ret << endl;
			return EXIT_SUCCESS;
		} 

		ret = AFCFileRefClose(hAFC, rAFC);
		delete[] data;
	}

	return EXIT_SUCCESS;

}
	
		
int sh_cd(string *arg)
{
	//Absolute path
	if(arg->c_str()[0]=='/')
	
		// check for existence
		if (dirExists(hAFC, (char *)arg->c_str()))
			shellpwd=*arg; 
		else
			cout << *arg << ": No such file or directory" << endl;
	else {
		
		//check for trailing '/'
		if(arg->at(arg->length() - 1) != '/')
			*arg = *arg + "/";

		// apply
		cd_apply(arg);
	}
	
	return EXIT_SUCCESS;

}

int sh_ls(string *arg)
{
	if(*arg == "NULL")
		ls(hAFC, shellpwd);
	else
		ls(hAFC, *arg);
		
	return EXIT_SUCCESS;
}

int sh_mkdir(string *arg)
{
	if(*arg != "NULL")
		mkdir(hAFC, *arg);
	else
		cout << "mkdir: please specify a directory to create" << endl;
		
	return EXIT_SUCCESS;
}

int sh_rmdir(string *arg)
{
	if(*arg != "NULL")
		rmdir(hAFC, arg);
	else
		cout << "rmdir: please specify a directory to remove" << endl;
	
	return EXIT_SUCCESS;
}

int sh_putfile(string *arg)
{
	if(*arg != "NULL")
		putFile(hAFC, arg);
	else
		cout << "putfile: please specify a file to upload" << endl;
	
	return EXIT_SUCCESS;
}

int sh_quit(string *arg)
{
	return EXIT_FAILURE;
}

int sh_exit(string *arg)
{
	return EXIT_FAILURE;
}

int shell() {

	string cmd,cline;
	size_t strpoint;
	int ret;
	funcptr retfxn;
	
	// Populate function tree
	node *treetop = new node("help",&sh_help);
	insertnode(treetop, new node("cd",		&sh_cd			));
	insertnode(treetop, new node("ls",		&sh_ls			));
	insertnode(treetop, new node("mkdir",		&sh_mkdir		));
	insertnode(treetop, new node("rmdir",		&sh_rmdir		));
	insertnode(treetop, new node("activate",	&sh_activate		));
	insertnode(treetop, new node("deactivate",	&sh_deactivate		));
	insertnode(treetop, new node("readvalue",	&sh_readvalue		));
	insertnode(treetop, new node("enterrecovery",	&sh_enterrecovery	));
	insertnode(treetop, new node("startservice",	&sh_startservice	));
	insertnode(treetop, new node("deviceinfo",	&sh_deviceinfo		));
	insertnode(treetop, new node("fileinfo",	&sh_fileinfo		));
	insertnode(treetop, new node("putfile",		&sh_putfile		));
	insertnode(treetop, new node("quit",		&sh_quit		));
	insertnode(treetop, new node("exit",		&sh_exit		));
	
	while(run) {
		if(connected) {
			
			// initial display of iphone status on app run
			cout << "iPhone state: ";
			GhettoCFStringPrint(AMDeviceCopyValue(iPhone, 0, CFSTR("ActivationState")));
			ret = AMDeviceStartService(iPhone, CFSTR("com.apple.afc"), &hAFC, NULL);
			if (ret != 0) {
				cout << "Problem starting AFC: " << ret << endl;
				return EXIT_FAILURE;
			}
			ret = AFCConnectionOpen(hAFC, 0, &hAFC);
			if (ret != 0) {
				cout << "Problem with AFCConnectionOpen: " << ret << endl; 
				return EXIT_FAILURE;
			}
			cout << "type \"help\" for help" << endl;

			//AFC Started
			//Main loop-- Interactive Mode
			while(run) 
			{	
				cout << "iPhone:" << shellpwd << "# ";
				getline(cin, cline);
				strpoint=cline.find(" ", 0);
				cmd=cline.substr(0,strpoint);
					
				// parse the entered command
				if(strpoint!=string::npos && strpoint+1 < cline.length())
					cline=cline.substr(strpoint+1);
				else
					cline="NULL";
				
				//check for command entered
				retfxn = getFxnPointer(&cmd, treetop);
				
				if(retfxn)
				{
					// if its here, execute and quit if function
					// doesnt return EXIT_SUCCESS
					if( (*retfxn)(&cline) != EXIT_SUCCESS )
						run = false;
					
				} else {	
					
					// in the case that its not found, try to do
					// simple completion based on the string entered
					vector<node *> *rs = new vector<node *>();
					searchIncompleteString(&cmd, rs, treetop);
					
					if( rs->size() > 0 )
					{
						cout << "Possible " << rs->size() << " commands:" << endl;

						for(int i = 0; i < rs->size(); i++)
						{
							cout << *((*rs)[i]->val) << endl;
						}
					} else {
						cout << "Command '" << cmd << "' not found." << endl;
					}
					
					delete rs;
				} 
			}
		}
	}
	
	delete treetop;
	return EXIT_SUCCESS;
}
