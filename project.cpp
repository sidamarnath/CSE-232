#include <iostream>
using std::cin; using std::endl; using std::cout;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include<fstream>
using std::ifstream;
#include <algorithm>
using std::pair;
#include<sstream>
using std::istringstream;
using std::ostringstream;
using std::sort;
using std::to_string;
#include <iomanip>
using std::fixed;
using std::setprecision;

//defining struct product which stores item name, price(in cents), and quantity
struct product {
    string item_name;
    double price_cents;
    int quantity;
};
// defined struct store
// stores store name, location, and inventory
struct store {
    string store_name;
    string location;
    vector<product> inventory;
    //member function returns true if items exist in inventory
    bool findItem(string item) {
        for(auto element : inventory) {
            if(element.item_name == item) {
                return true;
            }
        }
        return false;
    }
    //member function returns price for item in inventory
    int getPrice(string item) {
        for(auto element : inventory)  {
            if(element.item_name == item) {
                return element.price_cents;
            }
        }
        return -1;
    }
   //member function returns struct product for item in inventory
    product getItem(string item) {
        product dummy;
        for(auto element : inventory) {
            if(element.item_name == item) {
                return element;
            }
        }
        return dummy;
    }
};

//helper function splits strings seperated by comma delimiter 
//adds to vector 
void splitLine(const string& input_str, vector<string>& v_words, 
                char delim = ',') {
  string::size_type start = 0;
  v_words.clear();
  auto pos = input_str.find(delim, start);
  while(pos != string::npos) {
    v_words.push_back(input_str.substr(start, pos - start));
    start = pos + 1;
    pos = input_str.find(delim, start);
  }
  if(start != input_str.size()) {
    v_words.push_back(input_str.substr(start));
  }
}

//helper function that prints the store related header information
void printStoreSummary(const vector<store> &store_list) {
    cout << "Store Related Information (ordered by in-file order): " << endl;
    cout << "There are " << store_list.size() << " store(s)." << endl;
    for(size_t i = 0; i < store_list.size();i++) {
        cout << store_list[i].store_name << " has " <<
                store_list[i].inventory.size() << " distinct items." << endl;
        
    }
    cout << endl;
}

//helper function that prints the item related detail information
void printItemSummary(const map<string,int> &all_items) {
    cout << "Item Related Information (ordered alphabetically): " << endl;
    cout << "There are " << all_items.size()
        << " distinct item(s) available for purchase." << endl;
    for (auto iter = all_items.begin(); iter != all_items.end(); iter++) {
        string name = iter->first;
        int qty = iter->second;
        cout << "There are " << qty << " " << name << "(s)." << endl;
    }

}

//helper function that prints shopping instructions output
//takes in shopping list that user inputs and map of items and stores
void printShoppingInstructions(const vector<string> &shopping_list, 
                                map<string,vector<store>> &shopping_cart) {
    double grand_total_price = 0;
    double total_item_price = 0;
    cout << endl;
    cout << "Shopping: ";

    for(auto list_item : shopping_list) {
        std::size_t pos = list_item.find(" ");
        int order_qty = stoi(list_item.substr(0,static_cast<int>(pos)));
        string name = list_item.substr(pos + 1);
        
        map<string,vector<store>>::iterator it = shopping_cart.find(name);
        vector<store> v_s = it->second;
        
        cout << endl;
        cout << "Trying to order " << order_qty << " " << it->first << "(s)."
                << endl;
        cout << v_s.size() << " store(s) sell " << it->first << '.' << endl;
        vector<string> output_str;
        for(auto element : v_s) {
            product p = element.getItem(name);
            int store_qty = p.quantity;
            if(store_qty < order_qty) {
                string s1 = "Order " + to_string(store_qty) + " from " + 
                            element.store_name + " in " + element.location;
                output_str.push_back(s1);
                total_item_price += store_qty * p.price_cents;
                order_qty = order_qty - store_qty;
            }
            else if(store_qty >= order_qty) {
                string s2 = "Order " + to_string(order_qty) + " from " + 
                            element.store_name + " in " + element.location;
                output_str.push_back(s2);
                total_item_price += order_qty * p.price_cents;
                break;
            }
            else if(order_qty <= 0) {
                break;
            }
        }
        cout << "Total price: $" << fixed << setprecision(2) 
                << total_item_price / 100 << endl;
        grand_total_price += total_item_price;
        for(auto element : output_str) {
             cout << element << endl;
        }
        total_item_price = 0;
    }
    cout << endl;
    cout << "Be sure to bring $" << fixed << setprecision(2) << 
        grand_total_price / 100 << " when you leave for the stores." << endl;
}


int main() {
    vector<store> store_list;
    store temp_store;
    product temp_item;
    vector<string> temp;
    map<string,int> all_items;

    string line;
    bool new_store = true;
    getline(cin, line);
    std::size_t pos = line.find(" ");
    int num_of_stores = stoi(line.substr(0,static_cast<int>(pos)));
    int store_count = 0;
   
    
    vector<string> shopping_list;
    vector<store> stores_with_item;
    map<string,vector<store>> shopping_cart;

    //process the store informatin and its inventory 
    // store it in a map of stores and items
    while(store_count != num_of_stores){
        getline(cin,line);
        if(new_store == true) {
            temp_store.store_name = line;
            getline(cin,line);
            temp_store.location = line;
            new_store = false;
        }
        else {
            if(line == "") {
                new_store = true;
                store_list.push_back(temp_store);
                temp_store.inventory.clear();
                store_count++;
            }
            else {
                splitLine(line, temp);
                temp_item.item_name = temp[0];
                temp_item.price_cents = stod(temp[2].substr(1, temp[2].size()))*100;
                temp_item.quantity = stoi(temp[1]);
                temp_store.inventory.push_back(temp_item);
   
                map<string,int>::iterator it = all_items.find(temp_item.item_name);
                if(it != all_items.end()) {
                    it -> second += temp_item.quantity;
                }
                else {
                    all_items.insert(pair<string,int>(temp_item.item_name,
                                                        temp_item.quantity));
                }
            }
        }

    }
    getline(cin,line);
    
    // Now get shopping list
    while(getline(cin,line)) {
        std::size_t pos = line.find(" ");

        string input_item = line.substr(pos + 1);
           
        shopping_list.push_back(line);  
        for(auto element : store_list) {
            if(element.findItem(input_item)) {
                stores_with_item.push_back(element);
                   
            }
        }
        //makes sure that the stores are ordered by price of items
        sort(stores_with_item.begin(), stores_with_item.end(), 
                [input_item](store& s1, store& s2){
            int p1 = s1.getPrice(input_item);
            int p2 = s2.getPrice(input_item);
              return p1 < p2;
        });
        shopping_cart.insert(pair<string, vector<store>>(input_item, 
                            stores_with_item));
        stores_with_item.clear();
    }

    printStoreSummary(store_list);
    printItemSummary(all_items);

    printShoppingInstructions(shopping_list, shopping_cart);
    return 0;
   
}

