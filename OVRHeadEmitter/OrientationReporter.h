#pragma once

#include <curl/curl.h>

class OrientationReporter
{
public:
	OrientationReporter();
	~OrientationReporter();

	bool Connect(const char* url);
	void Report(float pitch, float yaw);
	void Clear();
private:
	CURL *curl;
	CURLcode res;
};