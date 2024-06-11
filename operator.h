// =========================================================
// File: operator.h
// Author:
// Date:
// Description:
// =========================================================

#ifndef OPERATOR_H
#define OPERATOR_H

#include <string>
#include <sstream>
#include <iomanip>

typedef enum{VOX, INTERNET} OperatorType;

class Operator {
protected:
  int id, discountRate;
  double talkingCharge, messageCost, networkCharge, totalInternetUsage;
  int totalSpentTalkingTime, totalMessageSent;
  OperatorType type;

public:
  Operator(int, double, double, double, int, OperatorType);
  Operator(const Operator&);

  int getId();
  int getDiscountRate();
  double getTalkingChage();
  double getMessageCost();
  double getNetworkCharge();
  int getTotalSpentTalkingTime();
  int getTotalMessageSent();
  double getTotalInternetUsage();
  OperatorType getType();

  void addTalkingTime(int);
  void addTotalMessageSent(int);
  void addTotalInternetUsage(double);

  std::string toString() const;

  virtual double calculateTalkingCost(int, int) = 0;
  virtual double calculateMessageCost(int, int, int) = 0;
  virtual double calculateNetworkCost(double) = 0;
};

// Constructor con parámetros
Operator::Operator(int id_,double talkingCharge_, double messageCost_, double networkCharge_,int discountRate_,OperatorType type_) {
    id = id_;
    discountRate = discountRate_;
    talkingCharge = talkingCharge_;
    messageCost = messageCost_;
    networkCharge = networkCharge_;
    totalInternetUsage = 0;
    totalSpentTalkingTime = 0;
    totalMessageSent = 0;
    type = type_;
}

// Constructor de copia
Operator::Operator(const Operator& other) {
    id = other.id;
    discountRate = other.discountRate;
    talkingCharge = other.talkingCharge;
    messageCost = other.messageCost;
    networkCharge = other.networkCharge;
    totalInternetUsage = other.totalInternetUsage;
    totalSpentTalkingTime = other.totalSpentTalkingTime;
    totalMessageSent = other.totalMessageSent;
    type = other.type;
}

int Operator::getId() {
    return id;
}

int Operator::getDiscountRate(){
    return discountRate;
}

double Operator::getTalkingChage() {
    return talkingCharge;
}

double Operator::getMessageCost(){
    return messageCost;
}

double Operator::getNetworkCharge(){
    return networkCharge;
}

int Operator::getTotalSpentTalkingTime(){
    return totalSpentTalkingTime;
}

int Operator::getTotalMessageSent(){
    return totalMessageSent;
}

double Operator::getTotalInternetUsage(){
    return totalInternetUsage;
}

OperatorType Operator::getType(){
    return type;
}

void Operator::addTalkingTime(int minute){
  if (minute > 0){
    totalSpentTalkingTime += minute;
  }
}

void Operator:: addTotalMessageSent(int quantity){
  if (quantity > 0){
    totalMessageSent += quantity;
  }

}
void Operator:: addTotalInternetUsage(double amount){
  if (amount > 0){
    totalInternetUsage += amount;
  }
}

std::string Operator::toString() const {
    std::ostringstream oss;
    oss << "Operator #" << id << " : " << totalSpentTalkingTime << " " << totalMessageSent << " " << std::fixed << std::setprecision(2) << totalInternetUsage;
    return oss.str();
}

#endif
