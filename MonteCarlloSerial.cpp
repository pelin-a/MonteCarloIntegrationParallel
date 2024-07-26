#include <iostream>
#include <cstdlib> // For rand(), srand(), and std::rand()
#include <ctime>
#include <omp.h>

// determining the function to be used
double function(double x) {
	return x * x;
}

// function for the Monte Carlo Integration
double Integration(int numberOfSamples, double a, double b) {

	double sum = 0.0; // initializing a sum 
	double range = b - a;//range is for the area we want to calculate, b-a represents the length
	std::srand((std::time(0)));// seeding the random number generator by using the current time, this ensures each yime different numbers are generated
	//the for loop runs numberOfSamples times. and calculates an x value for every random sample, then takes the sum of all.
	for (int i = 0; i < numberOfSamples; i++) {

		int randomNum = std::rand(); // generates a random number
		double normalizedRandom = static_cast<double>(randomNum) / RAND_MAX; // static_cast converts the number to a double, dividing it by RAND_MAX ensures that it is between 0.0 nd 1.0 which is called 
		//normalization provides a uniform distribution and makes it easier fit a desired range.
		double scaledRandom = normalizedRandom * range;// here the random number is scaled for our needs, according to the range
		double x = a + scaledRandom;// by adding a, we shift it to the desired interval so that x is random in the selected range
		sum += function(x); // adds the function value at x to the sum

	}

	//Average of the function values
	double integral = (range / numberOfSamples) * sum; // by dividing the number of samples we calculate the aavrage width of subintervals, multiplying by the sum scales the sum of the values to approximate the integral
	return integral;

}
//example function
int main() {
	double start = omp_get_wtime();
	int numSamples = 1000000; // Number of random samples
	double a = 0.0; // Start of interval
	double b = 1.0; // End of interval

	double result = Integration(numSamples, a, b);
	double end = omp_get_wtime();
	std::cout << "Estimated integral: " << result << std::endl;
	std::cout << "Calculation time is : " << (end - start);
	return 0;
	//output:Estimated integral: 0.333243
	//Calculation time is : 0.0390864
}