#include "stdafx.h"
#include <iostream>
#include "OrientationReporter.h"

using namespace std;

OrientationReporter::OrientationReporter()
{
}

OrientationReporter::~OrientationReporter()
{
	Clear();
}

bool OrientationReporter::Connect(const char* url)
{
	curl = curl_easy_init();
	if (!curl) {
		return false;
	}

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-type: application/json");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	return true;
}

void OrientationReporter::Report(float pitch, float yaw)
{
	char j[1024];
	sprintf(j, "{ \"pitch\": \"%.2f\", \"yaw\": \"%.2f\" }", pitch, yaw);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, j);
	res = curl_easy_perform(curl);
}

void OrientationReporter::Clear()
{
	if (curl) {
		curl_easy_cleanup(curl);
	}
}