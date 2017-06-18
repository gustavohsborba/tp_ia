# Boids Neural
Um jogo parecido com boids que utiliza uma rede neural para decidir a acao dos boids
baseado no exemplo 14 do livro 'AI For Game Developers - David M. Bourg, Glenn Seeman'.

[Tarefas](#todo)

[Requisitos](#requisitos)

[Execução](#execução)

 # TODO:
 -     [ ] Colocar um objetivo no jogo (ex matar todo mundo)
 -     [X] Colocar os hitpoints do Unit[0] como entrada da rede neural
 -     [ ] Preparar o texto
 -     [ ] Preparar a apresentacao
 -     [ ] Re treinar a rede neural a cada período de tempo, e não só quando um cara morre.
 
 ## EXTRAS: 
 -     [ ] Colocar menu, menu de pausa e tela de 'voce ganhou'
 -     [ ] Criar uma versão sem jogador (com dois times)
 -     [X] Colocar o poder de ataque do jogador como entrada da rede neural

# Requisitos

## Atualizando Cmake no linux e Ubuntu

1. Remova a versao atual
```bash
sudo apt-get purge cmake
```

2. Va a pagina oficila [CMake webpage](https://cmake.org/download/) e baixe a ultima versao.
```bash
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v3.9/cmake-3.9.0-rc3.tar.gz
tar xzvf cmake-3.9.0-rc3.tar.gz
cd cmake-3.9.0-rc3
```

3. Instalando e rodando o script:
```bash
./bootstrap
make -j4
sudo make install
cp -r bin /usr/
cp -r doc /usr/share/
cp -r man /usr/share/
cp -r share /usr/
```

4. Teste a nova versao do cmake.
```bash
cmake --version
```

5. Resultados de cmake --version:
```
cmake version 3.9.0-rc3

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

## Instalando o OpenGL(Glut)

1. Instalando o freeglut.
```bash
sudo apt-get install freeglut3 freeglut3-dev
```

# Execução
Na pasta raiz do projeto

```bash
mkdir build
cd build
cmake ..
make
./tp_final
```

