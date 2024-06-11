// =========================================================
// File: vox.h
// Author:
// Date:
// Description:
// =========================================================

#ifndef InternetOperator_H
#define InternetOperator_H

#include "operator.h"

class InternetOperator : public Operator {
private:
  const double LIMITGB = 1.0;

public:
  InternetOperator(int, double, double, double, int, OperatorType);
  InternetOperator(const InternetOperator&);

  double calculateTalkingCost(int, int);
  double calculateMessageCost(int, int, int);
  double calculateNetworkCost(double);
};

InternetOperator::InternetOperator(int id, double talkingCharge, double messageCost, double networkCharge, int discountRate, OperatorType type)
    : Operator(id, talkingCharge, messageCost, networkCharge, discountRate, type) {}

InternetOperator::InternetOperator(const InternetOperator& x)
:Operator(x){}

double InternetOperator::calculateTalkingCost(int minute, int age){
    if(age <= 0 || minute <= 0){
        return 0;
    }
    double cost = minute * talkingCharge;

    if(minute < 2){
        double discount=(cost*discountRate)/100;
        double final_cost = cost-discount;
        return final_cost;
    }
    return cost;
}

double InternetOperator::calculateMessageCost(int quantity, int thisOpId, int otherOpId){
        if (quantity <= 0) {
        return 0.0;
    }

    // Calcula el costo base de enviar los mensajes
    double cost = quantity * messageCost;

    // Aplica el descuento si la cantidad de mensajes es menor a 3
    if (quantity < 3) {
        double discount = (cost * discountRate) / 100;
        cost -= discount;
    }

    return cost;
}

double InternetOperator::calculateNetworkCost(double amount) {
    // Si la cantidad de GB es menor o igual a 0, regresa 0
    if (amount <= 0) {
        return 0.0;
    }

    // Calcula la cantidad de GB que excede el l�mite de 1 GB
    double excessAmount = amount - 1.0;

    // Si la cantidad de GB no excede el l�mite de 1 GB, no hay costo
    if (excessAmount <= 0) {
        return 0.0;
    }

    // Calcula el costo para la cantidad de GB que excede el l�mite
    double cost = excessAmount * networkCharge;

    return cost;
}


#endif
