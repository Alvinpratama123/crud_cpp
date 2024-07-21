#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "crud_cpp";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Connected to the database successfully." << endl;
        } else {
            cerr << "Connection failed: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init failed" << endl;
    }
    return conn;
}

void create_product(const string& name, const int price, const int size, const string& color) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO product (name, price, size, color) VALUES ('" << name << "', " << price << ", " << size << ", '" << color << "')";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Product successfully added." << endl;
        }
        mysql_close(conn);
    }
}

void get_products() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM product")) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Name: " << row[1] << ", Price: " << row[2] << ", Size: " << row[3] << ", Color: " << row[4] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void update_product(int idproduct, const string& name, const int price, const int size, const string& color) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE product SET name = '" << name << "', price = " << price << ", size = " << size << ", color = '" << color << "' WHERE  idproduct = " << idproduct;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Product successfully updated." << endl;
        }
        mysql_close(conn);
    }
}

void delete_product(int idproduct) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM product WHERE idproduct = " << idproduct;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Product successfully deleted." << endl;
        }
        mysql_close(conn);
    }
}

int main() {
    int choice;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add Product\n";
        cout << "2. Show All Products\n";
        cout << "3. Update Product\n";
        cout << "4. Delete Product\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name, color;
            int price, size;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter price: ";
            cin >> price;
            cout << "Enter size: ";
            cin >> size;
            cout << "Enter color: ";
            cin.ignore();
            getline(cin, color);
            create_product(name, price, size, color);
        } else if (choice == 2) {
            get_products();
        } else if (choice == 3) {
            int idproduct;
            string name, color;
            int price, size;
            cout << "Enter product ID to update: ";
            cin >> idproduct;
            cin.ignore();
            cout << "Enter new name: ";
            getline(cin, name);
            cout << "Enter new price: ";
            cin >> price;
            cout << "Enter new size: ";
            cin >> size;
            cout << "Enter new color: ";
            cin.ignore();
            getline(cin, color);
            update_product(idproduct, name, price, size, color);
        } else if (choice == 4) {
            int idproduct;
            cout << "Enter product ID to delete: ";
            cin >> idproduct;
            delete_product(idproduct);
        } else if (choice == 5) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
