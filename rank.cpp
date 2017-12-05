#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "rank.h"

using namespace std;

/* Return a hash for the string s in the range 0..table_size-1 */
int hash(string s, int table_size)
{
  unsigned int i, h = 0;
  for (i=0; i<s.length(); i++)
    h = (h * 2917 + (unsigned int)s[i]) % table_size;
  return h;
}

/* Allocate a table of pointers to nodes, all initialized to NULL */
Node **allocate_table(int size)
{
  Node **table = new Node *[size];
  for (int i=0; i<size; i++)
    table[i] = NULL;
  return table;
}

Rank::Rank()
{
  size = 4; // initial size of table    
  table = allocate_table(size);
  num_elems = 0; 
}

Rank::~Rank()
{
  for (int i=0; i<size; i++) {
    while (table[i] != NULL) {
      Node *temp = table[i];
      table[i] = table[i]->next;
      delete temp;
    }
  }
  delete[] table;
}

/* Return true if key is in the set */
int Rank::find(string key)
{
  int h = hash(key, size);
  Node *n = table[h];
  while (n != NULL) {
    if (n->key == key) 
      return n->id_index;
    n = n->next;
  }
  return -2;
}

/* Inserts a new key.  It is an error if key is already in the set. */
bool Rank::insert(string key, int id)
{
  if (find(key) != -2) return false;
  num_elems++;

  if (num_elems == size) {
    // Expand table -- allocate new table of twice the size
      size = size*2;
    // re-insert all keys into new table, and de-allocate old table.
        Node **oldtable;
        oldtable = table;
        table = allocate_table(size);
        num_elems = 0;
        int i;
        for (i=0; i<size/2; i++) {
        
          // pointer to the old table 
          Node *n = oldtable[i];
          
          // make sure the old table's nodes are null
          while (n != NULL) {
            insert(n->key, n->id_index); // give insert function the key 
            n = n->next; // increment though the old table
          }            
        }
        
          // deletes the contents in the old table one node at at time
         for (i=0; i<size/2; i++) {
            while (oldtable[i] != NULL) {
              Node *temp = oldtable[i];
              oldtable[i] = oldtable[i]->next;
              delete temp;
            }
         }
         
        // delete old smaller table 
        delete[] oldtable;
  }

  // Insert new element
  int sh;
  sh = hash(key, size); // calcuates hash of key
  
  if (table[sh] == NULL) {
    Node *n = new Node(key, id, NULL);
    table[sh] = n;
  }
   
  else {
    Node *p = table[sh];
    while (p->next != NULL) {
      p = p->next;
    }
    
    p->next = new Node(key, id, NULL);
  } 
    return true;
}

int Rank :: get_num_elems() {

  return num_elems;
}
