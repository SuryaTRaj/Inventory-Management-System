#include <iostream>
#include <memory>
#include <Windows.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
int constexpr timeval = 3000;

struct logInfo
{
    string username, password;
};

int serialCounter = 0;
map<string, logInfo> mapInfo;

struct product
{
    string name;
    int price{ 0 };
    int numItems{ 0 };
};

map<string, product> mapItem,  mapSales;

class QuantityInfo  // Displays name price and quantity information of the products
{
public:
    void printThings()
    {
        char q;
        cout << "\n\n\nList of Things\n\n";
        cout << "Name" << "\t\t" << "Price" << "\t\t" << "Quantity" << endl;

        for (auto it : mapItem)  // Low on stock error handling
        {
            cout << it.first << "\t\t" << it.second.price << "\t\t" << it.second.numItems;
            if (it.second.numItems <= 5)
            {
                cout << "     ***********Low On Stock***********";
            }
            cout << endl;
        }
        cout << "\n\n\nPress Q to go back: \n";
        cin >> q;
    }

    void salesReport()  // To display Sales report
    {
        char q;
        int sum = 0;

        cout << "\n\n\nSales Report\n\n";
        cout << "Name" << "\t\t" << "Price" << "\t\t" << "Quantity" << "\t\t"<<"Total item Price" << endl;

        for (auto it : mapSales)
        {
            sum += it.second.price * it.second.numItems;
            cout << it.first << "\t\t" << it.second.price << "\t\t" << it.second.numItems <<"\t\t\t"<< it.second.price * it.second.numItems;
            cout << endl;
        }
        cout << "\n\n\n\n\t\t\t\t-------------Total Sales Amount-----------------: "<<sum<<"\n";
        cout << "\n\n\nPress Q to go back: \n";
        cin >> q;
    }

    void Add(string name, double price, int qty)  // To add items to stock
    {
        if (mapItem.find(name) == mapItem.end())
        {
            product p;
            p.price = price;
            p.numItems += qty;
            mapItem[name] = p;
            cout << "\nItem Added Successfully " << endl;
            Sleep(timeval);
        }
        else
        {
            cout << "\nItem already Added " << endl;
            Sleep(timeval);
        }
    }

    void Update(string name, int qty, double price)  // To update items to stock
    {
        if (mapItem.find(name) != mapItem.end())
        {
            if (mapItem[name].numItems = +qty >= 0)
            {
                if(price != 0)
                    mapItem[name].price = price;
                mapItem[name].numItems = +qty;
                cout << "\nItem Updated Successfully " << endl;
                Sleep(timeval);
            }
        }
        else
        {
            cout << "\nItem is not present in our list. Please Add before Updating " << endl;
            Sleep(timeval);
        }
    }

    void Delete(string name)  // To delete items from the stock
    {
        if (mapItem.find(name) != mapItem.end())
        {
            mapItem.erase(name);
            cout << "\nItem Deleted Successfully " << endl;
            Sleep(timeval);
        }
        else
        {
            cout << "\nItem is not present in our list. Cannot be Deleted " << endl;
            Sleep(timeval);
        }
    }
};

QuantityInfo quantity;

class Customer
{   
public:
    string  name, contact;
    int serialNo;
    map<string, product> productlist;

    Customer() = default;
    ~Customer() = default;

    void filldetails(string mobile)
    {
        cout << "Name          : ";
        cin >> name;
        ++serialCounter;
        serialNo = serialCounter;
        contact = mobile;
    }

    void purchase()
    {
        product prod;
        cout << "Item Name     : ";
        cin >> prod.name;

        if (mapItem.find(prod.name) == mapItem.end())
        {
            cout << "Item not present in the Stock.. Please check :-)"<<endl;
            Sleep(timeval);
            return;
        }
        cout << "\n";
        cout << "Item Price    : ";
        cin >> prod.price;
        cout << "\n";
        cout << "Item Quantity : ";
        cin >> prod.numItems;
        cout << "\n";

        auto item = mapItem.find(prod.name);
        item->second.numItems -= prod.numItems;
      
        ofstream sales("sales.txt", fstream::app);
        if (sales.is_open())
        {
                sales << prod.name << " " << prod.price << " " << prod.numItems << "\n";
        }
        else
        {
            cout << "File can not be Opened" << endl;
        }
        sales.close();

        if (mapSales.find(prod.name) != mapSales.end())
        {
            mapSales[prod.name].numItems += prod.numItems;
        }
        else
        {
            mapSales[prod.name] = prod;
        }

        productlist[prod.name] = prod;         
    }

