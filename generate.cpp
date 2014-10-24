/*
  @ Andrew Li (FakeKane)
  Oct, 2014
  Generates theses for short papers given an e-text.
*/

#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>
#include <map>
typedef std::map<std::string, std::vector<std::string> > vec_dict;
typedef std::map<std::string, std::string> str_dict;
/*
  NOTES/TO-DO:
  Make character choice frequency = number of times it appears in text.
*/

/* FUNCTION DECLARATIONS */

// Returns a map that contains all strings located in "shells.txt"
vec_dict create_map_from_file();

// Pick one of each of the sections
str_dict choose_strs(
    vec_dict dict);

// Select sentence structure
std::string choose_struct();

// Form sentence from structure and selected words
std::string form_sentence(std::string s, str_dict dict);

//spliting algorithms (Evan Teran, StackOverflow http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c)
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

int main()
{
    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        vec_dict dict = create_map_from_file();
        str_dict rand_strs = choose_strs(dict);
        std::string structure = choose_struct();
        std::string sentence = form_sentence(structure, rand_strs);
        std::transform(sentence.begin(), sentence.end(), sentence.begin(), ::tolower);
        std::cout << sentence << '\n';
    }
}

vec_dict create_map_from_file()
{
    /* for theses words */
    std::ifstream f_dict("wordsdb.txt");

    vec_dict dict;
    std::string buffer;
    std::string cur_key;
    while (std::getline(f_dict, buffer))
    {
        if (f_dict.eof())
            break;
        if (buffer[0] == '@')
        {
            cur_key = buffer.substr(1);
        }
        else if (buffer != "")
        {
            dict[cur_key].push_back(buffer);
        }
    }
    return dict;
}

str_dict choose_strs(
    vec_dict dict)
{
    str_dict out;
    typedef vec_dict::iterator vec_it;
    for (vec_it iterator = dict.begin(); iterator != dict.end(); iterator++)
    {
        int rand_num = rand() % (*iterator).second.size();
        //TODO: CHOOSE BASED ON PROBABILITY OF APPEARING OF APPEARING IN TEXT
        std::string key = (*iterator).first;
        std::string val = (*iterator).second[rand_num];
        out.insert(std::pair<std::string, std::string> (key, val));
    }
    return out;
}

std::string choose_struct()
{
    /* for theses templates */
    std::ifstream f_struct("structures.txt");
    std::vector<std::string> structures;
    std::string buffer;
    while (std::getline(f_struct, buffer))
    {
        if (f_struct.eof())
            break;
        structures.push_back(buffer);
    }
    int rand_num = rand() % structures.size();
    return structures[rand_num];
}

std::string form_sentence(std::string s, str_dict dict)
{
    std::vector<std::string> words = split(s, ' ');
    std::string sentence = "";
    for (int i = 0; i < words.size(); i++)
    {
        int key_index = 0;
        if (words[i][0] == '[')
        {
            std::size_t found = words[i].find("]");
            std::string key = words[i].substr(1, found - 1);
            if (key.substr(0, 3) == "CHA")
            {
                std::vector<std::string> character_split = split(dict[split(key, ':')[0]], ':');
                if (key.substr(key.length() - 1, 1) == "1")
                {
                    sentence += character_split[0];
                }
                else if (key.substr(key.length() - 1, 1) == "2")
                {
                    sentence += character_split[1];
                }
            }
            else
                sentence += dict[key];
            key_index = found + 1;
        }
        sentence += words[i].substr(key_index) + " ";
    }
    return sentence;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
