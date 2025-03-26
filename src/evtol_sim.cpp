// imports
#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <string>
#include <memory>
#include <numeric>
#include <cassert>

using namespace std;

// Base class for vehicle characteristics
class VehicleType {
protected:
    string name;
    double cruiseSpeed;        // mph
    double batteryCapacity;    // kWh
    double chargeTime;        // hours
    double energyUse;         // kWh/mile
    int passengerCount;
    double faultProbability;  // per hour

public:
    VehicleType(string n, double speed, double battery, double charge, 
                double energy, int passengers, double faultProb) 
        : name(n), cruiseSpeed(speed), batteryCapacity(battery), 
          chargeTime(charge), energyUse(energy), passengerCount(passengers),
          faultProbability(faultProb) {}
    
    virtual ~VehicleType() = default;
    
    // Getters
    double getRange() const { return batteryCapacity / energyUse; }
    double getCruiseSpeed() const { return cruiseSpeed; }
    double getChargeTime() const { return chargeTime; }
    double getEnergyUse() const { return energyUse; }
    int getPassengerCount() const { return passengerCount; }
    double getFaultProbability() const { return faultProbability; }
    string getName() const { return name; }
};

// Individual vehicle instance
class Vehicle {
private:
    shared_ptr<VehicleType> type;
    double currentBattery;
    double flightTimeTotal;
    double distanceTotal;
    double chargeTimeTotal;
    int flightCount;
    int chargeCount;
    int faultCount;
    double passengerMiles;

public:
    Vehicle(shared_ptr<VehicleType> t) 
        : type(t), currentBattery(t->getRange() * t->getEnergyUse()), 
          flightTimeTotal(0), distanceTotal(0), chargeTimeTotal(0),
          flightCount(0), chargeCount(0), faultCount(0), passengerMiles(0) {}

    bool simulateFlight(double simTime, mt19937& gen) {
        double flightTime = min(simTime, getRemainingFlightTime());
        double distance = flightTime * type->getCruiseSpeed();
        
        // Check for faults
        uniform_real_distribution<> dis(0, 1);
        int hours = static_cast<int>(flightTime);
        for (int i = 0; i < hours; i++) {
            if (dis(gen) < type->getFaultProbability()) {
                faultCount++;
            }
        }

        flightTimeTotal += flightTime;
        distanceTotal += distance;
        passengerMiles += distance * type->getPassengerCount();
        flightCount++;
        currentBattery -= distance * type->getEnergyUse();
        
        return currentBattery > 0;
    }

    void charge(double time) {
        chargeTimeTotal += time;
        chargeCount++;
        currentBattery = type->getRange() * type->getEnergyUse();
    }

    // Statistics getters
    double getAvgFlightTime() const { return flightCount ? flightTimeTotal/flightCount : 0; }
    double getAvgDistance() const { return flightCount ? distanceTotal/flightCount : 0; }
    double getAvgChargeTime() const { return chargeCount ? chargeTimeTotal/chargeCount : 0; }
    int getFaultCount() const { return faultCount; }
    double getPassengerMiles() const { return passengerMiles; }
    shared_ptr<VehicleType> getVehicleType() const { return type; }
    
private:
    double getRemainingFlightTime() const {
        return currentBattery / (type->getCruiseSpeed() * type->getEnergyUse());
    }
};

// Simulation manager
class Simulator {
private:
    vector<shared_ptr<VehicleType>> vehicleTypes;
    vector<shared_ptr<Vehicle>> vehicles;
    queue<shared_ptr<Vehicle>> chargeQueue;
    vector<shared_ptr<Vehicle>> chargers;
    const int TOTAL_VEHICLES = 20;
    const int TOTAL_CHARGERS = 3;
    const double SIM_TIME = 3.0;  // hours

public:
    Simulator() {
        // Initialize vehicle types
        vehicleTypes.push_back(make_shared<VehicleType>("Alpha", 120, 320, 0.6, 1.6, 4, 0.25));
        vehicleTypes.push_back(make_shared<VehicleType>("Bravo", 100, 100, 0.2, 1.5, 5, 0.10));
        vehicleTypes.push_back(make_shared<VehicleType>("Charlie", 160, 220, 0.8, 2.2, 3, 0.05));
        vehicleTypes.push_back(make_shared<VehicleType>("Delta", 90, 120, 0.62, 0.8, 2, 0.22));
        vehicleTypes.push_back(make_shared<VehicleType>("Echo", 30, 150, 0.3, 5.8, 2, 0.61));
        
        chargers.resize(TOTAL_CHARGERS, nullptr);
    }

