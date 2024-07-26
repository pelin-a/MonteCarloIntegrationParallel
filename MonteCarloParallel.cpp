#include <iostream>
#include <cstdlib> // For rand(), srand(), and std::rand()
#include <ctime>
#include <omp.h>
// determining the function to be used
double func(double x) {
    return x * x;
}

// parallelized function for the Monte Carlo Integration
double ParallelIntegration(int numberOfSamples, double a, double b) {

    double sum = 0.0; // initializing a sum 
    double range = b - a; // range is for the area we want to calculate, b-a represents the length

    // seeding the random number generator by using the current time, this ensures each time different numbers are generated
    int seed = (std::time(0));
    std::srand(seed);
    //omp_set_num_threads(5);
    
    double start = omp_get_wtime();

    // Use OpenMP to parallelize the loop
#pragma omp parallel
    {
        // Each thread needs its own local sum since they are executing independently
        double local_sum = 0.0;
        
#pragma omp single // I am using omp single to print the number of threads used, if I don't use this it print it multiple times because of multiple threads 
        {
            int nthreads = omp_get_num_threads();
            std::cout << "Number of threads: " << nthreads << std::endl;
        }


        // Parallel for loop to calculate the integral, each thread will execute this code
#pragma omp for

        for (int i = 0; i < numberOfSamples; i++) {

            int randomNum = std::rand(); // generates a random number
            double normalizedRandom = static_cast<double>(randomNum) / RAND_MAX; // static_cast converts the number to a double, dividing it by RAND_MAX ensures that it is between 0.0 and 1.0
            double scaledRandom = normalizedRandom * range; // here the random number is scaled for our needs, according to the range
            double x = a + scaledRandom; // by adding a, we shift it to the desired interval so that x is random in the selected range
            local_sum += func(x); // adds the function value at x to the local sum
        }

        // using a critical section so that only one thread at a time updates the sum value to prevent race condition
#pragma omp critical
        {
            sum += local_sum;
        }
    }

    // Average of the function values
    double integral = (range / numberOfSamples) * sum; // by dividing the number of samples we calculate the average width of subintervals, multiplying by the sum scales the sum of the values to approximate the integral
    return integral;
}

// Example function
int main() {
    int numProcs = omp_get_num_procs();// this is to show how manyprocessors i have
    //omp_set_num_threads(8);// setting it to the number of processors since it can't be more even if i provide a different value
    std::cout << "Number of processors available: " << numProcs << std::endl;
    double start = omp_get_wtime();
    int numSamples = 1000000; // Number of random samples
    double a = 0.0; // Start of interval
    double b = 1.0; // End of interval

    double result = ParallelIntegration(numSamples, a, b);
    double end = omp_get_wtime();
    std::cout << "Estimated integral: " << result << std::endl;
    std::cout << "Calculation time is: " << (end - start) << " seconds" << std::endl;
    return 0;
    // Estimated integral: 0.333604
    //Calculation time is : 0.0143881 seconds
}