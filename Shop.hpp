#ifndef SHOP_H
#define SHOP_H

#include <fstream>
#include <sstream>

#include <vector>
#include <unordered_map>

#include "Product.hpp"

class shopException : public std::exception {
    private:
    std::string message;

    public:
    shopException(std::string msg) {
        message = msg;
    }

    const char * what () {
        return message.c_str();
    }
};

class Shop {
    private:
      std::string name;
      std::unordered_map<std::string, Product*> stock;

    public:
        // Constructors
        Shop();
        Shop(std::string);

       // Destructor
        ~Shop();
        
        // Getters
        std::string getName();
        std::unordered_map<std::string, Product*> getStock();

        // Setters
        void setName(std::string);

        // API
        void readData(); // Read data from CSV
        void addNewProduct(std::string, std::string, std::string, std::string, std::string, float, int);
        Product* findProduct(std::string); // Find product with key 
        void searchProducts(std::string); // Seach products by name
        void addToStock(std::string, int); // Add quantity to existing stock
        void sell(std::string, int); // Sell product with a given key and quantity
        void showMenu(); // Show CLI menu
        void welcome();
};

// Constructor por defecto
Shop::Shop() {
  name = " ";
}

// Constructor parametrizado
Shop::Shop(std::string name_) {
  setName(name_);
}

// Destructor
Shop::~Shop() {
  std::ofstream file("data.csv");

  file << "Key,Name,Brand,Type,Description,Price,Quantity\n";
  while (stock.begin() != stock.end()) {
    Product* temp = stock.begin() -> second;
    // Write product data to file
    file  << temp -> getKey() << "," 
          << temp -> getName() << "," 
          << temp -> getBrand() << "," 
          << temp -> getType() << "," 
          << temp -> getDesc() << "," 
          << temp -> getPrice() << "," 
          << temp -> getQuantity() << std::endl;

    // Delete product from stock and free memory
    stock.erase(stock.begin());
    delete temp;
  }
  
}

// Getters
std::string Shop::getName(){
  return this -> name;
}

std::unordered_map<std::string, Product*> Shop::getStock() {
  return this -> stock;
}

// Setters
void Shop::setName(std::string name_){
  name = name_;
}

void Shop::readData() {
  // Delete stock hash table from RAM
  while (stock.begin() != stock.end()) {
    Product* temp = stock.begin() -> second;
    stock.erase(stock.begin());
    delete temp;
  }

  // Create new hash table from csv

  std::vector<std::string> row;
	std::string line, word;

  std::fstream file("data.csv", std::ios::in);
  if(file.is_open()) {
    getline(file, line);
		while(getline(file, line)) {
      row.clear();
      std::stringstream str(line);

      while(getline(str, word, ','))
        row.push_back(word);
      
      Product* p = new Product(row[0], row[1], row[2], row[3], row[4], stof(row[5]), stoi(row[6]));
      stock.insert(make_pair(p -> getKey(), p));
		}
	} 
  else
    throw shopException("Error opening the file.\n");
  
  std::cout << "Data was loaded succesfully" << std::endl;
  file.close();
}

void Shop::addNewProduct(std::string key_, std::string name_, std::string brand_, std::string type_, std::string desc_, float price_, int quantity_) {
  if (stock.find(key_) != stock.end()) {
    throw shopException("A product has been previously registered with that key!\n");
  }
  Product* newProduct;
  try {
    newProduct = new Product(key_, name_, brand_, type_, desc_, price_, quantity_);
  } catch(productException e) {
    throw shopException(e.what());
    return;
  }
  short sentinel = stock.empty() ? 1 : 0;
  stock.insert(make_pair(newProduct -> getKey(), newProduct));

  /*
    We will add the product to the file for safety
    The file will be rewritten when the program ends to modify data from sales and restock
    We still write the data when adding products in case of an unexpected shutdown
  */

  std::ofstream file;
  file.open("data.csv", std::ios_base::app);
  if(file.is_open()) {
    if(sentinel) {
      file << "Key,Name,Brand,Type,Description,Price,Quantity\n";
    }
	  file << key_ << "," << name_ << "," << brand_ << "," << type_ << "," << desc_ << "," << price_ << "," << quantity_ << std::endl;
  } else {
    throw shopException("Could not open file\n");
  }
}

Product* Shop::findProduct(std::string key_) {
  std::unordered_map<std::string, Product*>::iterator it = stock.find(key_);
  if (it != stock.end()) {
    it -> second -> print();
    return it -> second;
  }
  
  std::cout << "Product not found" << std::endl;
  return nullptr;
}
  
void Shop::searchProducts(std::string name_) {
  // Case sensitive seach!
  // Pattern matching algorithms for strings were not considered

  std::unordered_map<std::string, Product*>::iterator it;
  int counter = 0;
  for (it = stock.begin(); it != stock.end(); ++it) {
    if (it -> second -> getName() == name_) {
      it -> second -> print();
      ++counter;
    }
  }

  if (!counter)
    std::cout << "No products found with that name" << std::endl;
}

void Shop::addToStock(std::string key_, int quantity_) {
  std::unordered_map<std::string, Product*>::iterator it = stock.find(key_);
  if (it != stock.end())
    *(it -> second) += quantity_;
  else
    throw shopException("Nonexistent product!\n");
}

void Shop::sell(std::string key_, int quantity_) {
  std::unordered_map<std::string, Product*>::iterator it = stock.find(key_);
  if (it != stock.end())
    *(it -> second) -= quantity_;
  else
    throw shopException("Nonexistent product!\n");
}

void Shop::welcome() {
  std::cout << "Welcome to \"" << this -> name << "\"" << std::endl;
}

void Shop::showMenu() {
  std::cout << "Choose one of the following options: \n" << std::endl;
  std::cout << "1) Find Product" << std::endl;
  std::cout << "2) Search Products" << std::endl;
  std::cout << "3) Add New Product" << std::endl;
  std::cout << "4) Add Stock to Product" << std::endl;
  std::cout << "5) Register Sale" << std::endl;
  std::cout << "6) Exit" << std::endl;
  std::cout << "\nEnter the number (1-6): ";
}

#endif
