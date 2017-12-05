#ifndef RANK_H /* Prevent multiple inclusion... */
#define RANK_H

#include <string>
using namespace std;

struct Node {
  string key;
  Node *next;
  int id_index;
  Node(string k, int id, Node *n) { key = k; id_index = id; next = n; }
  Node() { key = ""; id_index = 0; next = NULL; }
};

struct Page {

  string url;
  double weight;
  double new_weight;
  double p_links;
  Node *word;
  Node *http;
  Page(string u) { url = u; word = NULL; http = NULL; }
};

struct link {

};

class Rank {

 private: 
  Node **table;  // array of pointers to linked lists
  int size;      // size of table, as currently allocated
  int num_elems; // number of elements stored in the table

 public:
  Rank();
  ~Rank();
  int find(string key);
  bool insert(string key, int id);
  int get_num_elems(); 
};

#endif
