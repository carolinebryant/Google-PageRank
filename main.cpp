/*
Caroline Bryant
CPSC 212 - google pagerank algorithm
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "rank.h"

using namespace std;

int main (void) {

  int num_pages = 0;
  ifstream fin;
  string s;

  fin.open("webpages.txt");
  while (fin >> s) {
  
    if (s == "NEWPAGE")
      num_pages++;
  }
  fin.close();
  
  ifstream stuff;
  stuff.open("webpages.txt");
  int count = -1;
  
  // make a hash table to count words
  Rank *elems = new Rank();
  
  // array of pointers to an array that holds linked lists of pages
  Page **table_p = new Page*[num_pages];
  // initalize table to null 
  for (int i = 0; i < num_pages; i++) {
  
    table_p[i] = NULL;
  }

  while (stuff >> s) {
  
    if (s == "NEWPAGE") {
      stuff >> s;
      count++;
      // fill table with pages initalize links to various pages to zero 
      table_p[count] = new Page(s);
      table_p[count]->p_links = 0;
    }
    
    else if (s.compare(0, 7, "http://")) {
    
      // setting up linked lists with words 
      table_p[count]->word = new Node(s, -1, table_p[count]->word);
      elems->insert(s, -1);
    }
    
    else {
      // setting up hyperlink to the page and increment counter for number of links to pages
      table_p[count]->http = new Node(s, -1, table_p[count]->http);
      table_p[count]->p_links++;
    }
  }
  stuff.close();

  // inserting pages into the hash table
  Rank thing;
  for (int i = 0; i < num_pages; i++) {
    thing.insert(table_p[i]->url, i);
  }

  // pagerank - give each page initial weight = 1 / total number of pages
  for (int i = 0; i < num_pages; i++) {
     table_p[i]->weight = 1 / num_pages;
  }

  // repeat 50 times
  for (int j = 0; j < 50; j++) {

    // for each page i, set new_weight[i] = 0.1 / total number of pages  
    for (int i = 0; i < num_pages; i++) {
        table_p[i]->new_weight = 0.1/num_pages;   
    }
    
    for (int i = 0; i < num_pages; i++) {
        // for each page j (of t total) to which i links , increase new_weight[j] by 0.9 * weight[i] / t
        for(Node *n = table_p[i]->http; n != NULL; n = n->next) {
            // stores hash from first iteration, so it doesnt hash again
            if (n->id_index == -1) 
              n->id_index = thing.find(n->key);
            // if the current pointer finds the index returned by find function, calculate the new weight
            if (n->id_index != -2)
              table_p[n->id_index]->new_weight += 0.9 * (table_p[i]->weight / table_p[i]->p_links);        
        }
    }
     
     // for each page i , set weight[i] = new_weight[i] 
    for (int i = 0; i < num_pages; i++) {
      for (Node *n = table_p[i]->http; n != NULL; n = n->next) {
        table_p[i]->weight = table_p[i]->new_weight; 
      }
    }   
  }  
 
  // inserting words into hash table
  int rev_num = elems->get_num_elems();
  Node **rev_index = new Node*[rev_num];
  
  // initalizing reverse index array pointers to null
  for (int i = 0; i < rev_num; i++) {
    rev_index[i] = NULL;
  }
  
  delete elems;
  elems = new Rank();
  int rev_count = 0;
  
   // filling reverse index array with words
   for (int i = 0; i < num_pages; i++) {
      for (Node *n = table_p[i]->word; n != NULL; n = n->next) {
        bool inserted = elems->insert(n->key, rev_count);
        if (inserted) {
          rev_index[rev_count] = new Node(n->key, i, rev_index[rev_count]);
          rev_count++;
        }
        else {
          int index = elems->find(n->key);
          rev_index[index] = new Node(n->key, i, rev_index[index]);
        }
      }
   }   
  
    int run = 0;
    bool firstrun = true;
    string key;
    // takes in user input until they specify they want to quit
    while (run == 0) {
      // prompt user
      if (firstrun) {
        cout << "please enter a word: " ;
        if (!(cin >> key)) continue;
      }
      firstrun = false;
  
      for (Node *n = rev_index[elems->find(key)]; n != NULL; n = n->next) {
       Page *p = table_p[n->id_index];
       cout << (int)(p->weight * 10000000) << " " << p->url << endl;
      }
    
      // if user enters 1 it quits the program, any other number and it will promt user to enter another word
      cout << "press 1 if you'd like to quit, or enter another word: " << endl;
      cin >> key;
      if (key == "1") run = 1;
      
    }
  
  return 0;
}

