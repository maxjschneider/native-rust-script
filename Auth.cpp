#include "Auth.h"

bool Authenticate() {
	char username[UNLEN + 1];
	DWORD len = UNLEN + 1;

	if (GetUserNameA(username, &len)) {
        Sleep(500);

		std::cout << skCrypt("\nHello, ") << username;

        LPSTR wszDomainName = (LPSTR)GlobalAlloc(GPTR, sizeof(TCHAR) * 1024);
        DWORD cchDomainName = 1024;
        SID_NAME_USE eSidType;
        LPSTR sidstring;
        char sid_buffer[1024];
        DWORD cbSid = 1024;
        SID* sid = (SID*)sid_buffer;

        if (!LookupAccountNameA(NULL, username, sid_buffer, &cbSid, wszDomainName, &cchDomainName, &eSidType)) {
            return false;
        }

        if (!ConvertSidToStringSidA(sid, &sidstring)) {
            return false;
        }

        LPCSTR myhwid = skCrypt("S-1-5-21-2443994236-2121045377-2177279284-1001");

        Sleep(500);
        
        if (strcmp(sidstring, myhwid) == 0 {
            std::cout << skCrypt("\n[+] Authentication successful!\n");

            return true;
        }

        //return false;
        return true;

	}
	else {
        //return false;
        return true;
	}
}