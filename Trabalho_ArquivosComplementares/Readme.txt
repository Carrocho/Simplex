1) Instruções para Instalação no Linux/Ubuntu 64 bits:

1.1) Baixe o pacote (ILOGCPLEX_Ubuntu64.bin) do ILOG CPLEX no link do drive, na pasta ILOG CPLEX:

https://drive.google.com/drive/folders/15IpTiF5wy7uTT6cPGZnlNtJ2\_QEGL1oN?usp=sharing

1.2) Após baixar, abra o terminal, entre como "sudo", comando e sua senha do linux: 
sudo su

1.3) Vá no local do download do arquivo e dê permissão para o arquivo .bin usando o comando: 
chmod 777 ILOGCPLEX_Ubuntu64.bin

1.4) Para iniciar a instalação digite o comando: 
./ILOGCPLEX_Ubuntu64.bin

1.5) Espera, escolha o idioma, siga apertando "Enter", em algum momento digite 1 para aceitar
o contrato

1.6) Deixa a instalação na pasta padrão mesmo, o instalador faz isso.

1.7) Siga apertando "Enter" e uma barra de instalação surge no terminal.

1.8) Após concluir a instalação siga para o próximo passo.


2) Executando o modelo do Problema da Mochila 0/1:

2.1) Dentro da pasta ILOGCPLEX você encontra, uma pasta denominada Mochila01, abra-a.

2.2) A pasta contém:
- Código: main.cpp
- Dados de entrada: in.txt
- makefile

2.3) Assegure-se que você possui os comandos no seu linux:
- g++
- gcc
- make

Comando: sudo apt-get install -y make g++ gcc

2.3.1) (Opcional) Se usar o VSCode, ele não encontra a biblioteca ilocplex.h por padrão.

Abra as configurações da extensão C/C++ (Instalar caso não esteja instalada), buscando pelo comando (Usar Ctrl+Shift+P para abrir a caixa de comandos): C/C++ Edit Configuration (UI)

Para resolver basta adicionar o diretório de instalação no campo "Include path" dessa forma:
/opt/ibm/ILOG/CPLEX_Studio_Community221/**


2.4) Para compilar o código, no terminal digite o comando:
make

2.5) O comando vai criar um executável linux chamado main.exe

2.6) Use o comando para executar o código:
./main.exe < in.txt

2.7) Saída esperada em tela:
Verificacao da leitura dos dados:
Num. Itens: 8
Capacidade da mochila: 32
Itens - id: peso valor
1: 4 2
2: 15 2
3: 7 3
4: 9 4
5: 8 6
6: 10 5
7: 9 8
8: 11 7

--------Informacoes da Execucao:----------

#Var: 8
#Restricoes: 1
Memory usage after variable creation:  0.0340118 MB
Memory usage after cplex(Model):  0.0389481 MB
Version identifier: 22.1.0.0 | 2022-03-09 | 1a383f8ce
CPXPARAM_TimeLimit                               3600
Found incumbent of value 0.000000 after 0.00 sec. (0.00 ticks)
Tried aggregator 1 time.
MIP Presolve added 1 rows and 1 columns.
Reduced MIP has 2 rows, 9 columns, and 11 nonzeros.
Reduced MIP has 8 binaries, 1 generals, 0 SOSs, and 0 indicators.
Presolve time = 0.00 sec. (0.01 ticks)
Probing time = 0.00 sec. (0.00 ticks)
Tried aggregator 1 time.
Detecting symmetries...
MIP Presolve added 1 rows and 1 columns.
Reduced MIP has 3 rows, 10 columns, and 14 nonzeros.
Reduced MIP has 8 binaries, 2 generals, 0 SOSs, and 0 indicators.
Presolve time = 0.00 sec. (0.01 ticks)
Probing time = 0.00 sec. (0.00 ticks)
MIP emphasis: balance optimality and feasibility.
MIP search method: dynamic search.
Parallel mode: deterministic, using up to 4 threads.
Root relaxation solution time = 0.00 sec. (0.01 ticks)

        Nodes                                         Cuts/
   Node  Left     Objective  IInf  Best Integer    Best Bound    ItCnt     Gap

*     0+    0                            0.0000       37.0000              --- 
*     0+    0                           23.0000       37.0000            60.87%
      0     0        cutoff             23.0000       23.0000        1    0.00%
      0     0        cutoff             23.0000       23.0000        1    0.00%
Elapsed time = 0.01 sec. (0.05 ticks, tree = 0.01 MB, solutions = 2)

Root node processing (before b&c):
  Real time             =    0.01 sec. (0.05 ticks)
Parallel b&c, 4 threads:
  Real time             =    0.00 sec. (0.00 ticks)
  Sync time (average)   =    0.00 sec.
  Wait time (average)   =    0.00 sec.
                          ------------
Total (root+branch&cut) =    0.01 sec. (0.05 ticks)


Status da FO: Optimal
Variaveis de decisao: 
x[0]: 1
x[1]: 0
x[2]: 0
x[3]: 0
x[4]: 1
x[5]: 0
x[6]: 1
x[7]: 1

Funcao Objetivo Valor = 23
..(0.000000 seconds).

Memory usage before end:  0.034111 MB
