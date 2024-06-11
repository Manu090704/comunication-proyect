// =========================================================
// File: customer.h
// Author:
// Date:
// Description:
// =========================================================
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream> // Necesario para std::cout
#include "bill.h"
#include "operator.h"

class Customer {
private:
  int id, age, totalSpentTalkingTime, totalMessageSent;
  double totalInternetUsage;
  std::string name;
  Operator *op;
  Bill *bill;

public:
  Customer(int id_, const std::string &name_, int age_, Operator *op_, double creditLimit);
  Customer(const Customer&);
  ~Customer();

  int getId();
  int getAge();
  int getTotalSpentTalkingTime();
  int getTotalMessageSent();
  double getTotalInternetUsage();
  std::string getName();
  Operator* getOperator();
  Bill* getBill();

  void setOperator(Operator*);

  std::string toString() const;

  void talk(int, Customer&);
  void message(int, Customer&);
  void connection(double);
  void pay(double);
};

Customer::Customer(int id_, const std::string &name_, int age_, Operator *op_, double creditLimit)
    : id(id_), name(name_), age(age_), op(op_), bill(new Bill(creditLimit)),
      totalSpentTalkingTime(0), totalMessageSent(0), totalInternetUsage(0.0) {}

Customer::Customer(const Customer& x)
    : id(x.id), age(x.age), totalSpentTalkingTime(x.totalSpentTalkingTime),
      totalMessageSent(x.totalMessageSent), totalInternetUsage(x.totalInternetUsage),
      name(x.name), op(x.op), bill(new Bill(*x.bill)) {}

Customer::~Customer() {
    delete bill;
}

int Customer::getId(){
    return id;
}

int Customer::getAge(){
    return age;
}

int Customer::getTotalSpentTalkingTime(){
    return totalSpentTalkingTime;
}

int Customer::getTotalMessageSent(){
    return totalMessageSent;
}

double Customer::getTotalInternetUsage(){
    return totalInternetUsage;
}

std::string Customer::getName(){
    return name;
}

Operator* Customer::getOperator(){
    return op;
}

Bill* Customer::getBill(){
    return bill;
}

void Customer::setOperator(Operator* newOp) {
    op = newOp;
}

void Customer::talk(int minutes, Customer &other) {
    if (minutes > 0 && &other != this) {
        double callCost = minutes /10 * (op->calculateTalkingCost(minutes, age));
        if (bill->getLimitAmount() >= callCost) {
            bill->add(callCost);
            totalSpentTalkingTime += minutes;
            op->addTalkingTime(minutes);

            if (other.getOperator() != op) {
                other.getOperator()->addTalkingTime(minutes);
            }
        } else {
            std::cout << "No hay suficiente límite de crédito disponible en la factura." << std::endl;
        }
    } else {
        std::cout << "La cantidad de minutos debe ser mayor a 0 y el cliente no puede llamarse a sí mismo." << std::endl;
    }
}

void Customer::message(int quantity, Customer &other) {
    if (quantity > 0 && &other != this) {
        double messageCost = op->calculateMessageCost(quantity, op->getId(), other.getOperator()->getId());

        if (bill->getLimitAmount() >= messageCost) {
            bill->add(messageCost);
            totalMessageSent += quantity;
            op->addTotalMessageSent(quantity);

            if (other.getOperator() != op) {
                other.getOperator()->addTotalMessageSent(quantity);
            }
        } else {
            std::cout << "No hay suficiente l�mite de cr�dito disponible en la factura." << std::endl;
        }
    } else {
        std::cout << "La cantidad de mensajes debe ser mayor a 0 y el cliente no puede enviarse mensajes a s� mismo." << std::endl;
    }
}

void Customer::connection(double amount) {
    if (amount > 0) {
        double internetCost = amount * (op->calculateNetworkCost(amount));

        if (bill->getLimitAmount() >= internetCost) {
            bill->add(internetCost);
            totalInternetUsage += amount;
            op->addTotalInternetUsage(amount);
        } else {
            std::cout << "No hay suficiente l�mite de cr�dito disponible en la factura." << std::endl;
        }
    } else {
        std::cout << "La cantidad de GB utilizados debe ser mayor a 0." << std::endl;
    }
}

#endif CUSTOMER_H
