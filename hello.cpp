#include <curl/curl.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
using namespace std;

int main()
{
  string song_name;                                                     // Name of song being searched
  string file_name = "html.txt";                                        // File that contains html code
  string search_url = "https://www.youtube.com/results?search_query=";  // Youtube search url
  string youtube_url = "https://www.youtube.com/";                      // Youtube url
  string video_url;                                                     // Final searched video url
  string watch_url = "/watch?v=";                                       // Youtube standard watch url

  /* 
  * Get input and change it to match Youtube standard
  */
  getline(cin, song_name);
  replace(song_name.begin(), song_name.end(), ' ', '+');
  search_url.append(song_name);


  /* 
  * Use libcurl to get html from search_url
  */
  CURL *curl = curl_easy_init();
  CURLcode res;
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, search_url.c_str());
    FILE* file = fopen(file_name.c_str(), "w");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }

  /* 
  * Reading file into a buffer
  * Credit to https://stackoverflow.com/questions/2912520/read-file-contents-into-a-string-in-c
  * Martin York
  */
  ifstream file(file_name);
  /*
  * Get the size of the file
  */
  file.seekg(0,ios::end);
  std::streampos length = file.tellg();
  file.seekg(0,ios::beg);

  /*
  * Use a vector as the buffer.
  * It is exception safe and will be tidied up correctly.
  * This constructor creates a buffer of the correct length.
  * Because char is a POD data type it is not initialized.
  *
  * Then read the whole file into the buffer.
  */
  vector<char> buffer(length);
  file.read(&buffer[0],length);

  /*
  * Convert buffer to string and find index of first watch_url (video)
  */
  string s(buffer.begin(), buffer.end());
  size_t found = s.find(watch_url);

  /*
  * Get Youtube video url code
  */
  for (int i = found; s.at(i) != '\"'; i++)
    video_url += s.at(i);

  system(("open " + youtube_url + video_url).c_str()); // Open video

  return 0;
}