    void initializeVehicles(mt19937& gen) {
        discrete_distribution<> dist{4, 4, 4, 4, 4}; // Equal probability
        for (int i = 0; i < TOTAL_VEHICLES; i++) {
            int typeIdx = dist(gen);
            vehicles.push_back(make_shared<Vehicle>(vehicleTypes[typeIdx]));
        }
    }

    void runSimulation() {
        mt19937 gen(random_device{}());
        initializeVehicles(gen);
        
        double currentTime = 0;
        while (currentTime < SIM_TIME) {
            // Process flying vehicles
            for (auto& vehicle : vehicles) {
                if (!vehicle->simulateFlight(SIM_TIME - currentTime, gen)) {
                    chargeQueue.push(vehicle);
                }
            }

            // Process chargers
            for (int i = 0; i < TOTAL_CHARGERS; i++) {
                if (chargers[i] && currentTime >= chargers[i]->getAvgChargeTime()) {
                    chargers[i] = nullptr;
                }
                if (!chargers[i] && !chargeQueue.empty()) {
                    chargers[i] = chargeQueue.front();
                    chargers[i]->charge(vehicleTypes[i]->getChargeTime());
                    chargeQueue.pop();
                }
            }
            currentTime += 0.1; // Time step
        }
    }

    void printStatistics() {
        vector<vector<shared_ptr<Vehicle>>> groupedVehicles(5);
        for (auto& v : vehicles) {
            for (int i = 0; i < 5; i++) {
                if (v->getAvgFlightTime() && vehicleTypes[i]->getName() == v->getVehicleType()->getName()) {
                    groupedVehicles[i].push_back(v);
                    break;
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            if (groupedVehicles[i].empty()) continue;
            cout << "\n" << vehicleTypes[i]->getName() << " (" << groupedVehicles[i].size() << " vehicles):\n";
            double avgFlight = accumulate(groupedVehicles[i].begin(), groupedVehicles[i].end(), 0.0,
                [](double sum, shared_ptr<Vehicle> v) { return sum + v->getAvgFlightTime(); }) / groupedVehicles[i].size();
            double avgDist = accumulate(groupedVehicles[i].begin(), groupedVehicles[i].end(), 0.0,
                [](double sum, shared_ptr<Vehicle> v) { return sum + v->getAvgDistance(); }) / groupedVehicles[i].size();
            double avgCharge = accumulate(groupedVehicles[i].begin(), groupedVehicles[i].end(), 0.0,
                [](double sum, shared_ptr<Vehicle> v) { return sum + v->getAvgChargeTime(); }) / groupedVehicles[i].size();
            int totalFaults = accumulate(groupedVehicles[i].begin(), groupedVehicles[i].end(), 0,
                [](int sum, shared_ptr<Vehicle> v) { return sum + v->getFaultCount(); });
            double totalPassMiles = accumulate(groupedVehicles[i].begin(), groupedVehicles[i].end(), 0.0,
                [](double sum, shared_ptr<Vehicle> v) { return sum + v->getPassengerMiles(); });

            cout << "Avg Flight Time: " << avgFlight << " hours\n";
            cout << "Avg Distance: " << avgDist << " miles\n";
            cout << "Avg Charge Time: " << avgCharge << " hours\n";
            cout << "Total Faults: " << totalFaults << "\n";
            cout << "Total Passenger Miles: " << totalPassMiles << "\n";
        }
    }
};

// Basic unit test examples
void runUnitTests() {
    auto alpha = make_shared<VehicleType>("Alpha", 120, 320, 0.6, 1.6, 4, 0.25);
    assert(abs(alpha->getRange() - 200.0) < 0.001 && "Range calculation failed");

    Vehicle v(alpha);
    assert(v.getAvgFlightTime() == 0 && "Initial flight time should be 0");

    cout << "Basic unit tests passed\n";
}

int main() {
    runUnitTests();
    Simulator sim;
    sim.runSimulation();
    sim.printStatistics();
    return 0;
}