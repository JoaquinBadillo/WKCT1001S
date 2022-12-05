#include <iostream>

#include "Shop.hpp"

int main() {
  Shop test("Tester");

  std::string rawInput, ans, name, brand, type, desc, key, price, quantity;
  int option;

  test.readData();
  test.welcome();

  do {
    bool invalid = true;
    bool invalidQuantity = true;

    int restock = 0;
    int sold = 0;

    test.showMenu();
    std::getline(std::cin, rawInput);

    try {
      option = stoi(rawInput);
    } catch(std::invalid_argument e) {
      std::cerr << "Error: Option must be a whole number!\n" << std::endl;
      option = -1;
    }

    switch (option) {
      case -1:
        break;

      case 1:
        std::cout << "\nType the key of the product you are looking for: ";
        std::getline(std::cin, key);
        try {
          test.findProduct(key);
        } catch (shopException e) {
          std::cerr << e.what();
        }

        std::cout << "\nPress enter to continue..." << std::endl;
        std::getline(std::cin, ans);

        break;

      case 2:
        std::cout << "\nType the name of the products you are searching for: ";
        std::getline(std::cin, name);

        try {
          test.searchProducts(name);
        } catch (shopException e) {
          std::cerr << e.what();
        }

        std::cout << "\nPress enter to continue..." << std::endl;
        std::getline(std::cin, ans);

        break;

      case 3:
        int price_;
        int quantity_;
        std::cout << "\nName of the product: ";
        std::getline(std::cin, name);
        std::cout << "\nBrand: ";
        std::getline(std::cin, brand);
        std::cout << "\nType: ";
        std::getline(std::cin, type);
        std::cout << "\nBrief description: ";
        std::getline(std::cin, desc);
        do {
          std::cout << "\nPrice: ";
          std::getline(std::cin, price);

          try {
            price_ = stof(price);
            if (price_ >= 0)
              invalidQuantity = false;
            else
              std::cerr << "Price cannot be negative, try again" << std::endl;
          } catch (std::invalid_argument e) {
            std::cerr << "Price must be a number, try again" << std::endl;
          }
        } while (invalidQuantity);
        invalidQuantity = true;

        std::cout << "\nKey: ";
        std::getline(std::cin, key);

        do {
          std::cout << "\nQuantity: ";
          std::getline(std::cin, quantity);

          try {
            quantity_ = stoi(quantity);
            if (quantity_ >= 0)
              invalidQuantity = false;
            else
              std::cerr << "Quantity cannot be negative, try again" << std::endl;
          } catch (std::invalid_argument e) {
            std::cerr << "Quantity must be a number, try again" << std::endl;
          }
        } while (invalidQuantity);
          
        try {
          test.addNewProduct(key, name, brand, type, desc, stof(price), stoi(quantity));
          invalid = false;
        } catch(shopException e) {
          std::cerr << e.what();
          std::cout << "\nPress enter to try again or type \"c\" to cancel" << std::endl;
          std::getline(std::cin, ans);
          if (ans == "c")
            invalid = false;
        }       

        std::cout << "\nPress enter to continue..." << std::endl;
        std::getline(std::cin, ans);

        break;

      case 4:
        do {
          std::cout << "\nType the key of the product you are looking for: ";
          std::getline(std::cin, key);

          if (test.findProduct(key)) {
            do {
              std::cout << "\nType the quantity to restock: ";
              std::getline(std::cin, quantity);
              try {
                restock = stoi(quantity);
                try {
                  test.addToStock(key, restock);
                  invalidQuantity = false;
                  invalid = false;
                } catch (productException e) {
                  std::cerr << e.what();

                  std::cout << "\nPress enter to try again or type \"c\" to cancel" << std::endl;
                  std::getline(std::cin, ans);
                  if (ans == "c")
                    invalidQuantity = false;
                }
              } catch(std::invalid_argument e) {
                std::cerr << "Error: You must use an integer" << std::endl;

                std::cout << "\nPress enter to try again or type \"c\" to cancel" << std::endl;
                std::getline(std::cin, ans);
                if (ans == "c")
                  invalidQuantity = false;
              }
            } while (invalidQuantity);
          } else {
            std::cout << "\nProduct not found" << std::endl;

            std::cout << "\nPress enter to try again or type \"c\" to cancel" << std::endl;
            std::getline(std::cin, ans);
            if (ans == "c")
              invalid = false;
          }
        } while (invalid);

        std::cout << "\nPress enter to continue..." << std::endl;
        std::getline(std::cin, ans);

        break;

      case 5:
        do {
          std::cout << "\nType the key of the product you are looking for: ";
          std::getline(std::cin, key);

          if (test.findProduct(key)) {
            bool invalidQuantity = true;
            do {
              std::cout << "\nType the quantity sold: ";
              std::getline(std::cin, quantity);
              try {
                sold = stoi(quantity);
                try {
                  test.sell(key, sold);
                  invalidQuantity = false;
                  invalid = false;
                } catch (productException e) {
                  std::cerr << e.what();

                  std::cout << "\nPress enter to try again or type \"c\" to cancel" << std::endl;
                  std::getline(std::cin, ans);
                  if (ans == "c")
                    invalidQuantity = false;
                }
              } catch(std::invalid_argument e) {
                std::cerr << "Error: You must use an integer" << std::endl;

                std::cout << "\nPress enter to try again or type \"c\" to cancel" << std::endl;
                std::getline(std::cin, ans);
                if (ans == "c")
                  invalidQuantity = false;
              }
            } while (invalidQuantity);
          } else {
            std::cout << "\nProduct not found" << std::endl;

            std::cout << "\nPress enter to try again or type \"c\" to cancel" << std::endl;
            std::getline(std::cin, ans);
            if (ans == "c")
              invalid = false;
          }
        } while (invalid);

        std::cout << "\nPress enter to continue..." << std::endl;
        std::getline(std::cin, ans);

        break;

      case 6:
        break;

      default:
        std::cout << "Number must be between 1 and 6\n";
        break;
    }
  } while (option != 6);

  std::cout << "Program ended correctly" << std::endl;
  return 0;
}
