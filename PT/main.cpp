/*---------------- File: main.cpp  ---------------------+
|Modelo PLI - Problema do Transporte (PT)               |
+-------------------------------------------------------+ */

#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>

using namespace std;
ILOSTLBEGIN 

#define CPLEX_TIME_LIM 3600

int O; // Qtde de origens
int D; // Qtde de destinos
int M; // Qtde de arestas
vector<int> oferta;
vector<int> demanda;
vector<vector<int>> c; // Custo
vector<vector<bool>> adj; // Existe aresta?

void cplex(){
    IloEnv env;
    int i, j;
    int numberVar = 0;
    int numberRes = 0;

    IloArray<IloNumVarArray> x(env);
    for( i = 0; i < O; i++ ){
        x.add(IloNumVarArray(env));
        for( j = 0; j < D; j++ ){
            if(adj[i][j]){
                x[i].add(IloIntVar(env, 0, INT_MAX)); // Variaveis inteiras >= 0
                numberVar++;
            }else{
                x[i].add(IloIntVar(env, 0, 0)); 
            }
        }
    }

    IloModel model ( env );
    IloExpr sum(env);

    // FUNCAO OBJETIVO
    sum.clear();
    for( i = 0; i < O; i++ ){
        for( j = 0; j < D; j++ ){
            if(adj[i][j]){
                sum += (c[i][j] * x[i][j]);
            }
        }
    }
    model.add(IloMinimize(env, sum));

    // RESTRICOES
    // Oferta
    for( i = 0; i < O; i++ ){
        sum.clear();
        for( j = 0; j < D; j++ ){
            if(adj[i][j]) sum += x[i][j];
        }
        model.add(sum <= oferta[i]); 
        numberRes++;
    }

    // Demanda
    for( j = 0; j < D; j++ ){
        sum.clear();
        for( i = 0; i < O; i++ ){
            if(adj[i][j]) sum += x[i][j];
        }
        model.add(sum == demanda[j]); 
        numberRes++;
    }

    // EXECUCAO
    time_t timer, timer2;
    IloNum value, objValue;
    double runTime;
    string status;

    printf("--------Informacoes da Execucao:----------\n\n");
    printf("#Var: %d\n", numberVar);
    printf("#Restricoes: %d\n", numberRes);

    IloCplex cplex(model);
    cplex.setParam(IloCplex::TiLim, CPLEX_TIME_LIM);

    time(&timer);
    cplex.solve();
    time(&timer2);

    bool sol = true;
    switch(cplex.getStatus()){
        case IloAlgorithm::Optimal: status = "Optimal"; break;
        case IloAlgorithm::Feasible: status = "Feasible"; break;
        default: status = "No Solution"; sol = false;
    }

    cout << endl << "Status da FO: " << status << endl;

    if(sol){
        objValue = cplex.getObjValue();
        runTime = difftime(timer2, timer);
        cout << "Variaveis de decisao: " << endl;
        for( i = 0; i < O; i++ ){
            for( j = 0; j < D; j++ ){
                if(adj[i][j]){
                    value = IloRound(cplex.getValue(x[i][j]));
                    if(value > 0)
                        printf("x[%d][%d]: %.0lf\n", i+1, j+1, value); // Print 1-indexed
                }
            }
        }
        printf("\nFuncao Objetivo Valor = %.2lf\n", objValue);
        printf("..(%.6lf seconds).\n\n", runTime);
    }else{
        printf("No Solution!\n");
    }

    cplex.end();
    sum.end();
    env.end();
}

int main(){
    if(!(cin >> O >> D >> M)) return 0;
    
    oferta.assign(O, 0);
    for(int i = 0; i < O; i++) cin >> oferta[i];

    demanda.assign(D, 0);
    for(int j = 0; j < D; j++) cin >> demanda[j];

    c.assign(O, vector<int>(D, 0));
    adj.assign(O, vector<bool>(D, false));

    for(int i = 0; i < M; i++){
        int u_node, v_node, cost;
        cin >> u_node >> v_node >> cost;
        // Adjust for 0-indexed vectors
        u_node--; v_node--;
        adj[u_node][v_node] = true;
        c[u_node][v_node] = cost;
    }

    cplex();
    return 0;
}
