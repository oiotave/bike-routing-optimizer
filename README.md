# Algoritmos gulosos para resolução de problema de roteamento de bicicletas
A seguinte implementação foi apresentada como projeto final para a disciplina de Análise e Projeto de Algoritmos, na Universidade Federal da Paraíba. O código está completamente estruturado na linguagem C++ e seu objetivo é encontrar soluções ótimas (ou próximas o suficiente) para um problema de roteamento de bicicletas, por meio de algoritmos gulosos e de busca local. 

## Colaboradores
O desenvolvimento do código ficou sob responsabilidade de [Davi de Lacerda Teixeira](https://github.com/DavideLacerdaT), [João Victor Fernandes da Silveira](https://github.com/oiotave) e [Lael Gustavo Batista Ribeiro de Lima](https://github.com/sunny-fellow).

## Instruções de compilação
O projeto conta com o auxílio de um MAKEFILE para automatizar a compilação do código. Certifique-se que sua máquina tenha as dependências necessárias, caso ela não seja POSIX. Portanto, a compilação e execução é feita da seguinte forma:

```bash
# Compile usando make
make
```
```bash
# Rode o código, passando o arquivo com o problema e o valor da solução ótima
./bin/program filepath.txt 4200
```

É de suma importância que, ao executar o código, você passe o caminho para o arquivo em texto com as especificações da instância do problema e o valor da solução ótima associada à instância. **Não passe o valor ótimo dentro do arquivo.**

## Notas
Devido a sua característica aleatória, os valores ótimos retornados pela função ```vndRandom``` podem ser diferentes. Para efeitos de simplicidade e organização, foi selecionada a melhor resposta obtida através deste algoritmo.