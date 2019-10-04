/**
 * @file DriverManager.cpp
 * @author Keeton Feavel (keetonfeavel@cedarville.edu)
 * @brief 
 * @version 0.1
 * @date 2019-09-26
 * 
 * @copyright Copyright Keeton Feavel (c) 2019
 * 
 */
#include <drivers/DriverManager.hpp>

DriverManager* DriverManager::activeDriverManager = nullptr;

DriverManager::DriverManager() {
    numberOfDrivers = 0;
    activeDriverManager = this;
}

void DriverManager::addDriver(Driver* driver) {
    drivers[numberOfDrivers] = driver;
    numberOfDrivers++;
}

void DriverManager::activateAll() {
    for (int i = 0; i < numberOfDrivers; i++) {
        drivers[i]->activate();
    }
}

void DriverManager::activateLast() {
    int index = numberOfDrivers - 1;
    drivers[index]->activate();
}

Driver* DriverManager::getDriverWithTag(char* tag) {
    // Cycle through the drivers which have been added
    for (int i = 0; i < numberOfDrivers; i++) {
        // If we find a matching tag
        if (strcmp(drivers[i]->getDriverTypeTag(), tag) == 0) {
            // Return the associated driver
            return drivers[i];
        }
    }
    // Nothing was found.
    return nullptr;
}