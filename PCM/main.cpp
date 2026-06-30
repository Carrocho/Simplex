/*---------------- File: main.cpp  ---------------------+
|Modelo PLI - Problema do Caminho Minimo (PCM)          |
+-------------------------------------------------------+ */

#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>

using namespace std;
ILOSTLBEGIN 

#define CPLEX_TIME_LIM 3600

int N; //Qtd de vertices
int M; //Qtd de arestas
int S; //Origem
int T_dest; //Destino
vector<vector<int>> c; //Custo
vector<vector<bool>> adj; //Existe aresta?

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
                x[i].add(IloIntVar(env, 0, 1)); //Variaveis binarias
                numberVar++;
            }else{
                x[i].add(IloIntVar(env, 0, 0)); 
            }
        }
    }

    IloModel model ( env );
    IloExpr sum(env);
    IloExpr sum2(env);

    //FUNCAO OBJETIVO
    sum.clear();
    for( i = 0; i < N; i++ ){
        for( j = 0; j < N; j++ ){
            if(adj[i][j]){
                sum += (c[i][j] * x[i][j]);
            }
        }
    }
    model.add(IloMinimize(env, sum));

    //RESTRICOES
    //Conservacao de fluxo
    for( i = 0; i < N; i++ ){
        sum.clear(); //saindo de i
        for( j = 0; j < N; j++ ){
            if(adj[i][j]) sum += x[i][j];
        }
        sum2.clear(); //chegando em i
        for( j = 0; j < N; j++ ){
            if(adj[j][i]) sum2 += x[j][i];
        }
        
        int b = 0;
        if(i == S) b = 1;
        else if(i == T_dest) b = -1;
        
        model.add(sum - sum2 == b); 
        numberRes++;
    }

    //EXECUCAO
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
        cout << "Arestas no caminho minimo: " << endl;
        for( i = 0; i < N; i++ ){
            for( j = 0; j < N; j++ ){
                if(adj[i][j]){
                    value = IloRound(cplex.getValue(x[i][j]));
                    if(value > 0)
                        printf("x[%d][%d]: %.0lf\n", i+1, j+1, value); //Impressao com indice base 1
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
    sum2.end();
    env.end();
}

int main(){
    if(!(cin >> N >> M >> S >> T_dest)) return 0;
    
    //Ajuste para indice base 0
    S--; T_dest--;

    c.assign(N, vector<int>(N, 0));
    adj.assign(N, vector<bool>(N, false));

    for(int i = 0; i < M; i++){
        int u_node, v_node, cost;
        cin >> u_node >> v_node >> cost;
        u_node--; v_node--;
        adj[u_node][v_node] = true;
        c[u_node][v_node] = cost;
    }

    cplex();
    return 0;
}
