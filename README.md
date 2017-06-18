# boids_neural
um jogo parecido com boids que utiliza uma rede neural para decidir a acao dos boids
baseado no exemplo 14 do livro 'AI For Game Developers - David M. Bourg, Glenn Seeman'

# boids_neural
um jogo parecido com boids que utiliza uma rede neural para decidir a acao dos boids
baseado no exemplo 14 do livro 'AI For Game Developers - David M. Bourg, Glenn Seeman'


#Atualizando Cmake no linux e Ubuntu

1. Uninstall your current version.
sudo apt-get purge cmake

2. Go to official [CMake webpage] (https://cmake.org/download/) then download and extract the latest version.
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v3.9/cmake-3.9.0-rc3.tar.gz
tar xzvf cmake-3.9.0-rc3.tar.gz
cd cmake-3.9.0-rc3

3. Install it by running:
./bootstrap
make -j4
sudo make install
cp -r bin /usr/
cp -r doc /usr/share/
cp -r man /usr/share/
cp -r share /usr/

4. Test your new cmake version.
cmake --version

5. Results of cmake --version:
cmake version 3.9.0-rc3

CMake suite maintained and supported by Kitware (kitware.com/cmake).

