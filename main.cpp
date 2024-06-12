#include <iostream>
#include <fstream>
#include <vector>
#include "bill.h"
#include "operator.h"
#include "vox.h"
#include "internet.h"
#include "customer.h"

using namespace std;

int main(int argc, char* argv[]) {
    ifstream inputFile;
    ofstream outputFile;

    if (argc != 3) {
        cout << "usage: " << argv[0] << " input_file output_file\n";
        return -1;
    }

    inputFile.open(argv[1]);
    if (!inputFile.is_open()) {
        cout << argv[0] << ": File \"" << argv[1] << "\" not found\n";
        return -1;
    }

    outputFile.open(argv[2]);
    if (!outputFile.is_open()) {
        cout << argv[0] << ": Unable to create output file \"" << argv[2] << "\"\n";
        return -1;
    }

    vector<Customer*> customers;
    vector<Operator*> operators;

    int C, O, N;
    inputFile >> C >> O >> N;

    customers.resize(C);
    operators.resize(O);

    int idClient = 0;
    int idOperator = 0;
    int age,operatorId;
    double limit;
    string name;
    int opType;
    OperatorType type;
    double talkingCharge, messageCost, networkCharge, discountRate;
    Operator* op;
    Customer* customer;
    //case 3
    int id1, id2, time;
    //Customer* customer1 = customers[id1];
    //Customer* customer2 = customers[id2];
    //case 4
    int quantity;
   // Customer* customer1 = customers[id1];
   // Customer* customer2 = customers[id2];
    //case 5
    int id, amount;
   // Customer* customer = customers[id];
    //case 6
    //int id;
    //Customer* customer = customers[id];
    //double amount;
    //case 7
    int idCustomer;
   // Customer* customer = customers[id1];
    //Operator* newOperator = operators[id2];
    //case 8
    //int id, amount;
    //Customer* customer = customers[id];
    // Leer operaciones
    for (int i = 0; i < N; ++i) {
        cout<<"indice: "<<i<<endl;
        int operationType;
        inputFile >> operationType;
        cout<<"operationtype:"<<operationType<<endl;
        switch (operationType) {
            case 1:
                inputFile >> name >> age >> operatorId >> limit;
                cout<<"nombre:"<<name<<" "<<"edad:"<<age<<" "<<"operator:"<<operatorId<<" "<<"limite:"<<limit<<endl;
                customer = new Customer(idClient, name, age, operators[operatorId], limit);
                customers[idClient] = customer;
                cout << "customer " << idClient << customers[idClient]->toString() << "\n";
                idClient++;
                break;
            case 2:
                inputFile >> opType >> talkingCharge >> messageCost >> networkCharge >> discountRate;
                cout<<"tipo de operador:"<<opType<<" "<<"cargo de llamada"<<talkingCharge<<" "<<"cargo de mensaje:"<<messageCost<<" "<<"cargo de internet:"<<networkCharge<<"descuento: "<<discountRate<<endl;
                if (opType == 1){
                    op = new VoxOperator(idOperator, talkingCharge, messageCost, networkCharge, discountRate,type);
                    cout << "op = " << op->toString() << "\n";
                    operators[idOperator] = op;
                    cout << "operator = " << operators[idOperator]->toString() << "\n";
                }    
                else if (opType == 2){
                    op = new InternetOperator(idOperator, talkingCharge, messageCost, networkCharge, discountRate,type);
                    cout << "op = " << op->toString() << "\n";
                    operators[idOperator] = op;
                    cout << "operator = " << operators[idOperator]->toString() << "\n";
                }
                idOperator++;
                break;
            case 3: 
                inputFile >> id1 >> id2 >> time;
                // Realizar operación de llamada entre clientes
                customers[id1]->talk(time, *customers[id2]);
                break;
            case 4: 
                inputFile >> id1 >> id2 >> quantity;
                // Realizar operación de envío de mensajes entre clientes
                customer[id1]->message(quantity, *customer[id2]);
                break;
            case 5: 
                
                inputFile >> id >> amount;
                // Realizar operación de conexión a internet por parte del cliente
                customer->connection(amount);
                break;
            case 6: 
                
                inputFile >> id >> amount;
                customer->pay(amount);
                break;
            case 7: 
                
                inputFile >> id1 >> id2;
                customer->setOperator(newOperator);
                break;
            case 8: 
                
                inputFile >> id >> amount;
                customer->getBill()->changeTheLimit(amount);
                break;
            default:
                break;
        }
    }

    // Calcular y escribir resultados en el archivo de salida

    // Calcular y escribir resultados en el archivo de salida

// Imprimir estadísticas de los operadores

cout << "imprime los operadores"<<endl;
for (auto& op : operators) {
    cout << "op = " << op << "\n";
    outputFile << "Operator " << op->getId() << ": " << op->getTotalSpentTalkingTime() << " "
               << op->getTotalMessageSent() << " " << op->getTotalInternetUsage() << "\n";
}

// Imprimir estadísticas de los clientes
for (auto& customer : customers) {
    outputFile << "Customer " << customer->getId() << ": " << customer->getBill()->getTotalMoneySpent() << " "
               << customer->getBill()->getCurrentDebt() << "\n";
}

// Encontrar y imprimir el cliente que más habla
Customer* topTalker = nullptr;
for (auto& customer : customers) {
    if (!topTalker || customer->getTotalSpentTalkingTime() > topTalker->getTotalSpentTalkingTime() ||
        (customer->getTotalSpentTalkingTime() == topTalker->getTotalSpentTalkingTime() && customer->getId() < topTalker->getId())) {
        topTalker = customer;
    }
}
if (topTalker) {
    outputFile << topTalker->getName() << ": " << topTalker->getTotalSpentTalkingTime() << "\n";
}

// Encontrar y imprimir el cliente que más mensajes envía
Customer* topMessenger = nullptr;
for (auto& customer : customers) {
    if (!topMessenger || customer->getTotalMessageSent() > topMessenger->getTotalMessageSent() ||
        (customer->getTotalMessageSent() == topMessenger->getTotalMessageSent() && customer->getId() < topMessenger->getId())) {
        topMessenger = customer;
    }
}
if (topMessenger) {
    outputFile << topMessenger->getName() << ": " << topMessenger->getTotalMessageSent() << "\n";
}

// Encontrar y imprimir el cliente que más se conecta a Internet
Customer* topInternetUser = nullptr;
for (auto& customer : customers) {
    if (!topInternetUser || customer->getTotalInternetUsage() > topInternetUser->getTotalInternetUsage() ||
        (customer->getTotalInternetUsage() == topInternetUser->getTotalInternetUsage() && customer->getId() < topInternetUser->getId())) {
        topInternetUser = customer;
    }
}
if (topInternetUser) {
    outputFile << topInternetUser->getName() << ": " << topInternetUser->getTotalInternetUsage() << "\n";
}


    // Liberar memoria de los apuntadores
    for (auto& customer : customers) {
        delete customer;
    }
    for (auto& op : operators) {
        delete op;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}