    void showCustomerInfo()  // To show customer invoice information
    {
        char q;
        int sum = 0;

        cout << "\t\t\tCustomer Invoice Info\n\n\n" << endl;
        cout << "Customer Name : " << name << endl;
        cout << "Serial Number : " << serialNo << endl;
        cout << "Mobile Number : " << contact << endl;
        cout << "\n\nItem Name" << "\t\t\t" << "Item Price" << "\t\t\t" << "Item Quantity"<< "\t\t\t"<<"Total Price\n";

        for (auto it : productlist)
        {
            sum += it.second.price * it.second.numItems;
            cout << it.first << "\t\t\t\t" << it.second.price << "\t\t\t\t" << it.second.numItems<<"\t\t\t\t"<< it.second.price* it.second.numItems << "\n";
        }
        cout << "\n\n\n\n\t\t-------------Total Sales Amount-----------------: " << sum << "\n";
        cout << "\n\n\nPress Q to go back: \n";
        cin >> q;
    }
};

map<string, Customer> mapCustomer;

class Management
{
    string username, password;
    
public:
    virtual void entry() = 0;

    void clearscreen()
    {
        system("cls");
    }

    bool start()
    {
        ifstream file;
        file.open("login.txt", ios::out | ios::in);
        if (file.is_open())
        {
            string line;
            while (getline(file, line, '\n'))
            {
                istringstream s(line);
                int index;
                string value, user;
                for (int i = 0; i < line.length();i++)
                {
                    if (line[i] == ' ')
                    {
                        index = i;
                        break;
                    }
                    else
                    {
                        value += line[i];
                    }
                }
                user = value;
                value = "";
                for (int i = index+1; i < line.length();i++)
                {
                    if (line[i] == ' ')
                    {
                        index = i;
                        break;
                    }
                    else
                    {
                        value += line[i];
                    }
                }
                mapInfo[user] = logInfo{ user,  value };
            }
        }

       ifstream stock;
       stock.open("stock.txt", ios::out | ios::in);
       if (stock.is_open())
       {
           string line;
           product prod;
           while (getline(stock, line, '\n'))
           {
               istringstream s(line);
               int index{};
               string name, price, numItems, value;
               for (int i = 0; i < line.length();i++)
               {
                   if (line[i] == ' ')
                   {
                       index = i;
                       break;
                   }
                   else
                   {
                       value += line[i];
                   }
               }
               prod.name = value;
               value = "";
               for (int i = index + 1; i < line.length();i++)
               {
                   if (line[i] == ' ')
                   {
                       index = i;
                       break;
                   }
                   else
                   {
                       value += line[i];
                   }
               }
               price = value;
               try
               {
                   std::istringstream(price) >> prod.price;
               }
               catch (const std::exception& e)
               {
                   std::cout << "EXCEPTION::" << e.what() << endl;
               }

               value = "";
               for (int i = index + 1; i < line.length();i++)
               {
                   if (line[i] == ' ')
                   {
                       index = i;
                       break;
                   }
                   else
                   {
                       value += line[i];
                   }
               }
               numItems = value;
               try
               {
                   std::istringstream(numItems) >> prod.numItems;
               }
               catch (const std::exception&e)
               {
                   std::cout << "EXCEPTION::" << e.what() << endl;
               }
               mapItem[prod.name] = prod;
           }
       }

       ifstream sales;
       sales.open("sales.txt", ios::out | ios::in);
       if (sales.is_open())
       {
           string line;
           product prod;
           while (getline(sales, line, '\n'))
           {
               istringstream s(line);
               int index{};
               string name, price, numItems, value;
               for (int i = 0; i < line.length();i++)
               {
                   if (line[i] == ' ')
                   {
                       index = i;
                       break;
                   }
                   else
                   {
                       value += line[i];
                   }
               }
               prod.name = value;
               value = "";
               for (int i = index + 1; i < line.length();i++)
               {
                   if (line[i] == ' ')
                   {
                       index = i;
                       break;
                   }
                   else
                   {
                       value += line[i];
                   }
               }
               price = value;
               try
               {
                   std::istringstream(price) >> prod.price;
               }
               catch (const std::exception& e)
               {
                   std::cout << "EXCEPTION::" << e.what() << endl;
               }
               value = "";
               for (int i = index + 1; i < line.length();i++)
               {
                   if (line[i] == ' ')
                   {
                       index = i;
                       break;
                   }
                   else
                   {
                       value += line[i];
                   }
               }
               numItems = value;
               try
               {
                   std::istringstream(numItems) >> prod.numItems;
               }
               catch (const std::exception& e)
               {
                   std::cout << "EXCEPTION::" << e.what() << endl;
               }
               mapSales[prod.name] = prod;
           }
       }
   
        char ch;
        cout << "N--> New Registration\n";
        cout << "E--> Existing user\n";
        cin >> ch;

        switch (ch) // Switch cases for registration or login
        {
        case 'N':
        case 'n':
        {
            return Registration();
            break;
        }
        case 'E':
        case 'e':
        {
            return login();
            break;
        }
        }
    }

