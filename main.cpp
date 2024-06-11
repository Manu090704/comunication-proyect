#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "customer.h"
#include "operator.h"
#include "vox.h"
#include "internet.h"
#include "bill.h"

void readInputFile(const std::string& filename, std::vector<Customer*>& Customers, std::vector<Operator*>& Operators, int& numEvents, std::vector<std::string>& events) {
    std::ifstream infile(filename.c_str());
    if (!infile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de entrada." << std::endl;
        exit(EXIT_FAILURE);
    }

    int numCustomers, numOperators;
    infile >> numCustomers >> numOperators >> numEvents;
    events.resize(numEvents);
    infile.ignore();  // Para ignorar el resto de la línea después de leer los números iniciales.

    for (int i = 0; i < numEvents; ++i) {
        std::getline(infile, events[i]);
    }

    infile.close();
}

void writeOutputFile(const std::string& filename, const std::vector<Customer*>& Customers, const std::vector<Operator*>& Operators) {
    std::ofstream outfile(filename.c_str());
    if (!outfile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de salida." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Imprimir información de los operadores
    for (size_t i = 0; i < Operators.size(); ++i) {
        outfile << "Operator " << Operators[i]->getId() << ": "
                << Operators[i]->getTotalSpentTalkingTime() << " "
                << Operators[i]->getTotalMessageSent() << " "
                << std::fixed << std::setprecision(2) << Operators[i]->getTotalInternetUsage() << std::endl;
    }

    // Imprimir información de los clientes
    for (size_t i = 0; i < Customers.size(); ++i) {
        outfile << "Customer " << Customers[i]->getId() << ": "
                << std::fixed << std::setprecision(2) << Customers[i]->getBill()->getTotalMoneySpent() << " "
                << Customers[i]->getBill()->getCurrentDebt() << std::endl;
    }

    // Encontrar y escribir el cliente que más ha hablado
    int maxTalkingTime = -1;
    Customer* topTalker = 0;
    for (size_t i = 0; i < Customers.size(); ++i) {
        if (Customers[i]->getTotalSpentTalkingTime() > maxTalkingTime) {
            maxTalkingTime = Customers[i]->getTotalSpentTalkingTime();
            topTalker = Customers[i];
        }
    }
    outfile << topTalker->getName() << ": " << maxTalkingTime << std::endl;

    // Encontrar y escribir el cliente que más mensajes ha enviado
    int maxMessagesSent = -1;
    Customer* topMessenger = 0;
    for (size_t i = 0; i < Customers.size(); ++i) {
        if (Customers[i]->getTotalMessageSent() > maxMessagesSent) {
            maxMessagesSent = Customers[i]->getTotalMessageSent();
            topMessenger = Customers[i];
        }
    }
    outfile << topMessenger->getName() << ": " << maxMessagesSent << std::endl;

    // Encontrar y escribir el cliente que más ha usado Internet
    double maxInternetUsage = -1.0;
    Customer* topInternetUser = 0;
    for (size_t i = 0; i < Customers.size(); ++i) {
        if (Customers[i]->getTotalInternetUsage() > maxInternetUsage) {
            maxInternetUsage = Customers[i]->getTotalInternetUsage();
            topInternetUser = Customers[i];
        }
    }
    outfile << topInternetUser->getName() << ": " << maxInternetUsage << std::endl;

    outfile.close();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Error: Se requieren los nombres de los archivos de entrada y salida." << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];

    std::vector<Customer*> Customers;
    std::vector<Operator*> Operators;
    int numEvents;
    std::vector<std::string> events;

    readInputFile(inputFilename, Customers, Operators, numEvents, events);

    // Procesar eventos
    for (int i = 0; i < numEvents; ++i) {
        std::istringstream ss(events[i]);
        int eventType;
        ss >> eventType;

        if (eventType == 1) {
            std::string name;
            int age, OperatorId;
            double limitingAmount;
            ss >> name >> age >> OperatorId >> limitingAmount;
            Customers.push_back(new Customer(Customers.size(), name, age, Operators[OperatorId], limitingAmount));
        } else if (eventType == 2) {
            int opType;
            double talkingCharge, messageCost, networkCharge, discountRate;
            ss >> opType >> talkingCharge >> messageCost >> networkCharge >> discountRate;
            if (opType == 1) {
                Operators.push_back(new VoxOperator(Operators.size(), talkingCharge, messageCost, networkCharge, (int)discountRate, "VOX"));
            } else if (opType == 2) {
                Operators.push_back(new InternetOperator(Operators.size(), talkingCharge, messageCost, networkCharge, (int)discountRate, "INTERNET"));
            }
        } else if (eventType == 3) {
            int idCustomer1, idCustomer2, time;
            ss >> idCustomer1 >> idCustomer2 >> time;
            Customers[idCustomer1]->talk(time, *Customers[idCustomer2]);
        } else if (eventType == 4) {
            int idCustomer1, idCustomer2, quantity;
            ss >> idCustomer1 >> idCustomer2 >> quantity;
            Customers[idCustomer1]->message(quantity, *Customers[idCustomer2]);
        } else if (eventType == 5) {
            int idCustomer;
            double amount;
            ss >> idCustomer >> amount;
            Customers[idCustomer]->connection(amount);
        } else if (eventType == 6) {
            int idCustomer;
            double amount;
            ss >> idCustomer >> amount;
            Customers[idCustomer]->getBill()->pay(amount);
        } else if (eventType == 7) {
            int idCustomer, idOperator;
            ss >> idCustomer >> idOperator;
            Customers[idCustomer]->setOperator(Operators[idOperator]);
        } else if (eventType == 8) {
            int idCustomer;
            double amount;
            ss >> idCustomer >> amount;
            Customers[idCustomer]->getBill()->changeTheLimit(amount);
        }
    }

    writeOutputFile(outputFilename, Customers, Operators);

    // Liberar memoria
    for (size_t i = 0; i < Customers.size(); ++i) {
        delete Customers[i];
    }
    for (size_t i = 0; i < Operators.size(); ++i) {
        delete Operators[i];
    }

    return 0;
}
