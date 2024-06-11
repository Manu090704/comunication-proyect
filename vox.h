// =========================================================
// File: vox.h
// Author:
// Date:
// Description:
// =========================================================

#ifndef VOXOPERATOR_H
#define VOXOPERATOR_H

#include "operator.h"

class VoxOperator : public Operator {
public:
  VoxOperator(int, double, double, double, int, OperatorType);
  VoxOperator(const VoxOperator&);

  double calculateTalkingCost(int, int);
  double calculateMessageCost(int, int, int);
  double calculateNetworkCost(double);
};

VoxOperator::VoxOperator(int id, double talkingCharge, double messageCost, double networkCharge, int discountRate, OperatorType type)
    : Operator(id, talkingCharge, messageCost, networkCharge, discountRate, type) {}

VoxOperator::VoxOperator(const VoxOperator& x)
:Operator(x){}

double VoxOperator::calculateTalkingCost(int minute, int age){
    if(minute <= 0 || age <= 0){
        return 0;
    }

    double cost = minute * talkingCharge;

    if(age < 18 || age > 65){
        double discount = (cost * discountRate) / 100;
        return cost - discount;
    }

    return cost;
}


double VoxOperator:: calculateMessageCost(int quantity, int thisOpId, int otherOpId){
    if(quantity <= 0){
        return 0;
    }
    else if(thisOpId == otherOpId){
        double cost= quantity * messageCost;
        double final_cost = (cost*discountRate)/100;
        return cost - final_cost;
    }
    else{
        double cost= quantity * messageCost;
        return cost;
    }

}

double VoxOperator::calculateNetworkCost(double amount){
    if(amount <=0){
        return 0;
    }

    return amount * networkCharge;
}
#endif
