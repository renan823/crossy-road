# Crossy Road Multiplayer
---

### O jogo
---

### Implementação
Para o desenvolvimento do jogo, criamos duas bibliotecas utilitárias: uma
para manipular threads e outra para o servidor TCP.
</br>
O código implementa um servidor simples, que armazena as variáveis de
contexto do jogo (posição dos jogadores, mapa, etc), compartilhadas
com ambos os jogadores.
</br>
Cabe ao servidor do jogo carregar o mapa e também gerenciar as
vitórias e derrotas duarnte a partida.


### Para executar
---
Rode o servidor com:
```
make && make run
```
</br>

Rode um cliente como:
```
telnet 0.0.0.0 3000
```
