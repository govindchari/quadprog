#pragma once
#include "Eigen/Dense"
#include "Eigen/Sparse"
#include "utilities.hpp"
class QP
{
private:
    //Variable lengths
    unsigned int nx;
    unsigned int ns;
    unsigned int nz;
    unsigned int ny;
    unsigned int N;

    //Problem Data
    Eigen::MatrixXd Q;
    Eigen::VectorXd q;
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
    Eigen::MatrixXd G;
    Eigen::VectorXd h;

    //Optimization variables
    Eigen::VectorXd x;
    Eigen::VectorXd s;
    Eigen::VectorXd z;
    Eigen::VectorXd y;

    //KKT System
    Eigen::MatrixXd KKT;
    Eigen::SparseMatrix<double> KKT_sparse;
    Eigen::VectorXd rhs_a;
    Eigen::VectorXd rhs_c;
    Eigen::VectorXd p_a;
    Eigen::VectorXd p_c;

    DELTA delta;

    void rhs_kkt_a();
    void index_sol_a();
    void rhs_kkt_c(double sig, double mu);
    void index_sol_c();
    double linesearch(Eigen::VectorXd x,Eigen::VectorXd dx);
    void centering_params(double &sig, double &mu);
    void combine_deltas();
    void update_vars(double a);
    void initialize_kkt();
    void update_kkt();
    void logging(int iter, double a);
    void initialize();

public:
    SOLUTION solution;
    TOLERANCE tol;

    QP(Eigen::MatrixXd Qi, Eigen::VectorXd qi, Eigen::MatrixXd Ai, Eigen::VectorXd bi, Eigen::MatrixXd Gi, Eigen::VectorXd hi);

    void solve();
    void solve(bool verbose);
};
