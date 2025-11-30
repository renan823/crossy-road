# Crossy Road Multiplayer

### Alunos
<ul>
  <li>Pedro Bizon Dania (11812667)</li>
  <li>Renan Trofino Silva (15522316)</li>
</ul>

### O jogo
<p>O jogo consiste em atravessar as ruas e desviar dos carros. O player que alcançar o outro lado primeiro, ganha.</p>


### Implementação
Para o desenvolvimento do jogo, uma biblioteca de threads e semáforos foi criada, sendo genérica o suficiente para aplicar em qualquer parte do projeto.
</br></br>

O código implementa um servidor simples, que armazena as variáveis de
contexto do jogo (posição dos jogadores, mapa, etc), compartilhadas
com ambos os jogadores. Cabe ao servidor do jogo carregar o mapa e também gerenciar as
vitórias e derrotas duarnte a partida.
<br/></br>

No servidor, os dados compartilhados <strong>devem ser manipulados usando semáforos</strong>, já que são compartilhados entre
as threads de execução (responsável por atualizar estados) e as threads de requisição (criadas para lidar com requisições ao servidor).
</br></br>
No cliente, uma thread é responsável por renderizar a interface usando OpenGL (thread principal). Uma segunda thread é criada
para gerenciar a comunicação com o servidor, aguardado atualizações do estado do jogo.
</br>


### Para executar
Para a execução da interface, é necessário ter instalados os pacotes
gráficos do FreeGLUT.
Em distros Linux, é necessário instalar:
```
freeglut3 freeglut3-dev
```
</br>

Acesse a pasta do servidor `server/`. Então, execute:
```
make && make run
```
</br>

Cada player deve acessar um cliente.</br>
Acesse a pasta do cliente `client/`. Então, execute:
```
make && make run
```

Para mover o player, use as teclas WASD.