    bool login()  // To login as an existing user
    {
        clearscreen();
        cout << "Welcome to Login page.\n";
        cout << "\n\n\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        bool state = validuser(username, password);
        if (state)
        {
            cout << "\n\n\nLogin Successful :-)\n\n\n";
            Sleep(timeval);
        }
        return state;
    }

    bool validuser(string user, string pass)
    {
        bool status = false;

        if (mapInfo.find(user) != mapInfo.end())
        {
            if (mapInfo[user].password == pass)
            {
                status = true;
            }
            else
            {
                cout << "\n\n\nWrong Password.. Please Enter Correct Password :-(\n\n\n";
                Sleep(timeval);
            }
        }
        else
        {
            cout << "\n\n\nInvalid UserName :-(\n\n\n";
            Sleep(timeval);
        }
        return status;
    }

    bool Registration()  // To register as a new user
    {
        clearscreen();
        cout << "Welcome to Registration page.\n";
        cout << "Please Enter the Following Details.\n\n\n";
        cout << "Username     : ";
        cin >> username;
        cout << "Password     : ";
        cin >> password;

        mapInfo[username] = logInfo{ username, password };
        cout << "\n\n\nSuccesfully Registered\n\n\n";
        try
        {
            ofstream data("login.txt", fstream::app);
            if (data.is_open())
            {
                data << username << " " << password << "\n";
            }
            else
            {
                cout << "File can not be Opened" << endl;
            }
            data.close();
        }
        catch (const std::exception& e)
        {
            cout << "EXCEPTION::" << e.what() << endl;
        }
        Sleep(timeval);
        return login();
    }
};

class Owner : public Management
{
public:
    void entry() override  // To access the owner page
    {
        clearscreen();

        cout << "Welcome to Owner page.\n";
        if (start())
        {
            bool flag_owner = true;
            char c;

            while (flag_owner)
            {
                clearscreen();
                try
                {
                    ofstream data("stock.txt", fstream::app);
                    if (data.is_open())
                    {
                        for (auto it : mapItem)
                        {
                            data << it.first << " " << it.second.price <<" "<<it.second.numItems << "\n";
                        }                      
                    }
                    else
                    {
                        cout << "File can not be Opened" << endl;
                    }
                    data.close();

              
                }
                catch (const std::exception& e)
                {
                    cout << "EXCEPTION::" << e.what() << endl;
                }
                cout << "Welcome!! Please choose the option below.\n";
                cout << "A--> ADD Items\n";
                cout << "D--> Delete Items\n";
                cout << "U--> Update Items\n";
                cout << "S--> Sales Report\n";
                cout << "N--> New Customer\n";
                cout << "C--> Fetch Customer Info\n";
                cout << "F--> Fetch Stock Info\n";
                cout << "M--> Main menu\n";
                cin >> c;

                switch (c)  // Switch cases to add delete and update items from stock 
                            //To sign in new customer and to fetch customer and stock information 
                {
                    case 'A':
                    case 'a':
                    {
                        clearscreen();
                        string prodname;
                        double prodprice;
                        int prodqty;

                        cout << "Please Enter Product name to Add : ";
                        cin >> prodname;
                        cout << "\n";
                        cout << "Please Enter Price of the product: ";
                        cin >> prodprice;
                        cout << "\n";
                        cout << "Please Enter Quantity of the product: ";
                        cin >> prodqty;
                        quantity.Add(prodname, prodprice, prodqty);
                        cout << "\n\n\n\n";
                        break;
                    }
                    case 'D':
                    case 'd':
                    {
                        clearscreen();
                        string prodname;

                        cout << "Please Enter Product name to Delete : ";
                        cin >> prodname;
                        quantity.Delete(prodname);
                        cout << "\n\n\n\n";
                        break;
                    }
                    case 'U':
                    case 'u':
                    {
                        clearscreen();
                        string prodname;
                        int prodqty;
                        int prodprice;

                        cout << "Please Enter Product name to Update     : ";
                        cin >> prodname;
                        cout << "\n";
                        cout << "Please Enter Product Quantity to Update : ";
                        cin >> prodqty;
                        cout << "\n";
                        cout << "Please Enter Product Price to Update: ";
                        cin >> prodprice;
                        quantity.Update(prodname, prodqty, prodprice);
                        cout << "\n\n\n\n";
                        break;
                    }
                    case 'S':
                    case 's':
                    {
                        clearscreen();
                        quantity.salesReport();
                        break;
                    }
                    case 'C':
                    case 'c':
                    {
                        string mobile;
                        cout << "Enter Customer Number : ";
                        cin >> mobile;
                        clearscreen();

                        if (mapCustomer.find(mobile) != mapCustomer.end())
                        {
                            mapCustomer[mobile].showCustomerInfo();
                        }
                        else
                        {
                            cout << "Customer is not Registered. Please Register\n\n";
                            Sleep(timeval);
                        }
                        break;
                    }
                    case 'N':
                    case 'n':
                    {
                        clearscreen();
                        Customer cus;
                        string mobile;

                        cout << "Welcome Customer Registration Page\n\n";
                        cout << "Please Enter the Following Details.\n\n\n";
                        cout << "Enter Customer Number : ";
                        cin >> mobile;
                        cout << "\n\n";

                        if (mapCustomer.find(mobile) == mapCustomer.end())
                        {
                            cus.filldetails(mobile);
                            mapCustomer[mobile] = cus;
                        }
                        else
                        {
                            cout << "Customer is already Registered.\n\n";
                            Sleep(timeval);
                        }
                        break;
                    }
                    case 'F':
                    case 'f':
                    {
                        clearscreen();
                        quantity.printThings();
                        break;
                    }
                    case 'M':
                    case 'm':
                    {
                        flag_owner = false;
                        break;
                    }
                }
            }
        }
    }
};

