#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Direction.hpp"
#include "Notebook.hpp"
const int WIDTH=100;
const int MINCHAR=32;
const int MAXCHAR=126;
using namespace std;

namespace ariel
{
    
    Notebook::Notebook() 
    {
    }

    bool check_text(string text)  // check a given text does not have invalid chars in it.
    {
        for(int i=0; i<text.length(); i++)
        {
            if(text[(size_t)i] < MINCHAR || text[(size_t)i] > MAXCHAR || text[(size_t)i] == '~')
            {
                return false;
            }
        }
        return true;
    }

    void Notebook::write(int page, int row, int col, Direction direction, string text)
    {
        if(!check_text(text)) // check the text is valid
        {
            throw invalid_argument("invalid text");
        }
        if(page<0 || row<0 || col<0 || col >= WIDTH) //check if the arguments are valids (none negative and in the page boundries)
        {
            throw invalid_argument("invalid arguments");
        }
        if(direction == Direction::Horizontal && text.size() >= WIDTH - col) // check if the text won't fit in the Horizontal line
        {
            throw invalid_argument("trying to write to columns over 100");
        }

        if(!pages.contains(page))  // if the page does not exist yet we create new one
        {
            vector<vector<char>> new_page((size_t)row+1, vector<char>(WIDTH, '_'));
            pages[page] = new_page;
        }
        while(row >= pages[page].size()) // if the row is too high, we create new rows in the page until reaching that row.
        {
            pages[page].push_back(vector<char>(WIDTH, '_'));
        }
        
        int i = 0;
        if(direction == Direction::Vertical)
        {
            for(i=0; i<text.size(); i++)  // go over the text chars
            {
                if(i + row >= pages[page].size())  // if we dont have all the lines in the page, we testing for written or deleted chars.
                {
                    break;
                }
                if(pages[page][(size_t)row + (size_t)i][(size_t)col] != '_')  // if the line we check already had somthing written on it or deleted from.
                {
                    throw invalid_argument("trying to write on an occupied space");
                }
            }

            while(pages[page].size() < (size_t)row + text.size())  // add to the page lines in order to write the word.
            {
                pages[page].push_back(vector<char>(WIDTH, '_'));
            }

            for(i=0; i<text.size(); i++)  // after passing the tests, we add the text to the correct line, char after char.
            {
                pages[page][(size_t)row+(size_t)i][(size_t)col] = text[(size_t)i];
            }
        }
        else
        {
            for(i=0; i<text.size(); i++) // go over the text chars
            {
                // std::cout << "\npage: " << page << ", row: " << row << " , cul: " << col + i << " char in place: " << pages[page][(size_t)row][(size_t)col+(size_t)i] << "\n\n ";
                if(pages[page][(size_t)row][(size_t)col+(size_t)i] != '_') // if the collum we check already had somthing written on it or deleted from.
                {
                     throw invalid_argument("trying to write on an occupied space");
                }
            }

            for(i=0; i<text.size(); i++)  // after passing the test, we add the text to the correct collum, char after char.
            {
                pages[page][(size_t)row][(size_t)col+(size_t)i] = text[(size_t)i];
            }
            
        }
    }

    void Notebook::erase(int page, int row, int col, Direction direction, int amountToErase) 
    {
        if(page<0 || row<0 || col<0 || col >= WIDTH || amountToErase<0) //check if the arguments are valids (none negative and in the page boundries)
        {
            throw invalid_argument("invalid arguments");
        }
        if(direction == Direction::Horizontal && WIDTH < col + amountToErase)  // checking in horizontal state the we dont go over the limit of 100 chars.
        {
            throw invalid_argument("line only has 100 chars");
        }
        if(amountToErase == 0)
        {
            return;
        }

        if(!pages.contains(page))  // if the page does not exist yet we create new one
        {
            vector<vector<char>> new_page((size_t)row+1, vector<char>(WIDTH, '_'));
            pages[page] = new_page;
        }
        while(row >= pages[page].size()) // if the row is too high, we create new rows in the page until reaching that row.
        {
            pages[page].push_back(vector<char>(WIDTH, '_'));
        }

        

        if(direction == Direction::Vertical)
        {
            while(pages[page].size() <= row + amountToErase)  // add to the page new lines.
            {
                pages[page].push_back(vector<char>(WIDTH, '_'));
            }

            for(int i=0; i<amountToErase; i++)  // after checking, delete the chars and replace with '~'
            {
                pages[page][(size_t)row+(size_t)i][(size_t)col] = '~';
            }
        }
        else
        {

            for(int i=0; i<amountToErase; i++)  // after checking, delete the chars and replace with '~'
            {
                pages[page][(size_t)row][(size_t)col+(size_t)i] = '~';
            }
        }
    }

    string Notebook::read(int page,int row,int col, Direction direction,int amountToRead)
    {
        if(page<0 || row<0 || col<0 || WIDTH <= col || amountToRead<0) //check if the arguments are valids (none negative and in the page boundries)
        {
            throw invalid_argument("invalid arguments");
        }
        if(direction == Direction::Horizontal && col + amountToRead > WIDTH)  // if horizontal, check we wont go over 100 chars
        {
            throw invalid_argument("trying to read columns over 100");
        }
        if(!pages.contains(page))  // if the page does not exist yet we create new one
        {
            vector<vector<char>> new_page((size_t)row+1, vector<char>(WIDTH, '_'));
            pages[page] = new_page;
        }
        
        if(amountToRead == 0) // if amount to read is 0 then retuen empty string
        {
            return "";
        }
        while(row >= pages[page].size()) // if the row is too high, we create new rows in the page until reaching that row.
        {
            pages[page].push_back(vector<char>(WIDTH, '_'));
        }
        string ans(1, pages[page][(size_t)row][(size_t)col]);
        if(direction == Direction::Vertical)
        {
            while(pages[page].size() <= row + amountToRead)  // add to the page new lines.
            {
                pages[page].push_back(vector<char>(WIDTH, '_'));
            }
            for(int i=1; i<amountToRead; i++)  // insert correct chars to the answer string
            {
                ans += string(1, pages[page][(size_t)row+(size_t)i][(size_t)col]);
            }
        }
        else
        {
            for(int i=1; i<amountToRead; i++)  // insert correct chars to the answer string
            {
                ans += string(1, pages[page][(size_t)row][(size_t)col+(size_t)i]);
            }
        }
        return ans;
    }

    void Notebook::show(int page) 
    {
        if(page<0)
        {
            throw invalid_argument("page number need to be positive");
        }
        if(!pages.contains(page))  // if the page does not exist yet we create new one
        {
            vector<vector<char>> new_page(1, vector<char>(WIDTH, '_'));
            pages[page] = new_page;
        }

        for(int i=0; i < pages.at(page).size(); i++)
        {
            for(int j=0; j<WIDTH; j++)
            {
                cout << pages[page][(size_t)i][(size_t)j];
            }
            cout << '\n';
        }
    }
}