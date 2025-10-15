# Algoritmos para otimização de roteamento de bicicletas

Essa implementação foi apresentada como projeto final para a disciplina de Análise e Projeto de Algoritmos, na Universidade Federal da Paraíba, e seu objetivo é encontrar soluções ótimas (ou sub-ótimas) para um problema de roteamento de bicicletas, a partir da implementação de algoritmos gulosos e de busca local, por meio do algoritmo VND.

## Especificação

O problema consiste em planejar rotas para uma frota de *m* veículos idênticos, cada um com capacidade *Q*, que partem e retornam a um depósito central (vértice 0) para atender um conjunto de estações V = {1, 2, ..., n}. Cada estação possui uma demanda positiva (coleta de bicicletas) ou negativa (entrega de bicicletas). O objetivo é determinar até *m* rotas que atendam completamente todas as demandas, garantindo que a carga dos veículos nunca seja negativa nem ultrapasse a capacidade *Q*. Cada estação deve ser visitada exatamente uma vez, e as bicicletas transportadas podem ter origem ou destino tanto no depósito quanto em outras estações. Busca-se minimizar o custo total de viagem, representado pela soma dos custos associados a cada arco percorrido.

## Colaboradores
O desenvolvimento do código ficou sob responsabilidade de [Davi de Lacerda Teixeira](https://github.com/DavideLacerdaT), [João Victor Fernandes da Silveira](https://github.com/oiotave) e [Lael Gustavo Batista Ribeiro de Lima](https://github.com/sunny-fellow).

## Instruções de compilação

Certifique-se de executar os comandos um bash com suporte a Makefile, ou seja, qualquer bash em sistemas POSIX ou equivalentes para Windows.

1. Faça uma cópia local do repositório

```bash
git clone "github.com/oiotave/bike-routing-optimizer"
```

2. Compile o código-fonte completo

```bash
make all
```

3. Rode o código, passando o arquivo com o problema e o valor da solução ótima

```bash
./bin/program filepath.txt 4200
```

**Observação**: é de suma importância que, ao executar o código, você passe o caminho para o arquivo em texto com as especificações da instância do problema e o valor da solução ótima associada à instância. **Não passe o valor ótimo dentro do arquivo.**

## Notas

Devido a sua característica aleatória, os valores ótimos retornados pela função ```vndRandom``` podem ser diferentes de acordo com a execução.
