#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

struct Car {
    std::string model;
    double currentSpeed;
    double brakePressure;
    double tirePressure;
    std::string abs;
    std::string ebd;
    std::string esc;
    int airbags;
    double airbagDeploymentForce;
    double frictionCoefficient;
    double stoppingDistance;
    double maxImpact;
};

struct TestCase {
    std::string model;
    int expectedSafetyScore;
};

double calculateSafetyScore(const Car &car, double speed, double frictionCoefficient) {
    double score = 100 - (speed / 10) - (car.maxImpact / 10) + (car.airbags * 2) + (frictionCoefficient * 10);
    if (car.abs == "Yes") score += 5;
    if (car.ebd == "Yes") score += 3;
    if (car.esc == "Yes") score += 4;
    return std::round(score);
}

void loadCarData(const std::string &filename, std::vector<Car> &cars) {
    std::ifstream file(filename);
    std::string line;
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Car car;
        std::getline(ss, car.model, ',');
        ss >> car.currentSpeed; ss.ignore();
        ss >> car.brakePressure; ss.ignore();
        ss >> car.tirePressure; ss.ignore();
        std::getline(ss, car.abs, ',');
        std::getline(ss, car.ebd, ',');
        std::getline(ss, car.esc, ',');
        ss >> car.airbags; ss.ignore();
        ss >> car.airbagDeploymentForce; ss.ignore();
        ss >> car.frictionCoefficient; ss.ignore();
        ss >> car.stoppingDistance; ss.ignore();
        ss >> car.maxImpact;
        cars.push_back(car);
    }
}

void loadTestCases(const std::string &filename, std::vector<TestCase> &testCases) {
    std::ifstream file(filename);
    std::string line;
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        TestCase testCase;
        std::getline(ss, testCase.model, ',');
        ss >> testCase.expectedSafetyScore;
        testCases.push_back(testCase);
    }
}

double calculateMomentum(double mass, double relativeSpeed) {
    return mass * relativeSpeed;
}

double calculateCollisionIntensity(double carMass, double speed, double stoppingDistance) {
    return (carMass * std::pow(speed, 2)) / (2 * stoppingDistance);
}

