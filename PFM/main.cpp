/*---------------- File: main.cpp  ---------------------+
|Modelo PLI - Problema de Fluxo Maximo (PFM)            |
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
vector<vector<int>> cap; //Capacidade
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
                x[i].add(IloIntVar(env, 0, cap[i][j])); //Variaveis inteiras limitadas pela capacidade
                numberVar++;
            }else{
                x[i].add(IloIntVar(env, 0, 0)); 
            }
        }
    }
    
    //Variavel de fluxo total
    IloNumVar v(env, 0, INT_MAX, ILOINT);
    numberVar++;

    IloModel model ( env );
    IloExpr sum(env);
    IloExpr sum2(env);

    //FUNCAO OBJETIVO
    model.add(IloMaximize(env, v));

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
        
        if(i == S){
            model.add(sum - sum2 == v); 
        }else if(i == T_dest){
            model.add(sum - sum2 == -v);
        }else{
            model.add(sum - sum2 == 0);
        }
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
        cout << "Fluxo nas arestas: " << endl;
        for( i = 0; i < N; i++ ){
            for( j = 0; j < N; j++ ){
                if(adj[i][j]){
                    value = IloRound(cplex.getValue(x[i][j]));
                    if(value > 0)
                        printf("x[%d][%d]: %.0lf\n", i+1, j+1, value); //Impressao com indice base 1
                }
            }
        }
        printf("\nFluxo Maximo Total = %.0lf\n", objValue);
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

    cap.assign(N, vector<int>(N, 0));
    adj.assign(N, vector<bool>(N, false));

    for(int i = 0; i < M; i++){
        int u_node, v_node, c;
        cin >> u_node >> v_node >> c;
        u_node--; v_node--;
        adj[u_node][v_node] = true;
        cap[u_node][v_node] = c;
    }

    cplex();
    return 0;
}
