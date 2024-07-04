#include <string>
#include <iostream>
#include <utility>
#include <vector>

/* 

Improvements made:
1. Added "using namespace std" to declutter and simplify code.
2. Changed weird spacing with the constructors.
3. Changed items array from a C-style array to a C++ vector. This allows us to add an infinite number of items without having to manually
   resize our array once we reach the capacity of the array.
4. Handled deletion of items if their quantity reaches 0.

More areas for improvement:
1. I noticed that we currently do linear search to find the items. In the future, a dictionary could perhaps be used to quicken look up time.
2. Currently, there is only an option to add a new item. It would be useful if the option were simply "Add item" and the code checked if the
   item already existed in the database. From there, we could either add the item to the items vector (if it did not exist before) or increment
   the quantity for the item.
3. Related to 2 is another bug, where you can enter two items with the same name. This results in duplicate items in the vector as well
   as strange situations where you can have two items be named the same thing, but have different prices. Fixing this bug would involve
   implementing both 1 and 2 (i.e., using a dictionary, since keys must be unique in a dictionary). However, this is outside the scope of
   this ticket, as this ticket is only focused on making items disappear from the inventory if their quantity is updated to 0.
4. In the future, I would like to use smart pointers, which automate garbage collection for you while also retaining heap allocation.
   For the purposes of this challenge, I used manual memory allocation for simplicity.

*/

using namespace std;

class Item {
private:
    string name;
    int quantity;
    float price;

public:
    Item(string name, int quantity, float price) : name{move(name)}, quantity{quantity}, price{price} {}

    string get_name() const {
        return name;
    }

    int get_quantity() const {
        return quantity;
    }

    void set_quantity(int new_quantity) {
        quantity = new_quantity;
    }

    float get_price() const {
        return price;
    }

    bool is_match(const string &other) {
        return name == other;
    }
};

class Inventory {
private:
    vector<Item*> items;
    float total_money;
    int item_count;

    static void display_data(Item &item) {
        cout << "\nItem name: " << item.get_name();
        cout << "\nQuantity: " << item.get_quantity();
        cout << "\nPrice: " << item.get_price();
    }

public:
    Inventory() : items{}, total_money{0}, item_count{0} {}

    void add_item() {
        string name;
        int quantity;
        float price;

        cin.ignore();
        cout << "\nEnter item name: ";
        cin >> name;
        cout << "Enter quantity: ";
        cin >> quantity;
        cout << "Enter price: ";
        cin >> price;

        items.push_back(new Item(name, quantity, price));
        item_count++;
    }

    void sell_item() {
        string item_to_check;
        cin.ignore();
        cout << "\nEnter item name: ";
        cin >> item_to_check;

        for (int i = 0; i < item_count; i++) {
            if (items[i]->is_match(item_to_check)) {
                remove_item(i);
                return;
            }
        }
        cout << "\nThis item is not in your Inventory";
    }

    void remove_item(int item_index) {
        int input_quantity;
        Item *item = items[item_index];
        cout << "\nEnter number of items to sell: ";
        cin >> input_quantity;

        int quantity = item->get_quantity();
        if (input_quantity <= quantity) {
            float price = item->get_price();
            float money_earned = price * input_quantity;
            item->set_quantity(quantity - input_quantity);
            cout << "\nItems sold";
            cout << "\nMoney received: " << money_earned;
            total_money += money_earned;

            if (item->get_quantity() == 0) {
                delete_item(item_index);
            }
        } else {
            cout << "\nCannot sell more items than you have.";
        }
    }

    void delete_item(int item_index) {
        // Deallocate memory
        delete items[item_index];

        // Delete "garbage" item at index
        items.erase(items.begin() + item_index);
        item_count--;
    }

    void list_items() {
        if (item_count == 0) {
            cout << "\nInventory empty.";
            return;
        }

        for (int i = 0; i < item_count; i++) {
            display_data(*items[i]);
            cout << "\n";
        }
    }
};

// no need to modify anything here
int main() {
    int choice;
    Inventory inventory_system;
    cout << "Welcome to the inventory!";

    while (1) {
        cout << "\n\nMENU\n"
                  << "1. Add new item\n"
                  << "2. Sell item\n"
                  << "3. List items\n"
                  << "4. Exit\n\n"
                  << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                inventory_system.add_item();
                break;

            case 2:
                inventory_system.sell_item();
                break;

            case 3:
                inventory_system.list_items();
                break;

            case 4:
                exit(0);

            default:
                cout << "\nInvalid choice entered";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                break;
        }
    }
}
