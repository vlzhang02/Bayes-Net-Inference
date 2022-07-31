/*
 * main.cpp
 * by: Valerie Zhang
 *
 * Purpose: Run program for inference in a Bayes Net
 *
 */
#include "CPT.h"
#include "Node.h"
#include "BN.h"
#include "Inference.h"
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./BayesNet infoFile \n";
        exit(EXIT_FAILURE);
    }
    Inference i(argv[1]);
    i.run();
    return 0;
}