#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <bzlib.h>
#include <stdexcept>

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
	std::ofstream *out = static_cast<std::ofstream*>(stream);
	out->write(static_cast<char*>(ptr), size * nmemb);
	return size * nmemb;
}

int download_data(){	
	CURL *curl = curl_easy_init();
	if(!curl) return 1;

	std::time_t now = std::time(nullptr);
	std::tm* local_time = std::localtime(&now);

	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y%m%d", local_time);
	std::string date(buffer);

	std::string file_name = date + ".as-rel2.txt.bz2";
	std::string download_path = "https://publicdata.caida.org/datasets/as-relationships/serial-2/" + file_name;

	std::ofstream outfile(date + ".as-rel2.txt.bz2", std::ios::binary);
	curl_easy_setopt(curl, CURLOPT_URL, download_path.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outfile);
	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	if(res!= CURLE_OK){
		std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
	}
	return (res == CURLE_OK ? 0 : 1);
}

int parse_data(){

}

int main(){
}
