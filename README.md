# QPSolver

![Unit Tests](https://github.com/govindchari/QPSolver/actions/workflows/unit_tests.yml/badge.svg)


QPSolver is a quadratic program solver implemented in C++ based on the [cvxgen](https://stanford.edu/~boyd/papers/pdf/code_gen_impl.pdf) paper. This solver solves problems of the following form

<img src="https://user-images.githubusercontent.com/6196536/149648352-649501d1-2ece-4ef0-aacb-bc373be3d566.png" alt="img" width="200"/>

## Features

This solver implements the following
  * Mehrotra's predictor-corrector
  * Permuted LDLT decomposition
  * KKT system regularization
  * Iterative refinement

Debug:
  - Add #include <string.h> to top of QPSolver/tests/Epigraph/solvers/ecos/ecos_bb/ecos_bb.c

## Installation Guide

### Example
```cpp
#include <iostream>
#include "QPSolver.hpp"

int main()
{
    // Setup problem data
    int n = 10;         //Number of assets
    double gamma = 0.5; //Risk aversion parameter
    Eigen::MatrixXd A(1, n);
    Eigen::MatrixXd Q(n, n); //Return Covariance
    Eigen::MatrixXd G(n, n);
    Eigen::VectorXd q(n); //Expected Return
    Eigen::VectorXd b(1);
    Eigen::VectorXd h(n);

    //Generate random return covariance matrix (PSD matrix)
    Q.setRandom();
    Q = Q.transpose() * Q;
    Q += n * Eigen::MatrixXd::Identity(n, n);
    Q *= gamma;

    //Generate random expected return
    q.setRandom();

    //No shorting (non-negativity constraint)
    G = -Eigen::MatrixXd::Identity(n, n);
    h.setZero(n);

    A.setConstant(1);
    b << 1;

    //Set up QPSolver solver
    QP qp(Q, q, A, b, G, h);
    bool verbose = true;

    //Solve QP
    qp.solve(verbose);

    //Print optimal portfolio
    std::cout << "x_opt:" << std::endl;
    std::cout << qp.solution.x << std::endl;
}

```
Examples for equality only, inequality only, and unconstrained probelems can be found in [tests](tests)
