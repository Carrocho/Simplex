/*---------------- File: main.cpp  ---------------------+
|Modelo PLI - Problema da Designacao (PD)               |
+-------------------------------------------------------+ */

#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>

using namespace std;
ILOSTLBEGIN 

#define CPLEX_TIME_LIM 3600

int N; // Qtde de agentes/tarefas
int M; // Qtde de arestas (designacoes possiveis)
vector<vector<int>> c; // Custo
vector<vector<bool>> adj; // Existe aresta?

void cplex(){
    IloEnv env;
    int i, j;
    int numberVar = 0;
    int numberRes = 0;

    IloArray<IloNumVarArray> x(env);
    for( i = 0; i < N; i++ ){
        x.add(IloNumVarArray(env));
        for( j = 0; j < N; j++ ){
            if(adj[i][j]){
                x[i].add(IloIntVar(env, 0, 1)); // Variaveis binarias
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
    for( i = 0; i < N; i++ ){
        for( j = 0; j < N; j++ ){
            if(adj[i][j]){
                sum += (c[i][j] * x[i][j]);
            }
        }
    }
    model.add(IloMinimize(env, sum));

    // RESTRICOES
    // Cada agente eh designado para exatamente 1 tarefa
    for( i = 0; i < N; i++ ){
        sum.clear();
        for( j = 0; j < N; j++ ){
            if(adj[i][j]) sum += x[i][j];
        }
        model.add(sum == 1); 
        numberRes++;
    }

    // Cada tarefa eh executada por exatamente 1 agente
    for( j = 0; j < N; j++ ){
        sum.clear();
        for( i = 0; i < N; i++ ){
            if(adj[i][j]) sum += x[i][j];
        }
        model.add(sum == 1); 
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
        cout << "Variaveis de decisao (Agente -> Tarefa): " << endl;
        for( i = 0; i < N; i++ ){
            for( j = 0; j < N; j++ ){
                if(adj[i][j]){
                    value = IloRound(cplex.getValue(x[i][j]));
                    if(value > 0)
                        printf("x[%d][%d]: %.0lf\n", i, j, value); // Print 0-indexed
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
    if(!(cin >> N >> M)) return 0;
    
    c.assign(N, vector<int>(N, 0));
    adj.assign(N, vector<bool>(N, false));

    for(int i = 0; i < M; i++){
        int u_node, v_node, cost;
        cin >> u_node >> v_node >> cost;
        // Adjust for 0-indexed vectors (Removed)
        // u_node--; v_node--;
        adj[u_node][v_node] = true;
        c[u_node][v_node] = cost;
    }

    cplex();
    return 0;
}
