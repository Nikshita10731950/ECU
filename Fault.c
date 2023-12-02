#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Define signals for faults
#define TEMP_FAULT_SIGNAL SIGUSR1
#define SPEED_FAULT_SIGNAL SIGUSR2
#define FUEL_FAULT_SIGNAL SIGINT

// Signal handlers
void temperatureFaultHandler(int signo) {
    printf("Temperature is too high. Fault detected!\n");
    exit(EXIT_FAILURE);
}

void speedFaultHandler(int signo) {
    printf("High speed detected. Fault detected!\n");
    exit(EXIT_FAILURE);
}

void fuelLeakageHandler(int signo) {
    printf("Fuel leakage detected. Fault detected!\n");
    exit(EXIT_FAILURE);
}

int main() {
    // Register signal handlers
    signal(TEMP_FAULT_SIGNAL, temperatureFaultHandler);
    signal(SPEED_FAULT_SIGNAL, speedFaultHandler);
    signal(FUEL_FAULT_SIGNAL, fuelLeakageHandler);

    // Simulated sensor values
    float temperature = 105.0;
    float speed = 130.0;
    int fuelLeakage = 1;  // 1 represents leakage, 0 represents no leakage

    // Simulate faults by sending signals
    if (temperature > 100.0) {
        kill(getpid(), TEMP_FAULT_SIGNAL);
    }

    if (speed > 120.0) {
        kill(getpid(), SPEED_FAULT_SIGNAL);
    }

    if (fuelLeakage) {
        kill(getpid(), FUEL_FAULT_SIGNAL);
    }

    // Continue with normal program execution if no faults are detected
    printf("No faults detected. Continuing normal execution.\n");

    return 0;
}