class Staff : public Management
{
public:
    void entry() override  // To fetch existing or new customer information
    {
        clearscreen();
        cout << "Welcome to Staff page.\n";
        if (start())
        {
            bool flag_staff = true;
            char c;
            while (flag_staff)
            {
                clearscreen();
                cout << "E--> Existing Customer\n";
                cout << "F--> Fetch Customer Info\n";
                cout << "N--> New Customer\n";
                cout << "M--> Main menu\n";
                cin >> c;
                switch (c)
                {
                    case 'E':
                    case 'e':
                    {
                        clearscreen();
                        string mobile;
                        cout << "Customer Info Page\n\n";
                        cout << "Enter Customer Number : ";
                        cin >> mobile;
                        cout << "\n\n";
                        if (mapCustomer.find(mobile) != mapCustomer.end())
                        {
                            mapCustomer[mobile].purchase();
                        }
                        else
                        {
                            cout << "Customer is not Registered. Please Register\n\n";
                            Sleep(timeval);
                        } 
                        break;
                    }
                    case 'F':
                    case 'f':
                    {
                        string mobile;
                        cout << "Enter Customer Number : ";
                        cin >> mobile;
                        clearscreen();

                        if (mapCustomer.find(mobile) != mapCustomer.end())
                        {
                            mapCustomer[mobile].showCustomerInfo();
                        }
                        else
                        {
                            cout << "Customer is not Registered. Please Register\n\n";
                            Sleep(timeval);
                        }
                        break;
                    }
                    case 'N':
                    case 'n':
                    {
                        clearscreen();
                        Customer cus;
                        string mobile;
                        cout << "Welcome Customer Registration Page\n\n";
                        cout << "Please Enter the Following Details.\n\n\n";
                        cout << "Enter Customer Number : ";
                        cin >> mobile;
                        cout << "\n\n";

                        if (mapCustomer.find(mobile) == mapCustomer.end())
                        {
                            cus.filldetails(mobile);
                            mapCustomer[mobile] = cus;
                        }
                        else
                        {
                            cout << "Customer is already Registered.\n\n";
                            Sleep(timeval);
                        }
                        break;
                    }
                    case 'M':
                    case 'm':
                    {
                        flag_staff = false;
                        break;
                    }
                }
            }
        }
    }
};

void Menu(bool& run)  // Main page of the project with owner and staff login menu
{
    system("cls");
    Management* manage;

    char ch;
    cout << "\t\t\t\t Inventory Management System\n";
    cout << "\t\t\t\t\t Grocery Store";
    cout << "\n\n\n";
    cout << "Welcome! Please choose the option below.\n";
    cout << "A--> Owner\n";
    cout << "B--> Staff\n";
    cout << "Q--> Quit or Exit\n";
    cin >> ch;

    switch (ch) // Switch cases to login as new owner of new staff
    {
        case 'A':
        case 'a':
        {
            manage = new Owner();
            manage->entry();
            break;
        }  
        case 'B':
        case 'b':
        {
            manage = new Staff();
            manage->entry();
            break;
        }
        case 'Q':
        case 'q':
        {          
            cout << "\n\n\n\nThank you!!\n\n\n\n";
            run = false;
            break;
        }
    }
}

int main()  // Main function to execute the program
{
    bool flag = true;

    while (flag)
    {
        Menu(flag);
    }
    return 0;
}

