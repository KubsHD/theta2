#include "uuid.h"


#if WIN

#include <Windows.h>

String ID::get_uuid()
{
	UUID tmp;

	RPC_CSTR str = NULL;

	UuidCreate(&tmp);
	UuidToStringA(&tmp, &str);

	String s = (char*)str;

	return s;
}

#elif APPLE


#include <CoreFoundation/CoreFoundation.h>

String ID::get_uuid()
{
	auto uuid = CFUUIDCreate(NULL);
	auto str = CFUUIDCreateString(NULL, uuid);

	auto mut = CFStringCreateMutableCopy(NULL, 0, str);

	CFStringLowercase(mut, CFLocaleCopyCurrent());

	return String(CFStringGetCStringPtr(mut, kCFStringEncodingUTF8));
}

#endif
