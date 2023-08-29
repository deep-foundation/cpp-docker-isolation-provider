# cpp-docker-isolation-provider

## Install/Build
```bash
pip install -r requirements.txt

apt-get install autoconf cmake make automake libtool git libboost-all-dev libssl-dev g++
#apt-get install libboost-python1.74-dev
cmake .
make

/etc/init.d/php8.1-fpm restart
```

#### or
```bash
docker build -t python-php-container .
docker run -it --rm python-php-container
```

## Local restart docker
```bash
docker build -t cpp-docker-isolation-provider .
docker run -d -p 39100:39100 -e PORT=39100 cpp-docker-isolation-provider
docker ps
```


## Check open ports
```bash
netstat -tuln
curl http://localhost:39100
```