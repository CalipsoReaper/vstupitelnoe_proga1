#include <iostream>
#include <string>
#include <fstream>

class Product {
public:
    int id;
    std::string name;
    double cost;
    std::string supplier;

    Product(int id, std::string name, double cost, std::string supplier) { // конструкт для создания товара
        this->id = id;
        this->name = name;
        this->cost = cost;
        this->supplier = supplier;
    }

    void printShortInfo() {
        std::cout << "ID: " << id << ", Название: " << name << std::endl;
    }

    void printFullInfo() {
        std::cout << "ID: " << id << ", Название: " << name
            << ", Себестоимость: " << cost << ", Поставщик: " << supplier << std::endl;
    }
};

class ProductList {
private:
    struct Node {
        Product product;
        Node* next;
        Node* prev;

        Node(Product product) : product(product), next(nullptr), prev(nullptr) {} // конструкт для узла
    };

    Node* head;
    Node* tail;

public:
    ProductList() : head(nullptr), tail(nullptr) {} // конструкт для списка

    ~ProductList() { // очистка памяти
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addProduct(Product product) {
        Node* newNode = new Node(product);
        if (!head) {
            head = tail = newNode;
        }
        else { 
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void removeProduct(int id) { // удаление товара по айди
        Node* current = head;
        while (current) {
            if (current->product.id == id) { // если с нужным ID
                if (current->prev) { // Если есть узел
                    current->prev->next = current->next;
                }
                else { // если 1 узел
                    head = current->next;
                }
                if (current->next) { // Если есть другой узел
                    current->next->prev = current->prev;
                }
                else { // если это последний узел
                    tail = current->prev;
                }
                delete current;
                return;
            }
            current = current->next;
        }
        std::cout << "Товар с ID " << id << " не найден." << std::endl;
    }

    void saveToFile(std::string filename) { // сохранение
        std::ofstream file(filename);
        Node* current = head;
        while (current) {
            file << current->product.id << " "
                << current->product.name << " "
                << current->product.cost << " "
                << current->product.supplier << std::endl;
            current = current->next;
        }
        file.close();
        std::cout << "Список сохранен в файл " << filename << std::endl;
    }

    void loadFromFile(std::string filename) { // загрузка из списка
        std::ifstream file(filename);
        int id;
        std::string name, supplier;
        double cost;
        while (file >> id >> name >> cost >> supplier) {
            addProduct(Product(id, name, cost, supplier)); // добавление в список
        }
        file.close();
        std::cout << "Список загружен из файла " << filename << std::endl;
    }

    void printList() { // общий вывовд
        Node* current = head;
        while (current) {
            current->product.printFullInfo();
            current = current->next;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    ProductList list;

    list.addProduct(Product(1, "Товар 1", 100.0, "Поставщик 1")); // добавление товара в список
    list.addProduct(Product(2, "Товар 2", 200.0, "Поставщик 2"));

    std::cout << "Список товаров:" << std::endl;
    list.printList();

    std::cout << "Удаляем товар с ID 1..." << std::endl;
    list.removeProduct(1);

    std::cout << "Список товаров после удаления:" << std::endl;
    list.printList();

    list.saveToFile("products.txt");

    ProductList newList;
    newList.loadFromFile("products.txt");

    std::cout << "Загруженный список товаров:" << std::endl;
    newList.printList();

    return 0;
}