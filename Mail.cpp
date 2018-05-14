#include "stdafx.h"
#include "Mail.h"
const string SCRIPT_NAME = "EmailSend2.ps1";

string Mail::replaceString(string sourse, const string & data, const string & with)
{
	if (data.empty())
		return sourse;
	size_t size = 0;
	while ((size = sourse.find(data, size)) != string::npos)
		sourse.replace(size, data.length(), with), size += with.length();
	return sourse;
}

bool Mail::checkFileExists(const string &name)
{
	ifstream file(name);
	return (bool)file;
}
bool Mail::createScript()
{
	InputOutup temp;
	ofstream script(temp.getPath(true) + SCRIPT_NAME);
	if (!script)
		return false; 
	script << this->PowerShellScript;
	if (!script)
		return false;
	script.close();
	return true;
}
int Mail::sendMail(const string &subject, const string &body, const string &attachments)
{
	bool ok;    // used for testing within this code
	InputOutup temp;
	ok = temp.fillDir(temp.getPath(true));
	if (!ok)
		return -1;  // we weren't able to create a directory
	string scr_path = temp.getPath(true) + string(SCRIPT_NAME);
	if (!checkFileExists(scr_path))
		ok = createScript();
	if (!ok)
		return -2;  // the creation was unsuccessful
					// string for invoking the PowerShell script
	std::string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" -Subj \""
		+ replaceString(subject, "\"", "\\\"") + "\" -Body \""
		+ replaceString(body, "\"", "\\\"") + "\" -Att \"" + attachments + "\"";
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;     // prevent process from closing
	ShExecInfo.hwnd = NULL;         //no window when running
	ShExecInfo.lpVerb = "open";   
	ShExecInfo.lpFile = "powershell";
	ShExecInfo.lpParameters = param.c_str();
	ShExecInfo.lpDirectory = NULL;  // a working directory
	ShExecInfo.nShow = SW_HIDE;     // so it remains hidden (does not "pop" up)
	ShExecInfo.hInstApp = NULL;     // basically "handles" the instance
	ok = (bool)ShellExecuteEx(&ShExecInfo);
	if (!ok)
		return -3;
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE); // wait for 7sec
	DWORD exit_code = 100;
	GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);    // check how the powershell worked out
	//CurrentTimeDate tm;
	this->_timer.setFunction([&]()  // lamda or "anonymous" function
	{

		WaitForSingleObject(ShExecInfo.hProcess, 60000);    // wait 1 min
		GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);    // get exit code
		if ((int)exit_code == STILL_ACTIVE)    // exit code = 259 if it's still active
			TerminateProcess(ShExecInfo.hProcess, 100);
			this->_time.writeAppLog("<From SendMain> Return code: " + this->_time.toString((int)exit_code));
	});
	this->_timer.repeatCount(1L);    // so, execute on;y once
	this->_timer.setInterval(10L);
	this->_timer.Start(true);
	return (int)exit_code;
}
int Mail::sendMail(const string &subject, const string &body, const vector<string> &att)

{

	string attachments = "";

	if (att.size() == 1U)    // U = unsigned integer, so if we have only one attachment
		attachments = att.at(0);
	else    // else, we construct a  string of attachments
	{
		for (const auto &v : att)
			attachments += v + "::";
		attachments = attachments.substr(0, attachments.length() - 2);
	}
	return this->sendMail(subject, body, attachments);

}
Mail::Mail()
{

}


Mail::~Mail()
{
}
