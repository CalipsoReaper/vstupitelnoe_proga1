#include <iostream>
#include <string>
#include <fstream>

class Product {
public:
    int id;
    std::string name;
    double cost;
    std::string supplier;

    Product(int id, std::string name, double cost, std::string supplier) { // ��������� ��� �������� ������
        this->id = id;
        this->name = name;
        this->cost = cost;
        this->supplier = supplier;
    }

    void printShortInfo() {
        std::cout << "ID: " << id << ", ��������: " << name << std::endl;
    }

    void printFullInfo() {
        std::cout << "ID: " << id << ", ��������: " << name
            << ", �������������: " << cost << ", ���������: " << supplier << std::endl;
    }
};

class ProductList {
private:
    struct Node {
        Product product;
        Node* next;
        Node* prev;

        Node(Product product) : product(product), next(nullptr), prev(nullptr) {} // ��������� ��� ����
    };

    Node* head;
    Node* tail;

public:
    ProductList() : head(nullptr), tail(nullptr) {} // ��������� ��� ������

    ~ProductList() { // ������� ������
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

    void removeProduct(int id) { // �������� ������ �� ����
        Node* current = head;
        while (current) {
            if (current->product.id == id) { // ���� � ������ ID
                if (current->prev) { // ���� ���� ����
                    current->prev->next = current->next;
                }
                else { // ���� 1 ����
                    head = current->next;
                }
                if (current->next) { // ���� ���� ������ ����
                    current->next->prev = current->prev;
                }
                else { // ���� ��� ��������� ����
                    tail = current->prev;
                }
                delete current;
                return;
            }
            current = current->next;
        }
        std::cout << "����� � ID " << id << " �� ������." << std::endl;
    }

    void saveToFile(std::string filename) { // ����������
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
        std::cout << "������ �������� � ���� " << filename << std::endl;
    }

    void loadFromFile(std::string filename) { // �������� �� ������
        std::ifstream file(filename);
        int id;
        std::string name, supplier;
        double cost;
        while (file >> id >> name >> cost >> supplier) {
            addProduct(Product(id, name, cost, supplier)); // ���������� � ������
        }
        file.close();
        std::cout << "������ �������� �� ����� " << filename << std::endl;
    }

    void printList() { // ����� ������
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

    list.addProduct(Product(1, "����� 1", 100.0, "��������� 1")); // ���������� ������ � ������
    list.addProduct(Product(2, "����� 2", 200.0, "��������� 2"));

    std::cout << "������ �������:" << std::endl;
    list.printList();

    std::cout << "������� ����� � ID 1..." << std::endl;
    list.removeProduct(1);

    std::cout << "������ ������� ����� ��������:" << std::endl;
    list.printList();

    list.saveToFile("products.txt");

    ProductList newList;
    newList.loadFromFile("products.txt");

    std::cout << "����������� ������ �������:" << std::endl;
    newList.printList();

    return 0;
}