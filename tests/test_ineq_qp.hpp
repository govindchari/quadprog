#pragma once
#include "catch.hpp"
#include "QPSolver.hpp"
#include "epigraph.hpp"
#include <iostream>

using namespace cvx;

TEST_CASE("Inequality Constrained QP")
{
    int n = 10;
    Eigen::MatrixXd A;
    Eigen::MatrixXd Q(n, n);
    Eigen::MatrixXd G(2 * n, n);
    Eigen::VectorXd q(n);
    Eigen::VectorXd b;
    Eigen::VectorXd h(2 * n);

    Eigen::VectorXd temp(n);
    temp.setConstant(1);

    Q = Eigen::MatrixXd::Identity(n, n);

    q.setConstant(1);
    h.setZero(2 * n, 1);
    h(Eigen::seq(0, n - 1)) = temp;

    G(Eigen::seq(0, n - 1), Eigen::seq(0, n - 1)) = Eigen::MatrixXd::Identity(n, n);
    G(Eigen::seq(n, 2 * n - 1), Eigen::seq(0, n - 1)) = -Eigen::MatrixXd::Identity(n, n);

    QP qp(Q, q, A, b, G, h);
    qp.solve(false);

    OptimizationProblem qp_epi;
    VectorX x = qp_epi.addVariable("x", n);

    for (int i = 0; i < G.rows(); i++)
    {
        auto Gi = G.row(i);
        qp_epi.addConstraint(lessThan(par(Gi).dot(x), par(h(i))));
    }

    qp_epi.addCostTerm(x.transpose() * par(Q) * x + par(q).dot(x));
    osqp::OSQPSolver solver(qp_epi);
    solver.solve(false);

    Eigen::VectorXd QPSolver_sol = qp.solution.x;
    Eigen::VectorXd OSQP_sol = eval(x);

    double tol = 1e-3;
    
    REQUIRE((QPSolver_sol-OSQP_sol).norm() < tol);
}