int main() {
    std::vector<Car> cars;
    std::vector<TestCase> testCases;

    loadCarData("CarData.csv", cars);
    loadTestCases("TestCases.csv", testCases);

    // Flag to check if all test cases pass
    bool allTestsPassed = true;

    // Display test case results
    std::cout << "\nTest Case Results:\n";
    for (const auto &testCase : testCases) {
        bool testPassed = false;
        for (const auto &car : cars) {
            if (car.model == testCase.model) {
                double safetyScore = calculateSafetyScore(car, car.currentSpeed, car.frictionCoefficient);
                std::cout << "Model: " << car.model << "\n"
                          << "Expected Safety Score: " << testCase.expectedSafetyScore << "\n"
                          << "Calculated Safety Score: " << safetyScore << "\n";

                if (safetyScore == testCase.expectedSafetyScore) {
                    std::cout << "Passed!" << std::endl;
                    testPassed = true;
                } else {
                    std::cout << "Failed!" << std::endl;
                    allTestsPassed = false;  // Mark the overall test result as failed
                }
            }
        }

        // If no matching car model is found in the car data
        if (!testPassed) {
            std::cout << "Test case for model " << testCase.model << " did not pass.\n";
        }
    }

    if (allTestsPassed) {
        std::cout << "\nAll test cases passed!\n";
    } else {
        std::cout << "\nSome test cases failed.\n";
    }

    // Dynamic inputs for CarData
    while (true) {
        std::cout << "\nSelect a car by number:\n";
        for (size_t i = 0; i < cars.size(); ++i) {
            std::cout << i + 1 << ". " << cars[i].model << "\n";
        }
        std::cout << "Enter your choice: ";
        int carChoice;
        std::cin >> carChoice;

        if (carChoice < 1 || carChoice > static_cast<int>(cars.size())) {
            std::cout << "Invalid choice. Exiting program.\n";
            break;
        }

        Car &car = cars[carChoice - 1];

        std::cout << "\nSelect weather condition for " << car.model << ":\n";
        std::cout << "1. Clear\n2. Rainy\n3. Snowy\n";
        int weatherChoice;
        std::cin >> weatherChoice;
        std::string weatherCondition;
        switch (weatherChoice) {
            case 1: weatherCondition = "Clear"; break;
            case 2: weatherCondition = "Rainy"; break;
            case 3: weatherCondition = "Snowy"; break;
            default: weatherCondition = "Clear"; break;
        }

        // Adjust friction based on weather condition
        if (weatherCondition == "Clear") {
            car.frictionCoefficient = car.frictionCoefficient; // No change for clear weather
        } else if (weatherCondition == "Rainy") {
            car.frictionCoefficient *= 0.7;
        } else if (weatherCondition == "Snowy") {
            car.frictionCoefficient *= 0.5;
        }

        // Input obstacle mass
        double obstacleMass;
        std::cout << "Enter obstacle mass (in kg) for " << car.model << ": ";
        std::cin >> obstacleMass;

        // Assume car mass as 1500 kg
        double carMass = 1500;
        double collisionIntensity = calculateCollisionIntensity(carMass, car.currentSpeed, car.stoppingDistance);
        std::cout << "\nModel: " << car.model << "\n";
        std::cout << "Collision intensity if obstacle is stationary: " << collisionIntensity << " N\n";

        // Get obstacle distance and whether it's stationary
        double obstacleDistance;
        std::cout << "Enter obstacle distance (in meters) for " << car.model << ": ";
        std::cin >> obstacleDistance;

        char stationaryInput;
        std::cout << "Is the obstacle stationary (y/n)? ";
        std::cin >> stationaryInput;
        bool isStationary = (stationaryInput == 'y' || stationaryInput == 'Y');

        double collisionIntensityDynamic;
        if (isStationary) {
            collisionIntensityDynamic = calculateCollisionIntensity(carMass, car.currentSpeed, obstacleDistance);
            std::cout << "Collision intensity for stationary obstacle: " << collisionIntensityDynamic << " N\n";
        } else {
            collisionIntensityDynamic = calculateCollisionIntensity(obstacleMass, car.currentSpeed, obstacleDistance);
            std::cout << "Collision intensity for dynamic obstacle: " << collisionIntensityDynamic << " N\n";
        }

        // Get updated speed from the user
        double updatedSpeed;
        std::cout << "Enter updated speed for " << car.model << ": ";
        std::cin >> updatedSpeed;

        double currentSafetyScore = calculateSafetyScore(car, car.currentSpeed, car.frictionCoefficient);
        double updatedSafetyScore = calculateSafetyScore(car, updatedSpeed, car.frictionCoefficient);

        std::cout << "\n" << car.model << "\n";
        std::cout << "Current Speed: " << car.currentSpeed << " kmph\n";
        std::cout << "Brake Pressure: " << car.brakePressure << " Psi\n";
        std::cout << "Tire Pressure: " << car.tirePressure << " Psi\n";
        std::cout << "ABS: " << car.abs << "\n";
        std::cout << "EBD: " << car.ebd << "\n";
        std::cout << "ESC: " << car.esc << "\n";
        std::cout << "Number of Airbags: " << car.airbags << "\n";
        std::cout << "Airbag Deployment Force: " << car.airbagDeploymentForce << " N\n";
        std::cout << "Friction Coefficient: " << car.frictionCoefficient << "\n";
        std::cout << "Stopping Distance: " << car.stoppingDistance << " m\n";
        std::cout << "Maximum Impact: " << car.maxImpact << " N\n";
        std::cout << "Current Safety Score: " << currentSafetyScore << "\n";
        std::cout << "Updated Safety Score after speed change: " << updatedSafetyScore << "\n";

        char continueInput;
        std::cout << "Do you want to continue (y/n)? ";
        std::cin >> continueInput;
        if (continueInput != 'y' && continueInput != 'Y') {
            break;
        }
    }

    return 0;
}
